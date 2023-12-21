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
#include "Engine.h"

#include "Twofold/intern/QtScriptTargetBuilderApi.h"

#include "Twofold/intern/QStringHelper.h"
#include "Twofold/intern/find_last.h"

#include <QJSEngine>
#include <QString>

#include <vector>

namespace Twofold {
using namespace intern;

namespace {

auto generateExceptionCallerStack(const PreparedTemplate &preparedTemplate, const QStringList &exceptionStackTrace) -> BacktraceFilePositionList
{
    using namespace Qt::StringLiterals;

    BacktraceFilePositionList callerStack;
    // NOTE(Lathan): stackframe message format: <function>:<line>:<column>:<filename>
    // NOTE(Lathan): E.g. for native functions, function name and file name can be empty and line number and column
    // can be -1
    for (const auto& frameMsg : exceptionStackTrace) {
        const QStringList msgParts = frameMsg.split(':', Qt::KeepEmptyParts);
        Q_ASSERT(msgParts.size() == 4);
        static constexpr qsizetype functionIndex{0};
        static constexpr qsizetype lineIndex{1};
        static constexpr qsizetype columnIndex{2};
        static constexpr qsizetype filenameIndex{3};
        const QString functionString = std::invoke([](const QString &functionString) {
            return not functionString.isEmpty() ? functionString : u"<unknown function>"_s;
        }, msgParts[functionIndex]);
        const QString lineString = msgParts[lineIndex];
        const QString columnString = msgParts[columnIndex];
        const QString filename = msgParts[filenameIndex];
        // NOTE(Lathan): The sourcemap knows the filenames.
        Q_UNUSED(filename);

        const std::optional<int> line = std::invoke([](const QString& valueString){
            bool ok{false};
            const int value = valueString.toInt(&ok);
            return (ok && value > 0) ? std::optional{value} : std::nullopt;
        }, lineString);
        const std::optional<int> column = std::invoke([](const QString& valueString){
            bool ok{false};
            int value = valueString.toInt(&ok);
            if (ok) value = std::max(1, value);
            return ok ? std::optional{value} : std::nullopt;
        }, columnString);

        if (line && column) {
            SourceMap::Position position {*line, *column};
            const SourceMap::FilePosition filePosition = SourceMap::getOriginalPositionFromGenerated(preparedTemplate.sourceMap, position);
            if (callerStack.empty() || (callerStack.back() != filePosition)) {
                    callerStack.push_back({functionString, filePosition});
                }
        }
    }
    return callerStack;
}

} // namespace

class Engine::Private
{
public:
    Private(MessageHandlerPtr messageHandler, TextLoaderPtr textLoader)
        : m_messageHandler(messageHandler)
        , m_textLoader(textLoader)
    {
    }

    auto execPrepared(const PreparedTemplate &preparedTemplate, const QVariantHash &inputs) -> Target
    {
        using namespace Qt::StringLiterals;

        QtScriptTargetBuilderApi scriptTargetBuilder(preparedTemplate.originPositions);
        defineInputs(inputs);
        defineTemplateApi(scriptTargetBuilder);

        QStringList exceptionStackTrace;
        QJSValue resultValue = m_scriptEngine.evaluate(preparedTemplate.javascript, u""_s, 1, &exceptionStackTrace);
        if (not exceptionStackTrace.isEmpty()) {
            showException(resultValue, preparedTemplate, exceptionStackTrace);
        }

        undefineTemplateApi();
        undefineInputs(inputs);

        const SourceMapText sourceMapText = scriptTargetBuilder.build();
        return { sourceMapText.sourceMap, sourceMapText.text };
    }

    auto createPreparedBuilder() -> PreparedTemplateBuilder {
        return { m_messageHandler, m_textLoader };
    }

private:
    void showException(const QJSValue& maybeError, const PreparedTemplate &preparedTemplate, const QStringList& exceptionStackTrace)
    {
        using namespace Qt::StringLiterals;
        BacktraceFilePositionList positionStack = generateExceptionCallerStack(preparedTemplate, exceptionStackTrace);
        const QString text = maybeError.isError() ? maybeError.toString() : u"Exception: %1"_s.arg(maybeError.toString());
        m_messageHandler->javaScriptMessage(MessageType::Error, positionStack, text);
    }

    void defineTemplateApi(QtScriptTargetBuilderApi &templateApi)
    {
        QJSValue global = m_scriptEngine.globalObject();
        global.setProperty("_template", m_scriptEngine.newQObject(&templateApi));
    }

    void undefineTemplateApi()
    {
        QJSValue global = m_scriptEngine.globalObject();
        global.setProperty("_template", QJSValue(QJSValue::UndefinedValue));
    }

    void defineInputs(const QVariantHash &inputs)
    {
        QJSValue global = m_scriptEngine.globalObject();
        for (auto key : inputs.keys()) {
            global.setProperty( key, m_scriptEngine.toScriptValue(inputs[key]) );
        }
    }

    void undefineInputs(const QVariantHash &inputs)
    {
        QJSValue global = m_scriptEngine.globalObject();
        for (auto key : inputs.keys()) {
            global.setProperty(key, QJSValue(QJSValue::UndefinedValue));
        }
    }

    MessageHandlerPtr m_messageHandler;
    TextLoaderPtr m_textLoader;
    QJSEngine m_scriptEngine;
};

Engine::Engine(MessageHandlerPtr messageHandler, TextLoaderPtr textLoader)
    : m_private(new Private(messageHandler, textLoader))
{
}

Engine::Engine(TextLoaderPtr textLoader, MessageHandlerPtr messageHandler)
    : Engine(messageHandler, textLoader)
{

}

Engine::~Engine() = default;

auto Engine::exec(const PreparedTemplate &preparedTemplate, const QVariantHash &inputs) -> Target
{
    return m_private->execPrepared(preparedTemplate, inputs);
}

auto Engine::prepare(const QString &templateName) const -> PreparedTemplate
{
    auto prepared = m_private->createPreparedBuilder().build(templateName);
    return prepared;
}

auto Engine::execTemplateName(const QString &templateName, const QVariantHash &inputs) -> Target
{
    auto prepared = this->prepare(templateName);
    return this->exec(prepared, inputs);
}

} // namespace Twofold
