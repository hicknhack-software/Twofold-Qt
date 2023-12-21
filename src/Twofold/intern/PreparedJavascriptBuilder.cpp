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
#include "PreparedJavascriptBuilder.h"

#include "Twofold/intern/QStringHelper.h"

#include <QRegularExpression>


namespace Twofold {
namespace intern {

namespace {

auto escapeForJavascriptString(const TextSpan& source) -> QString
{
    QString out;
    out.reserve(static_cast<int>(std::distance(source.begin, source.end)));
    for(auto c = source.begin; c != source.end; ++c) {
        switch (c->unicode()) {
            case '\\': out += "\\\\"; break;
            case '\r': out += "\\r"; break;
            case '\n': out += "\\n"; break;
            case '"' : out += "\\\""; break;
            case '\'': out += "\\'"; break;
            default: out += *c; break;
        }
    }

    return out;
}

auto buildOriginText(FileLineColumnPosition origin,
                           int columnOffset,
                           TextSpan span,
                           Interpolation interpolation = Interpolation::None)
    -> OriginText
{
    const FileLineColumnPosition originPosition { origin.name, {origin.line, origin.column + columnOffset} };
    return OriginText { originPosition, span, interpolation };
}

} // namespace

auto PreparedJavascriptBuilder::build() const -> PreparedJavascript
{
    auto sourceMapText = m_sourceMapBuilder.build();
    return PreparedJavascript { sourceMapText.text, sourceMapText.sourceMap, m_originPositions };
}

auto PreparedJavascriptBuilder::operator <<(const OriginScript &script) -> PreparedJavascriptBuilder &
{
    m_sourceMapBuilder << script.text << NewLine();
    return *this;
}

auto PreparedJavascriptBuilder::operator <<(const OriginScriptExpression &expr) -> PreparedJavascriptBuilder &
{
    static QString s_prefix("_template.pushPartIndent(%1);_template.appendExpression(");
    static QString s_postfix(", %1);_template.popPartIndent();"); // origin index

    if (0 == std::distance(expr.text.span.begin, expr.text.span.end))
        return *this; // avoid empty script expressions

    const auto originLength = std::distance(expr.text.span.begin, expr.text.span.end);
    const auto originIndex = this->addOriginPosition(expr.text.origin);

    const QString prefix = s_prefix.arg(originIndex);
    const QString postfix = s_postfix.arg(originIndex);

    m_sourceMapBuilder << buildOriginText(expr.text.origin, -2, prefix, Interpolation::None); // #{
    m_sourceMapBuilder << expr.text;
    m_sourceMapBuilder << buildOriginText(expr.text.origin, originLength, postfix, Interpolation::None); // }
    m_sourceMapBuilder << NewLine();
    return *this;
}

auto PreparedJavascriptBuilder::operator<<(const OriginTarget &target) -> PreparedJavascriptBuilder &
{
    static QString s_prefix("_template.append(\"");
    static QString s_postfix("\", %1);"); // origin index

    if (0 == std::distance(target.text.span.begin, target.text.span.end))
        return *this; // avoid empty text

    const auto originLength = std::distance(target.text.span.begin, target.text.span.end);
    const auto originIndex = this->addOriginPosition(target.text.origin);

    const QString postfix = s_postfix.arg(originIndex);

    m_sourceMapBuilder << buildOriginText(target.text.origin, -1, s_prefix, Interpolation::None);
    m_sourceMapBuilder << OriginText { target.text.origin, escapeForJavascriptString(target.text.span) };
    m_sourceMapBuilder << buildOriginText(target.text.origin, originLength, postfix, Interpolation::None);
    m_sourceMapBuilder << NewLine();
    return *this;
}

auto PreparedJavascriptBuilder::operator <<(const IndentTargetPart &indent) -> PreparedJavascriptBuilder &
{
    static QString s_prefix("_template.indentPart(\"");
    static QString s_postfix("\", %1);"); // origin index

    const auto originLength = std::distance(indent.text.span.begin, indent.text.span.end);
    const auto originIndex = this->addOriginPosition(indent.text.origin);

    const QString postfix = s_postfix.arg(originIndex);

    m_sourceMapBuilder << buildOriginText(indent.text.origin, -1, s_prefix, Interpolation::None);
    m_sourceMapBuilder << OriginText { indent.text.origin, escapeForJavascriptString(indent.text.span) };
    m_sourceMapBuilder << buildOriginText(indent.text.origin, originLength, postfix, Interpolation::None);
    m_sourceMapBuilder << NewLine();
    return *this;
}

auto PreparedJavascriptBuilder::operator <<(const PushTargetIndentation &indent) -> PreparedJavascriptBuilder &
{
    static QString s_prefix("_template.pushIndentation(\"");
    static QString s_postfix("\", %1);"); // origin index

    const auto originLength = std::distance(indent.text.span.begin, indent.text.span.end);
    const auto originIndex = this->addOriginPosition(indent.text.origin);

    const QString postfix = s_postfix.arg(originIndex);

    m_sourceMapBuilder << buildOriginText(indent.text.origin, -1, s_prefix, Interpolation::None);
    m_sourceMapBuilder << OriginText { indent.text.origin, escapeForJavascriptString(indent.text.span) };
    m_sourceMapBuilder << buildOriginText(indent.text.origin, originLength, postfix, Interpolation::None);
    m_sourceMapBuilder << NewLine();
    return *this;
}

auto PreparedJavascriptBuilder::operator <<(const PopTargetIndentation &indent) -> PreparedJavascriptBuilder &
{
    static QString s_code("_template.popIndentation();");

    m_sourceMapBuilder << OriginText { indent.text.origin, s_code, Interpolation::None };
    m_sourceMapBuilder << NewLine();
    return *this;
}

auto PreparedJavascriptBuilder::operator <<(const TargetNewLine newLine) -> PreparedJavascriptBuilder &
{
    static const QString s_code("_template.newLine(%1);");

    const size_t originIndex = this->addOriginPosition(newLine.text.origin);
    const QString code = s_code.arg(originIndex);

    m_sourceMapBuilder << OriginText { newLine.text.origin, code, Interpolation::None };
    m_sourceMapBuilder << NewLine();
    return *this;
}

auto PreparedJavascriptBuilder::operator <<(const NewLine) -> PreparedJavascriptBuilder &
{
    m_sourceMapBuilder << NewLine();
    return *this;
}

auto PreparedJavascriptBuilder::addOriginPosition(const FileLineColumnPosition &position) -> size_t
{
    m_originPositions.push_back(position);
    return m_originPositions.size() - 1;
}

} // namespace intern
} // namespace Twofold

// The following is for Visual Studio to mark the iterator as safe!
#ifdef _HAS_ONE_TYPE
template<>
struct std::_Is_checked_helper<Twofold::intern::QStringAppendIterator>
        : public std::_Is_checked_helper<std::back_insert_iterator<QString>>
{};
#endif
