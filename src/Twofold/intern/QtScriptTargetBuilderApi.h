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

#include "Twofold/intern/SourceMapTextBuilder.h"
#include "Twofold/SourceMap.h"

#include <QObject>
#include <vector>

namespace Twofold {
namespace intern {

/**
 * @brief Target Builder with a QScript callable Api
 *
 * all public slots are callable by Javascript
 */
class QtScriptTargetBuilderApi : public QObject
{
    Q_OBJECT

public:
    QtScriptTargetBuilderApi(const FileLineColumnPositionList &originPositions);

    template <typename SourceMapping>
    inline SourceMapText<SourceMapping> build() const { return m_sourceMapBuilder.build<SourceMapping>(); }

public slots:
    void append(const QString &text, int originIndex);
    void newLine();

    void pushIndentation(const QString &indent, int originIndex);
    void popIndentation();

    void indentPart(const QString& indent, int originIndex);
    void pushPartIndent(int originIndex);
    void popPartIndent();

private:
    SourceMapTextBuilder m_sourceMapBuilder;
    QString m_partIndent;
    std::vector< std::pair<QString, QString> > m_indentationStack;
    FileLineColumnPositionList m_originPositions;
};

} // namespace intern
} // namespace Twofold

