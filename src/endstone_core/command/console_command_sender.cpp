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

#include "endstone/detail/command/console_command_sender.h"

namespace endstone::detail {

void ConsoleCommandSender::sendMessage(const std::string &message) const
{
    getServer().getLogger().info(message);
}

void ConsoleCommandSender::sendErrorMessage(const std::string &message) const
{
    getServer().getLogger().error(message);
}

std::string ConsoleCommandSender::getName() const
{
    return "CONSOLE";
}

bool ConsoleCommandSender::isOp() const
{
    return true;
}

void ConsoleCommandSender::setOp(bool value)
{
    getServer().getLogger().error("Cannot change operator status of server console");
}

}  // namespace endstone::detail
