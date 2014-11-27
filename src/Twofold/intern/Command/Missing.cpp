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
#include "Missing.h"

#include "Twofold/intern/Line/Command.h"

#include "Twofold/intern/LineProcessor.h"
#include "Twofold/intern/QStringHelper.h"

namespace Twofold {
namespace intern {
namespace Command {

Missing::Missing(const MessageHandlerPtr &messageHandler)
    : m_messageHandler(messageHandler)
{
}

void Missing::operator()(const LineCommand &command) const
{
    QString message = QString("unknown command %1").arg( toQString(command.begin, command.end) );
    m_messageHandler->templateMessage(MessageType::Error, command.line.position, message);
}

} // namespace Command
} // namespace intern
} // namespace Twofold
