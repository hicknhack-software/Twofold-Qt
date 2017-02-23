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

#include "Twofold/intern/SourceMapTextBuilder.h"

#include <QString>

#include <vector>

namespace Twofold {
namespace intern {

enum class OriginTextType {
    Script, ScriptExpression, Target, IndentPart, PushIndention, PopIndention, NewLine
};

template<OriginTextType>
struct OriginTextTemplate {
    OriginText text;
};

using OriginScript = OriginTextTemplate<OriginTextType::Script>;
using OriginScriptExpression = OriginTextTemplate<OriginTextType::ScriptExpression>;
using OriginTarget = OriginTextTemplate<OriginTextType::Target>;
using IndentTargetPart = OriginTextTemplate<OriginTextType::IndentPart>;
using PushTargetIndentation = OriginTextTemplate<OriginTextType::PushIndention>;
using PopTargetIndentation = OriginTextTemplate<OriginTextType::PopIndention>;
using TargetNewLine = OriginTextTemplate<OriginTextType::NewLine>;

/**
 * @brief data structure representing the prepared javascript
 */
template <typename SourceMapping>
struct PreparedJavascript
{
    const QString javascript;
    const SourceMapping sourceMap;
    const FileLineColumnPositionList originPositions;
};

/**
 * @brief Builds template rendering javascript and source maps
 *
 * builder class
 */
class PreparedJavascriptBuilder
{
public:
    template <typename SourceMapping>
    PreparedJavascript<SourceMapping> build() const;

    inline const QString& indentation() const { return m_sourceMapBuilder.indentation(); }
    inline void setIndentation(const QString &indent) { m_sourceMapBuilder.setIndentation(indent); }

    PreparedJavascriptBuilder &operator <<(const OriginScript& script);
    PreparedJavascriptBuilder &operator <<(const OriginScriptExpression& expr);
    PreparedJavascriptBuilder &operator <<(const OriginTarget& target);

    PreparedJavascriptBuilder &operator <<(const IndentTargetPart& indent);
    PreparedJavascriptBuilder &operator <<(const PushTargetIndentation& indent);
    PreparedJavascriptBuilder &operator <<(const PopTargetIndentation &indent);

    PreparedJavascriptBuilder &operator <<(const TargetNewLine newLine);

    PreparedJavascriptBuilder &operator <<(const NewLine);

private:
    size_t addOriginPosition(const FileLineColumnPosition &position);

private:
    SourceMapTextBuilder m_sourceMapBuilder;
    FileLineColumnPositionList m_originPositions;
};

} // namespace intern
} // namespace Twofold

