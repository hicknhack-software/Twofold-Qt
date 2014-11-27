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
#include "Command.h"

#include "Twofold/intern/LineProcessor.h"
#include "Twofold/intern/QCharHelper.h"
#include "Twofold/intern/QStringHelper.h"

#include <algorithm>

namespace Twofold {
namespace intern {
namespace Line {

namespace {

const Command::Function& findFunction(const Command::Map& map,
                                      const Command::Function& fallback,
                                      const LineCommand& command)
{
    auto it = map.find( toQString(command.begin, command.end) );
    if (it != map.end())
        return it->second;
    return fallback;
}

} // namespace

Command::Command(Map &&map, Function &&fallback)
    : m_map(std::move(map))
    , m_fallback(std::move(fallback))
{
}

void Command::operator()(const FileLine &line) const
{
    LineCommand command { line, 0, 0 };
    command.begin = std::find_if_not(line.firstNonSpace+1, line.end, QCharHelper::isSpace);
    command.end = std::find_if(command.begin, line.end, QCharHelper::isSpace);

    findFunction(m_map, m_fallback, command)(command);
}

} // namespace Line
} // namespace intern
} // namespace Twofold
