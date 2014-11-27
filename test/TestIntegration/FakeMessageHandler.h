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
#ifndef FAKEMESSAGEHANDLER_H
#define FAKEMESSAGEHANDLER_H

#include "Twofold/SourceMap.h"
#include "Twofold/MessageHandler.h"

#include <QMetaType>
#include <QPair>

struct MessageCount
{
    explicit MessageCount(int message = 0, int templateMessage = 0, int javaScriptMessage = 0)
        : message(message)
        , templateMessage(templateMessage)
        , javaScriptMessage(javaScriptMessage)
    {}

    int message = 0;
    int templateMessage = 0;
    int javaScriptMessage = 0;
};
Q_DECLARE_METATYPE(MessageCount)

class FakeMessageHandler : public Twofold::MessageHandler
{
public:
    void message(Twofold::MessageType type, const Text& text) override
    {
        count.message++;
        store.emplace_back( qMakePair(type, text) );
    }

    void templateMessage(Twofold::MessageType type,
                         const Position& position,
                         const Text& text) override
    {
        count.templateMessage++;
        this->MessageHandler::templateMessage(type, position, text);
    }

    void javaScriptMessage(Type type,
                           const PositionStack &positionStack,
                           const Text& text) override
    {
        errorTemplatePosition = positionStack.front();
        count.javaScriptMessage++;
        this->MessageHandler::javaScriptMessage(type, positionStack, text);
    }

    void display()
    {
        for(auto pair : store) this->MessageHandler::message(pair.first, pair.second);
    }

public:
    std::vector< QPair<Type, QString> > store;
    MessageCount count;
    Position errorTemplatePosition;
};

#endif // FAKEMESSAGEHANDLER_H
