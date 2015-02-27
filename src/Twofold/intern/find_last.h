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

#include <algorithm>

namespace Twofold {
namespace intern {

/// @returns a reverse iterator for a forward iterator
template<typename It>
std::reverse_iterator<It> revit(It it) {
    return std::reverse_iterator<It>(it);
}

/// @returns the forward iterator for a reverse iterator
template<typename It>
It fwdit(std::reverse_iterator<It> rit) {
    return rit.base();
}

/**
 * @brief find_last
 * @returns iterator to the last occurence of value
 *
 * works like std::find
 */
template<typename It, typename Type>
It find_last(It begin, It eend, const Type& value) {
    return fwdit(std::find(revit(eend), revit(begin), value));
}

} // namespace intern
} // namespace Twofold

