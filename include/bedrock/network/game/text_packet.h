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

#pragma once

#include <string>
#include <vector>

#include "bedrock/network/packet.h"

enum class TextPacketType : char {
    Raw = 0,
    Chat = 1,
    Translate = 2,
    Popup = 3,
    JukeboxPopup = 4,
    Tip = 5,
    SystemMessage = 6,
    Whisper = 7,
    Announcement = 8,
    TextObjectWhisper = 9,
    TextObject = 10,
    TextObjectAnnouncement = 11
};

class TextPacket : public Packet {
public:
private:
    TextPacketType type_;                // +48
    std::string unknown7_;               // +56
    std::string unknown8_;               // +88
    std::vector<std::string> unknown9_;  // +120
    std::int32_t unknown10_;             // +144
    std::string unknown11_;              // +152
    std::string unknown12_;              // +184
};
// static_assert(sizeof(TextPacket) == 216);
