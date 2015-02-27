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

#include "Twofold/MessageHandler.h"
#include "Twofold/TextLoader.h"

#include <QString>

#include <functional>
#include <vector>

namespace Twofold {
namespace intern {

class PreparedJavascriptBuilder;
struct LineCommand;

namespace Command {

/**
 * @brief This command includes a template
 * and returns the rendered content of that file.
 * Included templates have access to the variables of
 * the active context. If you are using the PathTextFileLoader,
 * the templates are looked for in the paths defined by it.
 */
class Include
{
public:
    using ProcessIncludedTextFunction = std::function<void(const QString &name, const QString &text)> ;
    using Stack = std::vector<QString>;

    Include(const MessageHandlerPtr &messageHandler,
            const TextLoaderPtr &textLoader,
            PreparedJavascriptBuilder &builder,
            const ProcessIncludedTextFunction &processIncludeText);

    void operator() (const LineCommand& command);

private:
    const MessageHandlerPtr m_messageHandler;
    const TextLoaderPtr m_textLoader;
    PreparedJavascriptBuilder &m_builder;
    const ProcessIncludedTextFunction m_processIncludeText;
    Stack m_stack;
};

} // namespace Command
} // namespace intern
} // namespace Twofold
