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
#include "SourceMapTextBuilder.h"

namespace Twofold {
namespace intern {

SourceMapText SourceMapTextBuilder::build() const
{
    return SourceMapText { SourceMapping(m_sourceData), m_textBuilder.build() };
}

void SourceMapTextBuilder::pushCaller(const FileLineColumnPosition &originPosition)
{
    auto parentIndex = m_callerIndexStack.empty() ? CallerIndex{} : m_callerIndexStack.back();
    auto index = CallerIndex{ (int)SourceMap::get< ExtCaller >(m_sourceData).size()};
    SourceMap::get< ExtCaller >(m_sourceData).push_back(Caller { originPosition, parentIndex });
    m_callerIndexStack.push_back(index);
}

void SourceMapTextBuilder::popCaller()
{
    m_callerIndexStack.pop_back();
}

SourceMapTextBuilder &SourceMapTextBuilder::operator <<(const OriginText &originText)
{
    if (originText.span.begin == originText.span.end)
        return *this; // empty span would create double entries

    auto callerIndex = m_callerIndexStack.empty() ? CallerIndex{} : m_callerIndexStack.back();
    m_sourceData.entries.push_back({{m_textBuilder.line(), m_textBuilder.column()},
                                    originText.origin,
                                    std::make_tuple(originText.interpolation, callerIndex)});

    m_textBuilder << originText.span;
    return *this;
}

SourceMapTextBuilder &SourceMapTextBuilder::operator <<(const NewLine)
{
    m_textBuilder << NewLine();
    return *this;
}

} // namespace intern
} // namespace Twofold
