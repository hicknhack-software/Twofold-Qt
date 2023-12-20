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

#include "Twofold/TextLoader.h"

#include <unordered_map>

class FakeTextLoader : public Twofold::TextLoader
{
public:
    auto load(const QString &name) const -> Result override
    {
        auto nameSearch = m_textSourceMap.find(name);
        if (nameSearch != m_textSourceMap.end()) {
            auto const& [_, text] = *nameSearch;
            return {Success, name, text};
        }
        qCritical() << "Text source" << name << "is not defined.";
        return { NotFound, QString(), QString() };
    }

    void addSource(const QString& name, const QString& text)
    {
        m_textSourceMap.insert({name, text});
    }
private:
    std::unordered_map<QString, QString> m_textSourceMap;
};

