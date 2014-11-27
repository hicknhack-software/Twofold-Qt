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
#include "LineProcessor.h"

#include "QCharHelper.h"

#include <algorithm>

namespace Twofold {
namespace intern {

namespace {

const LineProcessor::Function& findProcessor(const LineProcessor::Map& map,
                                             const LineProcessor::Function& fallback,
                                             const FileLine& line)
{
    if (line.firstNonSpace != line.end) {
        auto it = map.find(*line.firstNonSpace);
        if (it != map.end())
            return it->second;
    }
    return fallback;
}

} // namespace

LineProcessor::LineProcessor(LineProcessor::Map &&map, LineProcessor::Function &&fallback)
    : m_map(std::move(map))
    , m_fallback(std::move(fallback))
{
    Q_ASSERT(m_fallback);
}

void LineProcessor::operator()(const QString &name, const QString &text) const
{
    FileLine line { FileLineColumnPosition { name, {0, 0} }, text.begin(), 0, 0 };
    const QString::const_iterator textEnd = text.end();
    while (line.begin != textEnd) {
        line.position.line++;
        line.firstNonSpace = std::find_if_not(line.begin, textEnd, QCharHelper::isSpace);
        line.end = std::find_if(line.firstNonSpace, textEnd, QCharHelper::isNewline);

        findProcessor(m_map, m_fallback, line)(line);

        if (line.end == textEnd)
            break; // end of file
        line.begin = std::find_if_not(line.end + 1, textEnd, QCharHelper::IsComplementNewline(*line.end));
    }
}

} // namespace intern
} // namespace Twofold
