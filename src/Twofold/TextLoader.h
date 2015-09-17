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

#include <QString>

#include <memory>

namespace Twofold {

class TextLoader; // local forward
using TextLoaderPtr = std::shared_ptr<TextLoader>;

/**
 * @brief Interface for resolving and loading text files
 *
 * You can fetch files from memory or from a network.
 */
class TextLoader
{
public:
    enum Status {
        Success, ///< File was found and loading was sucessfull
        NotFound, ///< File was not found
        ErrorLoading, ///< File was found, error occured while loading it
    };
    struct Result {
        Status status;
        QString name; ///< resolved name (used in logs and sourceMaps) - only valid if found
        QString text; ///< content - empty unless success
    };

public:
    virtual Result load(const QString& name) const = 0;
};

} // namespace Twofold
