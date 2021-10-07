/*
 * MIT License
 *
 * Copyright (c) 2021 mrcao20
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <QApplication>
#include <QDebug>

#include <McBoot/McQuickBoot.h>
#include <McBoot/McQuickBootSimple.h>

int main(int argc, char *argv[])
{
#if 0
    McQuickBoot::setPreInitFunc([](QCoreApplication *) {
        qDebug() << QStringLiteral(u"此代码将在QGuiApplication构造完成后，框架初始化之前调用");
    });
    McQuickBoot::setAfterInitFunc([](QCoreApplication *, QQmlApplicationEngine *) {
        qDebug() << QStringLiteral(u"此代码将在QGuiApplication构造完成后并且框架初始化之后调用");
    });
    return McQuickBoot::run<QApplication>(argc, argv, QLatin1String("qrc:/main.qml"));
#else
    QApplication app(argc, argv);
    McQuickBootSimple::init();
    return app.exec();
#endif
}
