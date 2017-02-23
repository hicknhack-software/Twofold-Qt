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

#include <QCoreApplication>
#include <QtDebug>

const QString included_twofold = R"TWOFOLD(
function methodArgs(args) {
  args.forEach(function(arg, i){
    if (0!=i) {
        // use interpolation to preserve the whitespace
        \#{', '}
    }
    // use backslash to avoid linebreaks
        \#{arg}
  });
}
function showMethod(method) {
        |function #{method.name}(#{methodArgs(method.args)}) {
  // if body contains multiple lines they will all be indented
        |  #{method.body}
        |}
}
)TWOFOLD";

static const QString main_twofold = R"TWOFOLD(
#include "included.twofold"

        |function #{name}Class(#{methodArgs(args)}) {
methods.forEach(function(method){
  // every line outputted by showMethod is indented by two extra spaces
        =  showMethod(method)
});
        |
        |  return {
methods.forEach(function(method, i){
        |    "#{method.name}": #{method.name}#{(i+1 < methods.length) ? ',' : ''}
});
        |  };
        |}
)TWOFOLD";

class FakeTextLoader : public Twofold::TextLoader
{
public:
    Result load(const QString &name) const
    {
        if (name == "included.twofold") {
            return { Success, name, included_twofold };
        }
        if (name == "main.twofold") {
          return { Success, name, main_twofold };
        }
        return { NotFound, {}, {} };
    }
};

class MethodDescription : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QStringList args READ args)

public:
    MethodDescription(const QString name, const QStringList args, QObject* parent = 0)
        : QObject(parent)
        , m_name(name)
        , m_args(args)
    {}

    QString name() const
    {
        return m_name;
    }

    QStringList args() const
    {
        return m_args;
    }

private:
    QString m_name;
    QStringList m_args;
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    Q_UNUSED(app); // required to get QScriptEngine

    using namespace Twofold;

    Engine engine(std::make_shared<MessageHandler>(),
                  std::make_shared<FakeTextLoader>());
    PreparedTemplate prepared = engine.prepare("main.twofold");

    ::qDebug() << "generated javascript:";
    ::qDebug() << prepared.javascript;
    ::qDebug() << "-----";

    MethodDescription helloMethod("hello", {"greeted"});
    // Use QObject dynamic properties
    helloMethod.setProperty("body", "console.log('Hello' + greeted);");

    QVariantHash context; // these are the globals in the javascript execution
    context.insert( "name", "TwofoldGenerated" );
    context.insert( "args", QStringList() );
    context.insert( "methods", QVariantList() << QVariant::fromValue(&helloMethod) );

    Target target = engine.exec(prepared, context);

    ::qDebug() << "generated sourcecode:";
    ::qDebug() << target.text;
    ::qDebug() << "-----";
    ::qDebug() << "done";
}

#include "main.moc"
