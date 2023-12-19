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

using namespace Twofold;

class Object : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool boolValue READ boolValue WRITE setBoolValue FINAL)
    Q_PROPERTY(QString stringValue READ stringValue WRITE setStringValue FINAL)
    Q_PROPERTY(int intValue READ intValue WRITE setIntValue FINAL)
public:
    bool boolValue() const { return m_boolValue; }
    int intValue() const { return m_intValue; }
    QString stringValue() const { return m_stringValue; }
    void setBoolValue(bool newBoolValue) { m_boolValue = newBoolValue; }
    void setIntValue(int newIntValue) { m_intValue = newIntValue; }
    void setStringValue(const QString &newStringValue) { m_stringValue = newStringValue; }

private:
    bool m_boolValue;
    QString m_stringValue;
    int m_intValue;
};

class TestIntegration : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testTarget_data();
    void testTarget();
    void testTargetSourceMap_data();
    void testTargetSourceMap();
};

void TestIntegration::testTarget_data()
{
    QTest::addColumn< QString >( "templateText" );
    QTest::addColumn< QString >( "targetText" );
    QTest::addColumn< QVariantHash >( "context" );
    QTest::addColumn< MessageCount >( "messageCount" );

    auto* obj = new Object();

    obj->setBoolValue(true);
    obj->setStringValue("Text");
    obj->setIntValue(1);

    QStringList qArray;
    qArray.append( "value1" );
    qArray.append( "value2" );
    qArray.append( "value3" );

    QVariantHash context;

    context.insert( "qObject", QVariant::fromValue(obj) );
    context.insert( "qArray", qArray );
    context.insert( "boolValue", true );
    context.insert( "stringValue", "Text" );
    context.insert( "intValue", 2 );

    QTest::newRow("empty") << "|" << "\n" << context << MessageCount();
    QTest::newRow("text") << " \t | \t Hello World \t " << " \t Hello World \t \n" << context << MessageCount();
    QTest::newRow("unescaped newline") << "|Hello\\nWorld" << "Hello\\nWorld\n" << context << MessageCount();
    QTest::newRow("expression") << "|1 + 2 = #{ 1 + 2 }" << "1 + 2 = 3\n" << context << MessageCount();
    QTest::newRow("escaped expression") << "|####{ 1 + 2 }" << "##{ 1 + 2 }\n" << context << MessageCount();
    QTest::newRow("newline expression") << "|  #{'Hello\\nWorld'}" << "  Hello\n  World\n" << context << MessageCount();

    QTest::newRow("line continue") << "\\Hello\n| World\n" << "Hello World\n" << context << MessageCount();
    QTest::newRow("line continue expr") << "\\  Hello\n\\ #{'World\\nNext'}\n|" << "  Hello World\n  Next\n" << context << MessageCount();

    QTest::newRow("missing close bracket") << "|1 + 2 = #{ 1 + 2 " << "1 + 2 = \n" << context << MessageCount{ 1, 1, 0 };

    QTest::newRow("context qobject") << "|#{qObject.boolValue}, #{qObject.stringValue}, #{qObject.intValue}" << "true, Text, 1\n" << context << MessageCount();
    QTest::newRow("context qArray") << "|#{qArray[0]}, #{qArray[1]}, #{qArray[2]}" << "value1, value2, value3\n" << context << MessageCount();
    QTest::newRow("context primitive") << "|#{boolValue}, #{stringValue}, #{intValue}" << "true, Text, 2\n" << context << MessageCount();

    QTest::newRow("include") << "# include \"include.twofold\"" << "  Include Text\n" << context << MessageCount();
    QTest::newRow("include not found") << "# include \"notFound.twofold\"" << "" << context << MessageCount{ 1, 1, 0 };
    QTest::newRow("include error loading") << "# include \"errorLoading.twofold\"" << "" << context << MessageCount{ 1, 1, 0 };
    QTest::newRow("include recursive") << "# include \"recursive.twofold\"" << "" << context << MessageCount{ 1, 1, 0 };

    const QString callTemplate = R"EXAMPLE(
            function min(a, b) {
            | #{Math.min(a, b)}
}
            = min(qObject.intValue, intValue);
    )EXAMPLE";
            QTest::newRow("call") << callTemplate << "  1\n" << context << MessageCount();

    const QString multiline_expression = R"EXAMPLE(
            function called() {
            |#{'hello\nworld'}
}
            | simple
            = called();
    )EXAMPLE";
            QTest::newRow("multiline expression") << multiline_expression << " simple\n hello\n world\n" << context << MessageCount();

    const QString complex_callers = R"EXAMPLE(
            function a(p) {
            \a#{p}
}
            function b(p) {
            |#{a(p)}
            |b#{p}
}
            = b(1);
    |c
            = b(2);
    )EXAMPLE";
            QTest::newRow("complex callers") << complex_callers << " a1\n b1\nc\n a2\n b2\n" << context << MessageCount();

    const QString bug_no1 = R"EXAMPLE(
            function a(cb) {
            for(var i=0; i<2; i++) {
        | #{cb()}
    }
}
        = a(function() {
                return "CB"; })

)EXAMPLE";
QTest::newRow("bug_no1") << bug_no1 << " CB\n CB\n" << QVariantHash() << MessageCount{1,1,0};
}

