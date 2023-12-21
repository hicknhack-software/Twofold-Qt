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
#include "PathTextFileLoader.h"

#include <QDir>
#include <QTextStream>

namespace Twofold {

PathTextFileLoader::PathTextFileLoader()
{
}

void PathTextFileLoader::addPath(const QString &path)
{
    m_pathes.insert(m_pathes.begin(), path);
}

auto PathTextFileLoader::absolutePath(const QString &name) const -> const QString
{
    for (const auto& path : m_pathes) {
        const QString fullPath = QDir(path).absoluteFilePath(name);
        if (!QFile::exists(fullPath))
            continue; // file not found, try next path

        QFile file(fullPath);
        file.open(QIODevice::ReadOnly);
        if (!file.isOpen())
            continue; // file not readable, try next path

        return fullPath; // readable path found
    }
    return {};
}

auto PathTextFileLoader::load(const QString &name) const -> PathTextFileLoader::Result
{
    QString candidate;
    for (const auto& path : m_pathes) {
        const QString fullPath = QDir(path).absoluteFilePath(name);
        if (!QFile::exists(fullPath))
            continue; // file not found, try next path

        candidate = fullPath;
        QFile file(fullPath);
        file.open(QIODevice::ReadOnly);
        if (!file.isOpen())
            continue; // file not readable, try next path

        return { Success, fullPath,  QTextStream(&file).readAll() };
    }
    return { candidate.isEmpty() ? NotFound : ErrorLoading, candidate, QString() };
}

} // namespace Twofold
