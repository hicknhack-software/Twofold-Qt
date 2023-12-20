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
#include "MessageHandler.h"

#include <QDebug>

#include <algorithm>

namespace Twofold {

void MessageHandler::message(Type type, const Text &text)
{
    using namespace Qt::StringLiterals;
    switch( type ) {
    case Type::Info:
        ::qDebug().noquote() << u"Info:"_s << text;
        break;
    case Type::Warning:
        ::qDebug().noquote() << u"Warning:"_s << text;
        break;
    case Type::Error:
        ::qDebug().noquote() << u"Error:"_s << text;
        break;
    default:
        ::qDebug().noquote() << u"Unknown:"_s << text;
    }
}

void MessageHandler::templateMessage(Type type,
                                     const Position &position,
                                     const Text &text)
{
    using namespace Qt::StringLiterals;
    const auto composed = u"%1:%2 Template Error: %3"_s.arg(position.name).arg(position.line).arg(text);
    this->message(type, composed);
}

void MessageHandler::javaScriptMessage(Type type,
                                       const PositionStack &positionStack,
                                       const Text &text)
{
    using namespace Qt::StringLiterals;
    QStringList lines;
    lines << u"Scripting Error: %4"_s.arg(text);
    for (const BacktraceFilePosition &position : positionStack) {
        lines << u"  from %1:%2[%3]"_s.arg(position.name).arg(position.line).arg(position.column);
    }
    this->message(type, lines.join('\n'));
}

} // namespace Twofold
