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

#include "bedrock/resources/resource_pack_stack.h"

#include "endstone/detail/hook.h"

PackInstance::PackInstance(Bedrock::NotNullNonOwnerPtr<ResourcePack> pack, int subpack_index, bool /*is_dependent*/,
                           PackSettings *pack_settings)
    : pack_(pack), subpack_index_(subpack_index), pack_settings_(pack_settings)
{
}

std::unique_ptr<ResourcePackStack> ResourcePackStack::deserialize(
    std::istream &file_stream, Bedrock::NotNullNonOwnerPtr<const IResourcePackRepository> const &repo)
{
    return ENDSTONE_HOOK_CALL_ORIGINAL(&ResourcePackStack::deserialize, file_stream, repo);
}
