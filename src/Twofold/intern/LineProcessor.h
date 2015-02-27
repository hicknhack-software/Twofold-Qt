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

#include <QString>

#include <map>
#include <functional>

namespace Twofold {
namespace intern {

/**
 * @brief Parameter structure for line processor functions
 *
 * poco
 */
struct FileLine
{
    FileLineColumnPosition position;
    QString::const_iterator begin, end;
    QString::const_iterator firstNonSpace;
};

/**
 * @brief processes lines based on the first character
 *
 * functor service
 */
class LineProcessor
{
public:
    using Function = std::function<void (const FileLine&)>;
    using Map = std::map<QChar, Function>;

public:
    LineProcessor(Map&& map, Function&& fallback);

    void operator() (const QString &name, const QString &text) const;

private:
    const Map m_map;
    const Function m_fallback;
};

} // namespace intern
} // namespace Twofold

