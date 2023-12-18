// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include "endstone/core/endstone_server.h"

#include "endstone/core/logger_factory.h"

EndstoneServer::EndstoneServer() : logger_(LoggerFactory::getLogger("Server"))
{
    plugin_manager_ = std::make_unique<SimplePluginManager>(*this);
    //    plugin_manager_->registerLoader(std::make_unique<PythonPluginLoader>(*this, "endstone.plugin",
    //    "ZipPluginLoader")); plugin_manager_->registerLoader(
    //        std::make_unique<PythonPluginLoader>(*this, "endstone.plugin", "SourcePluginLoader"));
    //    plugin_manager_->registerLoader(std::make_unique<CppPluginLoader>(*this));
}

void EndstoneServer::loadPlugins()
{
    auto plugin_folder = std::filesystem::current_path() / "plugins";

    if (exists(plugin_folder)) {
        auto plugins = plugin_manager_->loadPlugins(plugin_folder);
        for (const auto &plugin : plugins) {
            plugin->getLogger().info("Loading {}", plugin->getDescription().getFullName());
            plugin->onLoad();
        }
    }
    else {
        create_directories(plugin_folder);
    }
}

void EndstoneServer::enablePlugins() const
{
    auto plugins = plugin_manager_->getPlugins();
    for (const auto &plugin : plugins) {
        if (!plugin->isEnabled()) {
            enablePlugin(*plugin);
        }
    }
}

void EndstoneServer::enablePlugin(Plugin &plugin) const
{
    plugin_manager_->enablePlugin(plugin);
}

void EndstoneServer::disablePlugins() const
{
    plugin_manager_->disablePlugins();
}

Logger &EndstoneServer::getLogger() const
{
    return logger_;
}

PluginManager &EndstoneServer::getPluginManager() const
{
    return *plugin_manager_;
}
