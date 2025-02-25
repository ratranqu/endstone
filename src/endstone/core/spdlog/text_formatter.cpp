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

#include "endstone/core/spdlog/text_formatter.h"

#include <spdlog/details/fmt_helper.h>

namespace endstone::core {

void TextFormatter::format(const spdlog::details::log_msg &msg, const tm &, spdlog::memory_buf_t &dest)
{

    const auto &input = msg.payload;
    for (std::size_t i = 0; i < input.size(); i++) {
        // Check for the § character in UTF-8 (0xC2A7) and ensure there's a following character
        if (i + 2 < input.size() && static_cast<unsigned char>(input[i]) == 0xC2 &&
            static_cast<unsigned char>(input[i + 1]) == 0xA7) {
            i += 2;                  // Skip §
            if (i < input.size()) {  // if there's a color code character to after §
                if (should_do_colors_) {
                    auto format_code = static_cast<unsigned char>(input[i]);
                    auto it = ansi_codes.find(format_code);
                    if (it != ansi_codes.end()) {
                        spdlog::details::fmt_helper::append_string_view(it->second, dest);
                    }
                    else {
                        fmt::format_to(std::back_inserter(dest), "{}", input[i]);
                    }
                }
            }
        }
        else {
            // Append the current character to the result if it's not part of a color code
            fmt::format_to(std::back_inserter(dest), "{}", input[i]);
        }
    }
}

std::unique_ptr<spdlog::custom_flag_formatter> TextFormatter::clone() const
{
    return spdlog::details::make_unique<TextFormatter>(should_do_colors_);
}

const std::unordered_map<unsigned char, spdlog::string_view_t> TextFormatter::ansi_codes = {
    // References: https://minecraft.wiki/w/Formatting_codes

    // Color codes
    {ColorFormat::Black.back(), "\x1b[30m"},
    {ColorFormat::DarkBlue.back(), "\x1b[34m"},
    {ColorFormat::DarkGreen.back(), "\x1b[32m"},
    {ColorFormat::DarkAqua.back(), "\x1b[36m"},
    {ColorFormat::DarkRed.back(), "\x1b[31m"},
    {ColorFormat::DarkPurple.back(), "\x1b[35m"},
    {ColorFormat::Gold.back(), "\x1b[33m"},
    {ColorFormat::Gray.back(), "\x1b[37m"},
    {ColorFormat::DarkGray.back(), "\x1b[90m"},
    {ColorFormat::Blue.back(), "\x1b[94m"},
    {ColorFormat::Green.back(), "\x1b[92m"},
    {ColorFormat::Aqua.back(), "\x1b[96m"},
    {ColorFormat::Red.back(), "\x1b[91m"},
    {ColorFormat::LightPurple.back(), "\x1b[95m"},
    {ColorFormat::Yellow.back(), "\x1b[93m"},
    {ColorFormat::White.back(), "\x1b[97m"},
    {ColorFormat::MinecoinGold.back(), "\x1b[38;2;221;214;5m"},
    {ColorFormat::MaterialQuartz.back(), "\x1b[38;2;227;212;209m"},
    {ColorFormat::MaterialIron.back(), "\x1b[38;2;206;202;202m"},
    {ColorFormat::MaterialNetherite.back(), "\x1b[38;2;68;58;59m"},
    {ColorFormat::MaterialRedstone.back(), "\x1b[38;2;151;22;7m"},
    {ColorFormat::MaterialCopper.back(), "\x1b[38;2;180;104;77m"},
    {ColorFormat::MaterialGold.back(), "\x1b[38;2;222;177;45m"},
    {ColorFormat::MaterialEmerald.back(), "\x1b[38;2;17;160;54m"},
    {ColorFormat::MaterialDiamond.back(), "\x1b[38;2;44;186;168m"},
    {ColorFormat::MaterialLapis.back(), "\x1b[38;2;33;73;123m"},
    {ColorFormat::MaterialAmethyst.back(), "\x1b[38;2;154;92;198m"},
    {ColorFormat::MaterialResin.back(), "\x1b[38;2;234;113;19m"},

    // Formatting codes
    {ColorFormat::Obfuscated.back(), "\x1b[8m"},
    {ColorFormat::Bold.back(), "\x1b[1m"},
    {ColorFormat::Italic.back(), "\x1b[3m"},
    {ColorFormat::Reset.back(), "\x1b[0m"},
};

}  // namespace endstone::core
