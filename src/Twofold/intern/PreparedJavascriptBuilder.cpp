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

struct QStringAppendIterator
        : public std::iterator<std::output_iterator_tag, void, void, void, void>
{
    inline QStringAppendIterator(QString& dst) : m_dst(dst) {}

    inline QStringAppendIterator& operator=(const QString& n)
    {
        m_dst += n;
        return *this;
    }

    inline QStringAppendIterator& operator*() { return *this; }
    inline QStringAppendIterator& operator++() { return *this; }

private:
    QString& m_dst;
};

QString escapeForJavascriptString(const TextSpan& source)
{
    QString out;
    out.reserve(std::distance(source.begin, source.end));
    std::transform(source.begin, source.end, QStringAppendIterator(out),
                   [](QChar chr) -> QString {
        switch (chr.unicode()) {
        case '\\': return "\\\\";
        case '\r': return "\\r";
        case '\n': return "\\n";
        case '"' : return "\\\"";
        case '\'': return "\\'";
        default: return chr;
        }
    });
    return out;
}

OriginText buildOriginText(FileLineColumnPosition origin,
                           int columnOffset,
                           TextSpan span,
                           Interpolation interpolation = Interpolation::None)
{
    const FileLineColumnPosition originPosition { origin.name, {origin.line, origin.column + columnOffset} };
    return OriginText { originPosition, span, interpolation };
}

} // namespace

PreparedJavascript PreparedJavascriptBuilder::build() const
{
    auto sourceMapText = m_sourceMapBuilder.build();
    return PreparedJavascript { sourceMapText.text, sourceMapText.sourceMap, m_originPositions };
}

PreparedJavascriptBuilder &PreparedJavascriptBuilder::operator <<(const OriginScript &script)
{
    m_sourceMapBuilder << script.text << NewLine();
    return *this;
}

PreparedJavascriptBuilder &PreparedJavascriptBuilder::operator <<(const OriginScriptExpression &expr)
{
    static QString s_prefix("_template.pushPartIndent(%1);_template.append(");
    static QString s_postfix(", %1);_template.popPartIndent();"); // origin index

    if (0 == std::distance(expr.text.span.begin, expr.text.span.end))
        return *this; // avoid empty script expressions

    const int originLength = std::distance(expr.text.span.begin, expr.text.span.end);
    const int originIndex = this->addOriginPosition(expr.text.origin);

    const QString prefix = s_prefix.arg(originIndex);
    const QString postfix = s_postfix.arg(originIndex);

    m_sourceMapBuilder << buildOriginText(expr.text.origin, -2, prefix, Interpolation::None); // #{
    m_sourceMapBuilder << expr.text;
    m_sourceMapBuilder << buildOriginText(expr.text.origin, originLength, postfix, Interpolation::None); // }
    m_sourceMapBuilder << NewLine();
    return *this;
}

PreparedJavascriptBuilder &PreparedJavascriptBuilder::operator<<(const OriginTarget &target)
{
    static QString s_prefix("_template.append(\"");
    static QString s_postfix("\", %1);"); // origin index

    if (0 == std::distance(target.text.span.begin, target.text.span.end))
        return *this; // avoid empty text

    const int originLength = std::distance(target.text.span.begin, target.text.span.end);
    const int originIndex = this->addOriginPosition(target.text.origin);

    const QString postfix = s_postfix.arg(originIndex);

    m_sourceMapBuilder << buildOriginText(target.text.origin, -1, s_prefix, Interpolation::None);
    m_sourceMapBuilder << OriginText { target.text.origin, escapeForJavascriptString(target.text.span) };
    m_sourceMapBuilder << buildOriginText(target.text.origin, originLength, postfix, Interpolation::None);
    m_sourceMapBuilder << NewLine();
    return *this;
}

PreparedJavascriptBuilder &PreparedJavascriptBuilder::operator <<(const IndentTargetPart &indent)
{
    static QString s_prefix("_template.indentPart(\"");
    static QString s_postfix("\", %1);"); // origin index

    const int originLength = std::distance(indent.text.span.begin, indent.text.span.end);
    const int originIndex = this->addOriginPosition(indent.text.origin);

    const QString postfix = s_postfix.arg(originIndex);

    m_sourceMapBuilder << buildOriginText(indent.text.origin, -1, s_prefix, Interpolation::None);
    m_sourceMapBuilder << OriginText { indent.text.origin, escapeForJavascriptString(indent.text.span) };
    m_sourceMapBuilder << buildOriginText(indent.text.origin, originLength, postfix, Interpolation::None);
    m_sourceMapBuilder << NewLine();
    return *this;
}

PreparedJavascriptBuilder &PreparedJavascriptBuilder::operator <<(const PushTargetIndentation &indent)
{
    static QString s_prefix("_template.pushIndentation(\"");
    static QString s_postfix("\", %1);"); // origin index

    const int originLength = std::distance(indent.text.span.begin, indent.text.span.end);
    const int originIndex = this->addOriginPosition(indent.text.origin);

    const QString postfix = s_postfix.arg(originIndex);

    m_sourceMapBuilder << buildOriginText(indent.text.origin, -1, s_prefix, Interpolation::None);
    m_sourceMapBuilder << OriginText { indent.text.origin, escapeForJavascriptString(indent.text.span) };
    m_sourceMapBuilder << buildOriginText(indent.text.origin, originLength, postfix, Interpolation::None);
    m_sourceMapBuilder << NewLine();
    return *this;
}

PreparedJavascriptBuilder &PreparedJavascriptBuilder::operator <<(const PopTargetIndentation &indent)
{
    static QString s_code("_template.popIndentation();");

    m_sourceMapBuilder << OriginText { indent.text.origin, s_code, Interpolation::None };
    m_sourceMapBuilder << NewLine();
    return *this;
}

PreparedJavascriptBuilder &PreparedJavascriptBuilder::operator <<(const TargetNewLine newLine)
{
    static QString s_code("_template.newLine();");

    m_sourceMapBuilder << OriginText { newLine.text.origin, s_code, Interpolation::None };
    m_sourceMapBuilder << NewLine();
    return *this;
}

PreparedJavascriptBuilder &PreparedJavascriptBuilder::operator <<(const NewLine)
{
    m_sourceMapBuilder << NewLine();
    return *this;
}

int PreparedJavascriptBuilder::addOriginPosition(const FileLineColumnPosition &position)
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
