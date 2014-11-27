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
#ifndef FAKETEXTLOADER_H
#define FAKETEXTLOADER_H

#include "Twofold/TextLoader.h"

class FakeTextLoader : public Twofold::TextLoader
{
public:
    FakeTextLoader(const QString &text)
        : m_text(text)
    {}

    Result load(const QString &name) const override
    {
        static QString includeTemplate = "| Include Text";
        static QString recursiveTemplate = "# include \"recursive.twofold\"";

        if(name == "include.twofold")
            return { Success, QString(), includeTemplate };
        if(name == "errorLoading.twofold")
            return { ErrorLoading, QString(), QString() };
        if(name == "notFound.twofold")
            return { NotFound, QString(), QString() };
        if(name == "recursive.twofold")
            return { Success, QString(), recursiveTemplate };

        return { Success, QString(), m_text };
    }

private:
    QString m_text;
};

#endif // FAKETEXTLOADER_H
