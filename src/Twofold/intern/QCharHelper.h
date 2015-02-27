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

#include <QChar>

namespace Twofold {
namespace intern {
namespace QCharHelper {

inline bool isSpace(QChar chr) { return chr.category() == QChar::Separator_Space || chr == '\t'; }
inline bool isNewline(QChar chr) { return chr == '\n' || chr == '\r'; }

/**
 * functor structure
 */
class IsComplementNewline
{
public:
    inline IsComplementNewline(QChar newLine)
        : m_complement(newLine == '\n' ? '\r' : '\n')
    {}

    inline bool operator() (QChar chr) {
        return m_complement == chr;
    }
private:
    QChar m_complement;
};

} // namespace QCharHelper
} // namespace intern
} // namespace Twofold

