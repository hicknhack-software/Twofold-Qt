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

#include "Twofold/SourceMap.h"
#include "Twofold/MessageHandler.h"

#include <tuple>

class FakeMessageHandler : public Twofold::MessageHandler
{
public:
    std::vector< std::tuple<Type, QString> > messageStore;
    std::vector< std::tuple<Type, Position, QString> > templateStore;
    std::vector< std::tuple<Type, PositionStack, QString> > javascriptStore;

    void message(Twofold::MessageType type, const Text& text) override
    {
        messageStore.emplace_back( type, text);
    }

    void templateMessage(Twofold::MessageType type,
                         const Position& position,
                         const Text& text) override
    {
        templateStore.emplace_back( type, position, text );
    }

    void javaScriptMessage(Type type,
                           const PositionStack &positionStack,
                           const Text& text) override
    {
        javascriptStore.emplace_back( type, positionStack, text );
    }
};

