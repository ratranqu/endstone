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

#include <memory>

#include "bedrock/world/scores/scoreboard.h"
#include "endstone/scoreboard/scoreboard.h"

namespace endstone::detail {

class EndstoneScoreboard : public Scoreboard {
public:
    explicit EndstoneScoreboard(::Scoreboard &board);
    std::shared_ptr<Objective> addObjective(std::string name, Criteria::Type criteria) override;
    std::shared_ptr<Objective> addObjective(std::string name, Criteria::Type criteria,
                                            std::string display_name) override;
    std::shared_ptr<Objective> addObjective(std::string name, Criteria::Type criteria, std::string display_name,
                                            RenderType render_type) override;
    [[nodiscard]] std::shared_ptr<Objective> getObjective(std::string name) const override;
    [[nodiscard]] std::shared_ptr<Objective> getObjective(DisplaySlot slot) const override;
    [[nodiscard]] std::vector<std::shared_ptr<Objective>> getObjectives() const override;
    [[nodiscard]] std::vector<std::shared_ptr<Objective>> getObjectivesByCriteria(
        Criteria::Type criteria) const override;
    [[nodiscard]] std::vector<std::shared_ptr<Score>> getScores(ScoreEntry entry) const override;
    void resetScores(ScoreEntry entry) override;
    [[nodiscard]] std::vector<ScoreEntry> getEntries() const override;
    void clearSlot(DisplaySlot slot) override;

private:
    friend class EndstoneObjective;
    friend class EndstoneScore;

    ::Scoreboard &board_;
};

}  // namespace endstone::detail
