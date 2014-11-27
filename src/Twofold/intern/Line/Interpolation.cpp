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
#include "Interpolation.h"

#include "Twofold/intern/PreparedJavascriptBuilder.h"
#include "Twofold/intern/LineProcessor.h"

#include "Twofold/intern/QStringHelper.h"
#include "Twofold/intern/QCharHelper.h"

#include <algorithm>

namespace Twofold {
namespace intern {
namespace Line {

namespace {

#define HASH '#'
#define CURLY_OPEN '{'
#define CURLY_CLOSE '}'
#define DOUBLE_QUOTE '"'
#define SINGLE_QUOTE '\''
#define BACK_SLASH '\\'

inline bool isHash(QChar chr) { return chr == HASH; }

inline bool isCurlyOrQuote(QChar chr)
{
    switch (chr.unicode())
    {
    case CURLY_OPEN:
    case CURLY_CLOSE:
    case DOUBLE_QUOTE:
    case SINGLE_QUOTE:
        return true;
    default:
        return false;
    }
}

QString::const_iterator findQuoteEnd(const QString::const_iterator& begin, const FileLine& line)
{
    QChar quote = *begin;
    auto it = begin + 1;
    while (it != line.end) {
        if (*it == quote) return it; // found closing quote
        if (*it == BACK_SLASH) {
            it++;
            if (it == line.end) break; // invalid (TODO: continue next line)
        }
        it++;
    }
    return it; // invalid
}

QString::const_iterator findExpressionEnd(QString::const_iterator begin, const FileLine& line)
{
    int depth = 0;
    while (begin != line.end) {
        auto it = std::find_if(begin, line.end, isCurlyOrQuote);
        if (it == line.end) return it; // invalid
        switch (it->unicode())
        {
        case CURLY_CLOSE:
            if (0 == depth) return it; // found end
            depth--;
            break;
        case CURLY_OPEN:
            depth++;
            break;
        default: // quote
            it = findQuoteEnd(it, line);
            if (it == line.end) return it; // invalid
        }
        begin = it + 1;
    }
    return begin;
}

void reportError(const MessageHandlerPtr& messageHandler, const FileLine &line, const QString &message)
{
    messageHandler->templateMessage(MessageType::Error, line.position, message);
}

} // namespace

Interpolation::Interpolation(const MessageHandlerPtr &messageHandler, PreparedJavascriptBuilder &builder)
    : m_messageHandler(messageHandler)
    , m_builder(builder)
{
}

void Interpolation::operator()(const FileLine &line) const
{
    const auto beginIndent = line.firstNonSpace+1;
    auto begin = std::find_if_not(beginIndent, line.end, QCharHelper::isSpace);
    m_builder << IndentTargetPart {{line, TextSpan {beginIndent, begin}}};

    auto end = begin;
    while (begin != line.end) {
        end = std::find_if(end, line.end, isHash);
        if (end == line.end) break; // reached line end
        auto expressionBegin = end + 1;
        if (expressionBegin == line.end) break; // reached line end
        switch (expressionBegin->unicode())
        {
        case HASH:
            m_builder << OriginTarget {{line, TextSpan{begin, expressionBegin}}};
            begin = end = expressionBegin + 1;
            continue; // double ExprInitiator - one is skipped

        case CURLY_OPEN:
            m_builder << OriginTarget {{line, TextSpan{begin, end}}};
            expressionBegin++;
            auto expressionEnd = findExpressionEnd(expressionBegin, line);
            if (expressionEnd == line.end) {
                reportError(m_messageHandler, line, "Missing close bracket!");
                begin = line.end;
                break; // terminate as invalid javascript
            }
            m_builder << OriginScriptExpression {{line, TextSpan{expressionBegin, expressionEnd}}};
            begin = end = expressionEnd + 1; // skip closing brackets
            continue; // expression evaluated
        }
        end = expressionBegin + 1;
    }
    m_builder << OriginTarget {{line, TextSpan{begin, line.end}}};
}

} // namespace Line
} // namespace intern
} // namespace Twofold