void TestIntegration::testTarget()
{
    QFETCH( QString, templateText );
    QFETCH( QString, targetText );
    QFETCH( QVariantHash, context );
    QFETCH( MessageCount, messageCount );

    auto messageHandler = std::make_shared<FakeMessageHandler>();
    auto textLoader = std::make_shared<FakeTextLoader>(templateText);

    Engine engine(messageHandler, textLoader);
    Target target = engine.execTemplateName("testTemplate", context);

    if (messageHandler->count.message != messageCount.message) {
        messageHandler->display();
    }
    QCOMPARE(messageHandler->count.message, messageCount.message);
    QCOMPARE(messageHandler->count.templateMessage, messageCount.templateMessage);
    QCOMPARE(messageHandler->count.javaScriptMessage, messageCount.javaScriptMessage);
    QCOMPARE(target.text, targetText);
}

void TestIntegration::testTargetSourceMap_data()
{
    QTest::addColumn< QString >( "templateText" );
    QTest::addColumn< QString >( "searchText" );
    QTest::addColumn< int >( "templateLine" );
    QTest::addColumn< int >( "templateColumn" );
    QTest::addColumn< size_t >( "callerCount" );

    // callerCount must be 0 because SourceMapTextBuilder redirects parent index to avoid duplicated
    // callers in the callstack.
    //                                                1         2
    //                                       123456789012345678901234567
    QTest::newRow("inline expression 1") << "| simple #{(1 == 2)? 'Really?' : 'Correct'}" << "Correct" << 1 << 12 << size_t(0);

    //                                                           1
    //                                       123456789  12345678901
    QTest::newRow("inline expression 2") << "| simple \n|#{'hello'}" << "ello" << 2 << 4 << size_t(0);

    const QString complex_callers = R"EXAMPLE(
            function txta(p) {
            \txta#{p}
}
            function txtb(p) {
            |#{txta(p)}
            |txtb#{p}
}
            = txtb(1);
    |c
            = txtb(2);
    )EXAMPLE";
    QTest::newRow("complex callers 1") << complex_callers << "txtb2" << 7 << 14 << size_t(1);
    QTest::newRow("complex callers 2") << complex_callers << "txta1" << 3 << 14 << size_t(2);
}

void TestIntegration::testTargetSourceMap()
{
    QFETCH(QString, templateText);
    QFETCH(QString, searchText);
    QFETCH(int, templateLine);
    QFETCH(int, templateColumn);
    QFETCH(size_t, callerCount);

    using namespace Twofold;

    Engine engine(std::make_shared<FakeMessageHandler>(),
                  std::make_shared<FakeTextLoader>(templateText));
    QVariantHash context;
    Target target = engine.execTemplateName("testTemplate", context);

    auto targetBegin = target.text.cbegin();
    auto targetEnd = target.text.cend();

    auto it = std::search(targetBegin, targetEnd, searchText.cbegin(), searchText.cend());
    QVERIFY(it!=targetEnd);
    auto lastNewLine = intern::find_last(targetBegin, it, QChar('\n'));
    if (*lastNewLine == '\n') lastNewLine++;

    int targetColumn = std::distance(lastNewLine, it) + 1;
    int targetLine = std::count(targetBegin, it, QChar('\n')) + 1;

    auto source = target.sourceMap.findEntryByGenerated({targetLine, targetColumn});

    QCOMPARE(source->original.line, templateLine);
    QCOMPARE(source->original.column, templateColumn);

    auto callers = SourceMap::buildCallerStack(target.sourceMap.data(), source);
    QCOMPARE(callers.size(), callerCount);
}

// QTEST_APPLESS_MAIN not possible, because we must
// construct a Q(Core)Application before a QScriptEngine
QTEST_GUILESS_MAIN(TestIntegration)

#include "TestIntegration.moc"
