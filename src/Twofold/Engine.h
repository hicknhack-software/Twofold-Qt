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
#ifndef TWOFOLD_TARGETBUILDER_H
#define TWOFOLD_TARGETBUILDER_H

#include "Twofold/PreparedTemplateBuilder.h"

#include "Twofold/MessageHandler.h"
#include "Twofold/TextLoader.h"

#include <QVariantHash>
#include <QString>
#include <memory>

namespace Twofold {

struct Target
{
    const SourceMapping sourceMap;
    const QString text;
};

/**
 * @brief Main interface for Twofold
 *
 * It's not thread safe!
 * A single QScriptEngine is encapsulated, that changes states during execution.
 */
class Engine
{
public:
    Engine(MessageHandlerPtr messageHandler, TextLoaderPtr textLoader);
    Engine(TextLoaderPtr textLoader, MessageHandlerPtr messageHandler);

    void showTemplateSyntaxErrors(const PreparedTemplate &preparedTemplate) const;

    Target exec(const PreparedTemplate &preparedTemplate, const QVariantHash &inputs);

    PreparedTemplate prepare(const QString &templateName) const;

    Target execTemplateName(const QString &templateName, const QVariantHash &inputs);

private:
    class Private;
    struct PrivateDeleter {
        void operator()(Private* p) const;
    };
    const std::unique_ptr<Private, PrivateDeleter> m_private;
};

} // namespace Twofold

#endif // TWOFOLD_TARGETBUILDER_H
