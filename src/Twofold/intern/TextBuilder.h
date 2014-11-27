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
#ifndef TWOFOLD_INTERN_TEXTBUILDER_H
#define TWOFOLD_INTERN_TEXTBUILDER_H

#include <QString>

namespace Twofold {
namespace intern {

/// @brief empty structure marking new lines
struct NewLine {};

/// @brief text between two iterators
struct TextSpan {
    inline TextSpan(QString::const_iterator begin, QString::const_iterator end)
        : begin(begin), end(end) {}

    /**
     * @brief construct from a QString
     * @param text has to live at least as long as the TextSpan
     */
    inline TextSpan(const QString& text) : begin(text.begin()), end(text.end()) {}

    QString::const_iterator begin, end;
};

using Text = QString;

/**
 * @brief constructs correctly indented text
 *
 * builder with state accessors
 */
class TextBuilder
{
public:
    /// retrieve the build text
    Text build() const;

    /// returns the current indentation
    const QString& indentation() const;

    /// returns the current line
    int line() const;

    /// returns the current column
    int column() const;

    /// returns true, if no text is on the current line
    bool isBlankLine() const;

    /// change the indentation for the next line
    void setIndentation(const QString &indent);

    /// add part of a line
    TextBuilder& operator<<(const TextSpan& textSpan);

    /// explicitly add a line break
    TextBuilder& operator<<(NewLine);

private:
    QString m_buffer;
    QString m_indentation;
    int m_line = 1;
    int m_column = 0;
};

} // namespace intern
} // namespace Twofold

#endif // TWOFOLD_INTERN_TEXTBUILDER_H
