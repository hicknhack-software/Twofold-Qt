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
#include "TextBuilder.h"

namespace Twofold {
namespace intern {

Text TextBuilder::build() const
{
    return m_buffer;
}

const QString &TextBuilder::indentation() const
{
    return m_indentation;
}

int TextBuilder::line() const
{
    return m_line;
}

int TextBuilder::column() const
{
    if (0 == m_column) return 1 + m_indentation.length();
    return m_column;
}

bool TextBuilder::isBlankLine() const
{
    return 0 == m_column;
}

void TextBuilder::setIndentation(const QString &indent)
{
    m_indentation = indent;
}

TextBuilder &TextBuilder::operator<<(const TextSpan &textSpan)
{
    auto it = textSpan.begin;
    const auto end = textSpan.end;
    if (it == end)
        return *this; // empty text does not trigger indentation
    auto column = m_column;
    if (0 == column) m_buffer.append( m_indentation );
    while (true) {
        auto line_end = std::find(it, end, '\n');
        if (line_end != end) {
            ++m_line;
            column = 0;
            ++line_end;
        }
        else {
            if (0 == column) column = 1 + m_indentation.length();
            column += std::distance(it, end);
        }
        m_buffer.append(it, std::distance(it, line_end));
        if (line_end == end)
            break; // end of text reached
        it = line_end;
        m_buffer.append( m_indentation );
    }
    m_column = column;
    return *this;
}

TextBuilder &TextBuilder::operator<<(NewLine)
{
    ++m_line;
    m_column = 0;
    m_buffer.append('\n');
    return *this;
}

} // namespace intern
} // namespace Twofold
