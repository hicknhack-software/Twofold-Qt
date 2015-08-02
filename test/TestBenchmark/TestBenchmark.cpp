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
#include "Twofold/Engine.h"

#include <QObject>
#include <QString>
#include <QtTest>

class TestBenchmark : public QObject
{
    Q_OBJECT

public:
    TestBenchmark();

private Q_SLOTS:
    void prepare();
    void execute_data();
    void execute();
    void calls();
    void exceptions();

private:
    Twofold::Engine engine;
    QVariantHash context;
    Twofold::PreparedTemplate prepared;
    Twofold::Target target;
};

#include "TestBenchmark.moc"

#define TEMPLATE_REPETITION 1000

class FakeTextLoader : public Twofold::TextLoader
{
public:
    Result load(const QString &name) const
    {
        static const QString templateText1 = QString::fromLatin1(R"EXAMPLE(
for(var i = 0; i <= %1; i++) {
    | Das ist ein kurzer Text! #{i}
    # include "lib.twofold"
}
|---------------------------------
|
function structFieldHeader(type) {
  |#{ type.name }
  |struct #{ type.name } {
  |};
}

if (type.isArray) {
    | // #{ type.name } Type is an Array #{type.text}"
    = structFieldHeader(type);
}

    |class #{ name } {
     for(var i = 0; i < baseNames.length; i++) {
    |  #{ (i==0 ? ':' : ',') } public #{ baseNames[i] }
     }
|};
)EXAMPLE"
        ).arg(TEMPLATE_REPETITION);

        static const QString templateText2 = R"EXAMPLE(
|Line 1 included
|Line 2 incldued
)EXAMPLE";

        return (name != "lib.twofold")
                ? Result { Success, QString(), templateText1 }
                : Result { Success, QString(), templateText2 };
    }
};

TestBenchmark::TestBenchmark()
    : engine(std::make_shared<Twofold::MessageHandler>(),
             std::make_shared<FakeTextLoader>())
#if defined(_MSC_VER) && _MSC_VER < 1900
    , prepared({})
    , target({})
#endif
{
    QObject* pType = new QObject();
    pType->setProperty( "isArray", true );
    pType->setProperty( "name", "TestArray" );

    QStringList baseNames;
    baseNames.append( "base1" );
    baseNames.append( "base2" );
    baseNames.append( "base3" );

    context.insert( "type", QVariant::fromValue(pType) );
    context.insert( "baseNames", baseNames );
    context.insert( "name", "Main" );
}

void TestBenchmark::prepare()
{
    QBENCHMARK {
        engine.prepare("TextTemplate.twofold");
    }
}

void TestBenchmark::execute_data()
{
    prepared = engine.prepare("TextTemplate.twofold");
}

void TestBenchmark::execute()
{
    QBENCHMARK {
        engine.exec(prepared, context);
    }
}

extern int func(int x, int y);
extern int func_except(int x, int y);

int func(int x, int y) {
    return (x + y);
}

int func_except(int x, int y) {
    throw (x + y);
}

void TestBenchmark::calls()
{
    int r = 1;
    QBENCHMARK {
        r *= func(49, 430);
    }
}

void TestBenchmark::exceptions()
{
    int r = 1;
    QBENCHMARK {
        try {
            func_except(49, 430);
        }
        catch(int x) {
            r *= x;
        }
    }
}

QTEST_GUILESS_MAIN(TestBenchmark)
