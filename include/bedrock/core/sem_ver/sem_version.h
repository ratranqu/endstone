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

class SemVersion {
public:
    [[nodiscard]] const std::string &asString() const;
    bool operator==(const SemVersion &version) const;

private:
    std::uint16_t major_;              // +0
    std::uint16_t minor_;              // +2
    std::uint16_t patch_;              // +4
    std::string pre_release_;          // +8
    std::string build_meta_;           // +40
    std::string full_version_string_;  // +72
    bool valid_version_;               // +104
    bool any_version_;                 // +105
};
