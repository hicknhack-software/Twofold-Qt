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

#include <vector>

namespace Twofold {
using namespace intern;

namespace {

BacktraceFilePositionList generateExceptionCallerStack(const PreparedTemplate &preparedTemplate, const QStringList &backtrace)
{
    BacktraceFilePositionList callerStack;
    for (const auto& traceLine : backtrace) {
        // traceline format: "<function>() at <line>"
        auto lineBegin = traceLine.begin();
        const auto end = traceLine.end();
        lineBegin = find_last(lineBegin, end, QChar(' '));
        const auto lineString = toQString(lineBegin, end);

        bool convertSuccesful = false;
        const int line = lineString.toInt(&convertSuccesful);
        const int column = 1;
        if (convertSuccesful && (line > 0)) {
            const SourceMap::FilePosition position = SourceMap::getOriginalPositionFromGenerated(preparedTemplate.sourceMap, {line, column});
            if (callerStack.empty() || (callerStack.back() != position)) {
                const auto functionEnd = std::find(traceLine.begin(), end, QChar('('));
                const auto functionString = toQString(traceLine.begin(), functionEnd);
                callerStack.push_back({functionString, position});
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

    Target execPrepared(const PreparedTemplate &preparedTemplate, const QVariantHash &inputs)
    {
        QtScriptTargetBuilderApi scriptTargetBuilder(preparedTemplate.originPositions);
        defineInputs(inputs);
        defineTemplateApi(scriptTargetBuilder);

        QJSValue resultValue = m_scriptEngine.evaluate(preparedTemplate.javascript);
        if (resultValue.isError()) {
            showError(resultValue, preparedTemplate);
        }

        undefineTemplateApi();
        undefineInputs(inputs);

        const SourceMapText sourceMapText = scriptTargetBuilder.build();
        return { sourceMapText.sourceMap, sourceMapText.text };
    }

    PreparedTemplateBuilder createPreparedBuilder() {
        return { m_messageHandler, m_textLoader };
    }

private:
    void showError(const QJSValue& resultValue, const PreparedTemplate &preparedTemplate)
    {
        //const QStringList backtrace = m_scriptEngine.uncaughtExceptionBacktrace();
        BacktraceFilePositionList positionStack = {}; //generateExceptionCallerStack(preparedTemplate, backtrace);
        // const int line = m_scriptEngine.uncaughtExceptionLineNumber();
        // const int column = 1; // TODO: use agent and stack!
        // positionStack.insert(positionStack.begin(), {QString(), SourceMap::getOriginalPositionFromGenerated(preparedTemplate.sourceMap, {line, column})});
        const QString text = "Uncaught Exception: " + resultValue.toString();
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

Target Engine::exec(const PreparedTemplate &preparedTemplate, const QVariantHash &inputs)
{
    return m_private->execPrepared(preparedTemplate, inputs);
}

PreparedTemplate Engine::prepare(const QString &templateName) const
{
    auto prepared = m_private->createPreparedBuilder().build(templateName);
    return prepared;
}

Target Engine::execTemplateName(const QString &templateName, const QVariantHash &inputs)
{
    auto prepared = this->prepare(templateName);
    return this->exec(prepared, inputs);
}

} // namespace Twofold
