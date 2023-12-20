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
#include "PreparedTemplateBuilder.h"

#include "Twofold/intern/Command/Include.h"
#include "Twofold/intern/Command/Missing.h"

#include "Twofold/intern/Line/Passtrough.h"
#include "Twofold/intern/Line/Interpolation.h"
#include "Twofold/intern/Line/InterpolateLine.h"
#include "Twofold/intern/Line/Call.h"
#include "Twofold/intern/Line/Command.h"

#include "Twofold/intern/LineProcessor.h"
#include "Twofold/intern/PreparedJavascriptBuilder.h"

namespace Twofold {
using namespace intern;

class PreparedTemplateBuilder::Private
{
public:
    Private(const MessageHandlerPtr &messageHandler, const TextLoaderPtr &textLoader)
        : messageHandler(messageHandler)
        , textLoader(textLoader)
        , lineProcessor(buildLineProcessor())
    {}

    inline LineProcessor buildLineProcessor()
    {
        LineProcessor::Map map;
        map['\\'] = Line::Interpolation(messageHandler, preparedJavascriptBuilder);
        map['|'] = Line::InterpolateLine(messageHandler, preparedJavascriptBuilder);
        map['='] = Line::Call(messageHandler, preparedJavascriptBuilder);
        map['#'] = Line::Command(buildLineCommand());
        LineProcessor::Function fallback = Line::Passtrough(preparedJavascriptBuilder);
        return { std::move(map), std::move(fallback) };
    }

    inline Line::Command buildLineCommand()
    {
        Line::Command::Map map;
        map["include"] = Command::Include(messageHandler,
                                          textLoader,
                                          preparedJavascriptBuilder,
                                          std::ref(lineProcessor));
        Line::Command::Function fallback = Command::Missing(messageHandler);
        return { std::move(map), std::move(fallback) };
    }

    inline PreparedTemplate build(const QString &name) const
    {
        const auto result = textLoader->load(name);
        if (result.status != TextLoader::Success) {
            return { QString{}, SourceMapping{{}}, FileLineColumnPositionList{} }; // could not load
        }

        lineProcessor(result.name, result.text);
        const auto preparedJavascript = preparedJavascriptBuilder.build();
        return { preparedJavascript.javascript, preparedJavascript.sourceMap, preparedJavascript.originPositions };
    }

    const MessageHandlerPtr messageHandler;
    const TextLoaderPtr textLoader;
    PreparedJavascriptBuilder preparedJavascriptBuilder;
    const LineProcessor lineProcessor;
};

PreparedTemplateBuilder::~PreparedTemplateBuilder() = default;

PreparedTemplateBuilder::PreparedTemplateBuilder(MessageHandlerPtr messageHandler, TextLoaderPtr textLoader)
    : m_private(new Private(messageHandler, textLoader))
{
}

PreparedTemplateBuilder::PreparedTemplateBuilder(TextLoaderPtr textLoader, MessageHandlerPtr messageHandler)
    : PreparedTemplateBuilder(messageHandler, textLoader)
{
}

PreparedTemplate PreparedTemplateBuilder::build(const QString &name) const
{
    return m_private->build(name);
}

} // namespace Twofold
