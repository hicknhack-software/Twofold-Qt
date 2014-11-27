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
#include "Include.h"

#include "Twofold/intern/Line/Command.h"
#include "Twofold/intern/PreparedJavascriptBuilder.h"
#include "Twofold/intern/QStringHelper.h"

#include <QDebug>

#include <algorithm>
#include <cassert>

namespace Twofold {
namespace intern {
namespace Command {

namespace {

#define MAX_INCLUDE_STACK_DEPTH 1000

inline bool isDoubleQuote(QChar chr) { return chr == '"'; }

QString extractNameArgument(const LineCommand &command)
{
    auto nameBegin = std::find_if(command.end, command.line.end, isDoubleQuote);
    if (nameBegin == command.line.end)
        return {}; // no initial double quote found
    nameBegin++;
    auto nameEnd = std::find_if(nameBegin, command.line.end, isDoubleQuote);
    if (nameEnd == command.line.end)
        return {}; // no second double quote found
    return toQString(nameBegin, nameEnd);
}

void reportParameterError(const MessageHandlerPtr &messageHandler, const LineCommand &command, const char *message)
{
    messageHandler->templateMessage(MessageType::Error, command.line.position, message);
}

void reportLoaderError(const MessageHandlerPtr &messageHandler, const LineCommand &command, const QString &name, TextLoader::Status status)
{
    const QString message = (status == TextLoader::NotFound)
            ? QString("Could not find %1").arg(name)
            : QString("Could not load %1").arg(name);

    messageHandler->templateMessage(MessageType::Error, command.line.position, message);
}

void reportStackError(const MessageHandlerPtr &messageHandler, const LineCommand &command, const Include::Stack &stack)
{
    QStringList messageLines;
    messageLines << QString("Include depth too deep: %1").arg(stack.size());
    for (auto name : stack)
        messageLines.prepend(QString("included: %1").arg(name));

    messageHandler->templateMessage(
                MessageType::Error,
                command.line.position,
                messageLines.join('\n'));
}

struct StackEntry {
    StackEntry(Include::Stack& stack, const QString& name)
        : m_stack(stack)
#ifdef assert
        , m_name(name)
        , m_size(m_stack.size())
#endif
    {
        m_stack.push_back(name);
    }

    ~StackEntry()
    {
#ifdef assert
        assert(m_stack.size() == m_size + 1);
        assert(m_stack.back() == m_name);
#endif
        m_stack.pop_back();
    }
private:
    Include::Stack &m_stack;
#ifdef assert
    QString m_name;
    size_t m_size;
#endif
};

} // namespace

Include::Include(const MessageHandlerPtr &messageHandler, const TextLoaderPtr &textLoader, PreparedJavascriptBuilder &builder, const ProcessIncludedTextFunction &processIncludeText)
    : m_messageHandler(messageHandler)
    , m_textLoader(textLoader)
    , m_builder(builder)
    , m_processIncludeText(processIncludeText)
{
}

void Include::operator()(const LineCommand &command)
{
    const QString name = extractNameArgument(command);
    if (name.isEmpty()) {
        reportParameterError(m_messageHandler, command, "filename argument in \"quotes\" required!");
        return; // skip include continue with next line
    }

    auto result = m_textLoader->load( name );
    if (result.status != TextLoader::Success) {
        reportLoaderError(m_messageHandler, command, name, result.status);
        return; // skip include continue with next line
    }

    StackEntry stackEntry(m_stack, name);
    (void)stackEntry;
    if (m_stack.size() > MAX_INCLUDE_STACK_DEPTH) {
        reportStackError(m_messageHandler, command, m_stack);
        return; // skip include continue with next line
    }

    auto indentBegin = command.line.firstNonSpace + 1;
    m_builder << PushTargetIndentation {{command.line, TextSpan{indentBegin, command.begin}}};
    m_processIncludeText(result.name, result.text);
    m_builder << PopTargetIndentation {{command.line, TextSpan{command.end, command.end}}};
}

} // namespace Command
} // namespace intern
} // namespace Twofold
