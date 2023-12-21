/* Twofold-Qt
 * (C) Copyright 2014 HicknHack Software GmbH
 *
 * The original code can be found at:
 *     https://github.com/hicknhack-software/Twofold-Qt
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include "Twofold/TextLoader.h"

#include <vector>
#include <memory>

namespace Twofold {

class PathTextFileLoader;
using PathTextFileLoaderPtr = std::shared_ptr<PathTextFileLoader>;

/**
 * @brief Searches filenames in a set of Pathes
 */
class PathTextFileLoader : public TextLoader
{
public:
    PathTextFileLoader();

    void addPath(const QString& path);

    /// @returns resolved full filename (empty if no readable candidate was found)
    auto absolutePath(const QString &name) const -> const QString;

    // TextFileLoader interface
public:
    auto load(const QString &name) const -> Result override;

private:
    std::vector< QString > m_pathes;
};

} // namespace Twofold
