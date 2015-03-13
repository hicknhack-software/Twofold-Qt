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

#include "Twofold/intern/LineProcessor.h"

namespace Twofold {
namespace intern {
namespace Javascript {

class BraceCounter
{
public:
    using String = QString;
    using It = QString::const_iterator;
    /**
     * @return the nesting level of braces at the end
     * >0 means that not all braces were closed
     * <0 means more braces are closed than opened
     *
     * It does not enforce that the braces are matching pairs
     */
    static int countExpressionDepth(It begin, It end, int beginDepth = 0);

    /**
     * @return the iterator to the end of the javascript expression
     * will return end if no end of the expression was found
     */
    static It findExpressionEnd(It begin, It end);
};

} // namespace Javascript
} // namespace intern
} // namespace Twofold
