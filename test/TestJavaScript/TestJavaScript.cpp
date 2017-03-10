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

#include "Twofold/PreparedTemplateBuilder.h"

#include "Twofold/intern/find_last.h"

#include <QtTest>

class TestJavaScript : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testInterpolation_data();
    void testInterpolation();
    void testSourceMap_data();
    void testSourceMap();
};

#define FAKE_TWOFOLD_NAME "testTemplate"

void TestJavaScript::testInterpolation_data()
{
    QTest::addColumn< QString >( "templateText" );
    QTest::addColumn< QString >( "javaScriptText" );

    const static QString s_newLine("_template.newLine(%1);\n");
    const static QString s_output("_template.append(\"%1\", %2);\n");
    const static QString s_indentPart("_template.indentPart(\"%1\", %2);\n");
    const static QString s_expression("_template.pushPartIndent(%2);_template.appendExpression(%1, %2);_template.popPartIndent();\n");

    static auto newLine = [] (int originIndex) { return s_newLine.arg(originIndex); };
    static auto output = [](QString text, int originIndex){ return s_output.arg(text).arg(originIndex); };
    static auto indentPart = [](QString text, int originIndex){ return s_indentPart.arg(text).arg(originIndex); };
    static auto expression = [](QString text, int originIndex){ return s_expression.arg(text).arg(originIndex); };

    QTest::newRow("empty") << "|" << indentPart("", 0) + newLine(1);
    QTest::newRow("simple") << "|simple " << indentPart("", 0) + output("simple ", 1) + newLine(2);
    QTest::newRow("indentation space") << "\t| simple" << indentPart(" ", 0) + output("simple", 1) + newLine(2);
    QTest::newRow("indentation tab") << " |\tsimple" << indentPart("\t", 0) + output("simple", 1) + newLine(2);

    QTest::newRow("no expr") << "|#(test" << indentPart("", 0) + output("#(test", 1) + newLine(2);
    QTest::newRow("escaped expr") << "|##{test" << indentPart("", 0) + output("#", 1) + output("{test", 2) + newLine(3);
    QTest::newRow("single expr 1") << "|#{test}" << indentPart("", 0) + expression("test", 1) + newLine(2);
    QTest::newRow("single expr 2") << "|#{test}}" << indentPart("", 0) + expression("test", 1) + output("}", 2) + newLine(3);
    QTest::newRow("inline expr 1") << "|simple #{test.prop} expr" << indentPart("", 0) + output("simple ", 1) + expression("test.prop", 2) + output(" expr", 3) + newLine(4);
    QTest::newRow("logic expr") << "|#{ (i==0 ? ':' : \",\") }" << indentPart("", 0) + expression(" (i==0 ? ':' : \",\") ", 1) + newLine(2);
    QTest::newRow("escaped expr 1") << "|escape ##{test} expr" << indentPart("", 0) + output("escape #", 1) + output("{test} expr", 2) + newLine(3);
    QTest::newRow("escaped expr 2") << "|###{test}" << indentPart("", 0) + output("#", 1) + expression("test", 2) + newLine(3);
    QTest::newRow("string expr 1") << "|#{\"}\"}" << indentPart("", 0) + expression("\"}\"", 1) + newLine(2);
    QTest::newRow("string expr 2") << "|#{'\\'\\\"}'}" << indentPart("", 0) + expression("'\\'\\\"}'", 1) + newLine(2);
    QTest::newRow("nested expr") << "|#{'#{hello}'}" << indentPart("", 0) + expression("'#{hello}'", 1) + newLine(2);
}

void TestJavaScript::testInterpolation()
{
    QFETCH( QString, templateText );
    QFETCH( QString, javaScriptText );

    using namespace Twofold;

    auto messageHandler = std::make_shared<FakeMessageHandler>();
    auto textLoader = std::make_shared<FakeTextLoader>(templateText);

    PreparedTemplateBuilder builder(messageHandler, textLoader);

    const PreparedTemplate result = builder.build(FAKE_TWOFOLD_NAME);

    QCOMPARE(messageHandler->count.message, 0);
    QCOMPARE(messageHandler->count.templateMessage, 0);
    QCOMPARE(messageHandler->count.javaScriptMessage, 0);

    QCOMPARE(result.javascript, javaScriptText);
}

void TestJavaScript::testSourceMap_data()
{
    QTest::addColumn< QString >( "templateText" );
    QTest::addColumn< QString >( "searchText" );
    QTest::addColumn< int >( "templateLine" );
    QTest::addColumn< int >( "templateColumn" );

    //                                                1         2
    //                                       123456789012345678901234567
    QTest::newRow("inline expression 1") << "if(test) { return 1 : 2 }" << "1 : 2" << 1 << 19;

    //                                                1         2
    //                                       123456789012345678901234567
    QTest::newRow("inline expression 1") << "| simple #{(test)? 1 : 2 )}" << "1 : 2" << 1 << 20;

    //                                                 1         2
    //                                       123456789 012345678901
    QTest::newRow("inline expression 2") << "| simple \n|#{'hello'}" << "hello" << 2 << 5;

    //                                              1         2
    //                                    123456789 012345678901
    QTest::newRow("generated code 1") << "| simple \n|#{'hello'}" << "pendExpression('hello" << 2 << 2;
    //                                             1
    //                                    12345678901
    QTest::newRow("generated code 2") << "|#{'hello'}" << ", 1);_template.pop" << 1 << 11;
}

void TestJavaScript::testSourceMap()
{
    QFETCH(QString, templateText);
    QFETCH(QString, searchText);
    QFETCH(int, templateLine);
    QFETCH(int, templateColumn);

    using namespace Twofold;

    PreparedTemplateBuilder preparedBuilder(std::make_shared<FakeMessageHandler>(),
                                            std::make_shared<FakeTextLoader>(templateText));
    const PreparedTemplate result = preparedBuilder.build("testTemplate");

    auto jsBegin = result.javascript.cbegin();
    auto jsEnd = result.javascript.cend();

    auto it = std::search(jsBegin, jsEnd, searchText.cbegin(), searchText.cend());
    QVERIFY(it!=jsEnd);
    auto lastNewLine = intern::find_last(jsBegin, it, QChar('\n'));
    if (*lastNewLine == '\n') lastNewLine++;

    int generatedColumn = std::distance(lastNewLine, it) + 1;
    int generatedLine = std::count(jsBegin, it, QChar('\n')) + 1;

    auto source = SourceMap::getOriginalPositionFromGenerated(result.sourceMap, {generatedLine, generatedColumn});

    QCOMPARE(source.line, templateLine);
    QCOMPARE(source.column, templateColumn);
}

QTEST_APPLESS_MAIN(TestJavaScript)

#include "TestJavaScript.moc"
