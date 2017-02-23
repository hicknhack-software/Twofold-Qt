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

#include "SourceMap/Extension/Caller.h"
#include "SourceMap/Extension/Interpolation.h"

#include "SourceMap/Mapping.h"

#include <vector>

namespace Twofold {

// The SourceMap type specialization for Twofold

using FileLineColumnPosition = SourceMap::FilePosition;
using FileLineColumnPositionList = std::vector< FileLineColumnPosition >;

using ExtInterpolation = SourceMap::Extension::Interpolation;
using ExtColumnFormatInterpolation = SourceMap::Extension::ColumnFormatInterpolation;
using Interpolation = SourceMap::Interpolation;

using ExtCaller = SourceMap::Extension::Caller;
using ExtCallstackFormatCaller = SourceMap::Extension::CallstackFormatCaller;
using Caller = SourceMap::Caller;
using CallerList = SourceMap::CallerList;
using CallerIndex = SourceMap::CallerIndex;

using SourceMapping = SourceMap::Mapping< ExtInterpolation, ExtCaller >;
using SourceData = SourceMapping::Data;
using SourceEntry = SourceMapping::Entry;

using CallstackSourceMapping = SourceMap::Mapping< ExtColumnFormatInterpolation, ExtCallstackFormatCaller >;
using CallstackSourceData = CallstackSourceMapping::Data;
using CallstackSourceEntry = CallstackSourceMapping::Entry;

#ifdef _MSC_VER
extern template SourceEntry;
extern template SourceMapping;

extern template CallstackSourceData;
extern template CallstackSourceEntry;
#endif

} // namespace Twofold
