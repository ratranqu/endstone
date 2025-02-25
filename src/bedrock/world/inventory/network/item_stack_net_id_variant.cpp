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

#include "bedrock/world/inventory/network/item_stack_net_id_variant.h"

#include "bedrock/platform/brstd/variant.h"

std::string ItemStackNetIdVariant::toString() const
{
    return std::visit(brstd::overloaded{
                          [](TypedServerNetId<ItemStackNetIdTag> id) -> std::string {
                              return "[ItemStackNetId: " + id.toString() + "]";
                          },
                          [](TypedClientNetId<ItemStackRequestIdTag> id) -> std::string {
                              return "[ItemStackRequestId: " + id.toString() + "]";
                          },
                          [](TypedClientNetId<ItemStackLegacyRequestIdTag> id) -> std::string {
                              return "[ItemStackLegacyRequestId: " + id.toString() + "]";
                          },
                      },
                      variant_);
}
