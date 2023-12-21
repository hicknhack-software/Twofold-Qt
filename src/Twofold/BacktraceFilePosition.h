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

#include "Twofold/SourceMap.h"

namespace Twofold {

struct BacktraceFilePosition : public FileLineColumnPosition
{
    /// constructs an invalid position
    BacktraceFilePosition()
        : BacktraceFilePosition(QString(), FileLineColumnPosition())
    {}

    /// constructs a valid position
    BacktraceFilePosition(QString functionName, const FileLineColumnPosition &filePosition)
        : FileLineColumnPosition(filePosition)
        , functionName(std::move(functionName))
    {}

    auto operator==(BacktraceFilePosition const& obj) const -> bool = default;

    inline auto isValid() const -> bool { return ! functionName.isEmpty() && this->FileLineColumnPosition::isValid(); }

    QString functionName;
};

using BacktraceFilePositionList = std::vector< BacktraceFilePosition >;

} // namespace Twofold
