// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "endstone/detail/command/command_wrapper.h"

#include <bedrock/locale/i18n.h>
#include <boost/algorithm/string.hpp>

#include "bedrock/server/commands/command_origin_loader.h"
#include "endstone/detail/level/level.h"

namespace endstone::detail {

CommandWrapper::CommandWrapper(MinecraftCommands &minecraft_commands, std::unique_ptr<Command> command)
    : Command(*command), minecraft_commands_(minecraft_commands), command_(std::move(command))
{
}

bool CommandWrapper::execute(CommandSender &sender, const std::vector<std::string> &args) const
{
    if (!testPermission(sender)) {
        return true;
    }

    const auto command_origin = getCommandOrigin(sender);
    if (!command_origin) {
        throw std::runtime_error("Unsupported command origin type");
    }

    // compile command
    std::vector command_parts = {"/" + getName()};
    command_parts.insert(command_parts.end(), args.begin(), args.end());
    const auto full_command = boost::algorithm::join(command_parts, " ");
    const auto *command = minecraft_commands_.compileCommand(  //
        full_command, *command_origin, CurrentCmdVersion::Latest,
        [&sender](auto const &err) { sender.sendErrorMessage(err); });

    // run the command
    CommandOutput output{CommandOutputType::AllOutput};
    command->run(*command_origin, output);

    // redirect outputs to sender
    for (const auto &message : output.getMessages()) {
        switch (message.getType()) {
        case CommandOutputMessageType::Success:
            sender.sendMessage(getI18n().get(message.getMessageId(), message.getParams(), nullptr));
            break;
        case CommandOutputMessageType::Error:
            sender.sendErrorMessage(getI18n().get(message.getMessageId(), message.getParams(), nullptr));
            break;
        default:
            throw std::runtime_error("Unsupported CommandOutputMessageType");
        }
    }

    return output.getSuccessCount() > 0;
}

PluginCommand *CommandWrapper::asPluginCommand() const
{
    return unwrap().asPluginCommand();
}

Command &CommandWrapper::unwrap() const
{
    return *command_;
}

std::unique_ptr<CommandOrigin> CommandWrapper::getCommandOrigin(CommandSender &sender)
{
    auto &server = entt::locator<EndstoneServer>::value();

    if (auto *console = sender.asConsole(); console) {
        CompoundTag tag;
        {
            tag.putByte("OriginType", static_cast<std::uint8_t>(CommandOriginType::DedicatedServer));
            tag.putString("RequestId", "00000000-0000-0000-0000-000000000000");
            tag.putByte("CommandPermissionLevel", static_cast<std::uint8_t>(CommandPermissionLevel::Owner));
            tag.putString("DimensionId", "overworld");
        }
        auto *level = static_cast<EndstoneLevel *>(server.getLevel());
        return CommandOriginLoader::load(tag, static_cast<ServerLevel &>(level->getHandle()));
    }

    if (auto *player = static_cast<EndstonePlayer *>(sender.asPlayer()); player) {
        CompoundTag tag;
        {
            tag.putByte("OriginType", static_cast<std::uint8_t>(CommandOriginType::Player));
            tag.putInt64("PlayerId", player->getHandle().getOrCreateUniqueID().raw_id);
        }
        return CommandOriginLoader::load(tag, static_cast<ServerLevel &>(player->getHandle().getLevel()));
    }

    // TODO: entity

    // TODO: if sender is CommandOriginWrapper, unwrap the origin and return its clone
    return nullptr;
}

}  // namespace endstone::detail
