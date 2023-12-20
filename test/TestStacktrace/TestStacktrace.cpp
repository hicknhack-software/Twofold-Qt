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
#include "FakeMessageHandler.h"
#include "FakeTextLoader.h"

#include "Twofold/Engine.h"

#include "Twofold/intern/find_last.h"

#include <QtTest>

#include <algorithm>

using namespace SourceMap;
using namespace Twofold;

class TestStacktrace : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testSyntaxError();
    void testErrorObject();
    void testException();
};

namespace std {
template<typename T>
char *toString(const std::vector<T> &value)
{
    using namespace Qt::StringLiterals;
    QString inner;
    for(qsizetype i{0}; const T& element: value) {
        if (i++ > 0) inner += u", "_s;
        std::unique_ptr<char> data(::toString(element));
        inner.append(data.get());
    }
    auto frame = u"std::vector{%1}"_s.arg(inner);
    return qstrdup(frame.toLatin1().data());
}
}

namespace SourceMap {
char *toString(const FilePosition &value)
{
    using namespace Qt::StringLiterals;
    auto text = u"FilePosition{%1:%2:%3}"_s.arg(value.name).arg(value.line).arg(value.column);
    return qstrdup(text.toLatin1().data());
}
}

namespace Twofold {
char *toString(const BacktraceFilePosition &value)
{
    using namespace Qt::StringLiterals;
    std::unique_ptr<char[]> fileposition(::toString(static_cast<const SourceMap::FilePosition&>(value)));
    auto text = u"BacktraceFilePosition{%1:%2}"_s.arg(value.functionName, fileposition.get());
    return qstrdup(text.toLatin1().data());
}
}

void TestStacktrace::testSyntaxError()
{
    using namespace Qt::StringLiterals;
    static const QString templateText = QString::fromLatin1(R"EXAMPLE(
let innerFunc = (text) => {
    return `Hello, ${text}
}
let outerFunc = (text) => {
    return innerFunc(text)
}
|#{outerFunc('Stacktrace')}
)EXAMPLE");

    auto messageHandler = std::make_shared<FakeMessageHandler>();
    auto textLoader = std::make_shared<FakeTextLoader>();
    textLoader->addSource("testTemplate", templateText);

    Engine engine(messageHandler, textLoader);
    QVariantHash inputs;
    Target target = engine.execTemplateName("testTemplate", inputs);
    Q_UNUSED(target);

    QCOMPARE(messageHandler->messageStore.size(), 0);
    QCOMPARE(messageHandler->templateStore.size(), 0);
    QCOMPARE(messageHandler->javascriptStore.size(), 1);
    auto expected = std::tuple{
        MessageType::Error,
        BacktraceFilePositionList{BacktraceFilePosition("<unknown function>", FilePosition("testTemplate", Position(3, 26)))},
        u"SyntaxError: Expected token `(template tail)'"_s
    };
    QCOMPARE(messageHandler->javascriptStore.front(), expected);
}

void TestStacktrace::testErrorObject()
{
    using namespace Qt::StringLiterals;
    static const QString templateText = QString::fromLatin1(R"EXAMPLE(
let innerFunc = (text) => {
    throw new Error('There was a problem')
    return `Hello, ${text}`
}
let outerFunc = (text) => {
    return innerFunc(text)
}
|#{outerFunc('Stacktrace')}
)EXAMPLE");

    auto messageHandler = std::make_shared<FakeMessageHandler>();
    auto textLoader = std::make_shared<FakeTextLoader>();
    textLoader->addSource("testTemplate", templateText);

    Engine engine(messageHandler, textLoader);
    QVariantHash inputs;
    Target target = engine.execTemplateName("testTemplate", inputs);
    Q_UNUSED(target);

    QCOMPARE(messageHandler->messageStore.size(), 0);
    QCOMPARE(messageHandler->templateStore.size(), 0);
    QCOMPARE(messageHandler->javascriptStore.size(), 1);
    auto expected = std::tuple{
        MessageType::Error,
        BacktraceFilePositionList{
            BacktraceFilePosition("innerFunc", FilePosition("testTemplate", Position(3, 1))),
            BacktraceFilePosition("outerFunc", FilePosition("testTemplate", Position(7, 1))),
            BacktraceFilePosition("%entry", FilePosition("testTemplate", Position(9, 2)))},
        u"Error: There was a problem"_s};
    QCOMPARE(messageHandler->javascriptStore.front(), expected);
}

void TestStacktrace::testException()
{
    using namespace Qt::StringLiterals;
    static const QString templateText = QString::fromLatin1(R"EXAMPLE(
let innerFunc = (text) => {
    throw 'Custom non Error object'
    return `Hello, ${text}`
}
let outerFunc = (text) => {
    return innerFunc(text)
}
|#{outerFunc('Stacktrace')}
)EXAMPLE");

    auto messageHandler = std::make_shared<FakeMessageHandler>();
    auto textLoader = std::make_shared<FakeTextLoader>();
    textLoader->addSource("testTemplate", templateText);

    Engine engine(messageHandler, textLoader);
    QVariantHash inputs;
    Target target = engine.execTemplateName("testTemplate", inputs);
    Q_UNUSED(target);

    QCOMPARE(messageHandler->messageStore.size(), 0);
    QCOMPARE(messageHandler->templateStore.size(), 0);
    QCOMPARE(messageHandler->javascriptStore.size(), 1);
    auto expected = std::tuple{
                               MessageType::Error,
                               BacktraceFilePositionList{
                                                         BacktraceFilePosition("innerFunc", FilePosition("testTemplate", Position(3, 1))),
                                                         BacktraceFilePosition("outerFunc", FilePosition("testTemplate", Position(7, 1))),
                                                         BacktraceFilePosition("%entry", FilePosition("testTemplate", Position(9, 2)))},
                               u"Exception: Custom non Error object"_s};
    QCOMPARE(messageHandler->javascriptStore.front(), expected);
}

// QTEST_APPLESS_MAIN not possible, because we must
// construct a Q(Core)Application before a QJSEngine
QTEST_GUILESS_MAIN(TestStacktrace)

#include "TestStacktrace.moc"
