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

#include "Twofold/MessageHandler.h"

#include <QMetaType>

struct MessageCount
{
    MessageCount(int message = 0, int templateMessage = 0, int javaScriptMessage = 0)
        : message(message)
        , templateMessage(templateMessage)
        , javaScriptMessage(javaScriptMessage)
    {}

    bool operator==(MessageCount &other) const
    {
        return message == other.message
                && templateMessage == other.templateMessage
                && javaScriptMessage == other.javaScriptMessage;
    }

    int message = 0;
    int templateMessage = 0;
    int javaScriptMessage = 0;
};
Q_DECLARE_METATYPE(MessageCount)

class FakeMessageHandler : public Twofold::MessageHandler
{
public:
    void message(Type, const Text&) override
    {
        count.message++;
    }

    void templateMessage(Type, const Position&, const Text&) override
    {
        count.templateMessage++;
    }

    void javaScriptMessage(Type, const PositionStack &, const Text&) override
    {
        count.javaScriptMessage++;
    }

public:
    MessageCount count;
};

#endif // FAKEMESSAGEHANDLER_H
