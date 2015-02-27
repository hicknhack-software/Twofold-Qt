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

namespace Twofold {
namespace intern {

struct LineCommand;

namespace Command {

/**
 * @brief This command could be used as default behavior
 * in order to compensate missing commands or user spelling
 * mistakes. It emits a error message to the handler.
 */
class Missing
{
public:
    Missing(const MessageHandlerPtr& messageHandler);

    void operator() (const LineCommand& command) const;

private:
    const MessageHandlerPtr m_messageHandler;
};

} // namespace Command
} // namespace intern
} // namespace Twofold

