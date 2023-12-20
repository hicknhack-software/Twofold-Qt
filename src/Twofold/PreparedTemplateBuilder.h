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

#include "Twofold/SourceMap.h"

#include <QString>
#include <memory>

namespace Twofold {

struct PreparedTemplate
{
    const QString javascript;
    const SourceMapping sourceMap;
    const FileLineColumnPositionList originPositions;

    inline PreparedTemplate& operator=(const PreparedTemplate& src) {
        this->~PreparedTemplate();
        new(this) PreparedTemplate(src);
        return *this;
    }
};

/**
 * @brief Use this interface for building prepared templates
 *
 * This might be handy if you dont want to instanciate a QScriptEngine.
 * Otherwise prefer the Engine.
 */
class PreparedTemplateBuilder
{
public:
    PreparedTemplateBuilder(MessageHandlerPtr messageHandler, TextLoaderPtr textLoader);
    PreparedTemplateBuilder(TextLoaderPtr textLoader, MessageHandlerPtr messageHandler);
    virtual ~PreparedTemplateBuilder();

    PreparedTemplate build(const QString &name) const;

private:
    class Private;
    const std::unique_ptr<Private> m_private;
};

} // namespace Twofold
