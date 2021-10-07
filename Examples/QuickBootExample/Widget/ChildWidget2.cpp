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
#include "ChildWidget2.h"

#include <QDebug>
#include <QPushButton>
#include <QThread>

#include <McBoot/McBootGlobal.h>
#include <McWidgetIoc/McWidgetGlobal.h>

#include "Domain/RequestVo.h"
#include "Domain/ReturnVo.h"

MC_STATIC()
qRegisterMetaType<ChildWidget2 *>();
MC_STATIC_END

ChildWidget2::ChildWidget2(QWidget *parent) : QWidget(parent)
{
    auto button = new QPushButton("ChildWidget2", this);
    connect(button, &QAbstractButton::clicked, []() {
        qDebug() << QStringLiteral(u"在这里面调用业务方法，界面调用线程：") << QThread::currentThread();
        //! invoke方法的第一个参数有两个字符串，由.分割，第一个字符串为MC_CONTROLLER宏包裹的字符串，标识调用某一个具体的Controller对象。
        //! 第二个字符串为该对象中的函数名，该函数必须被Q_INVOKABLE宏标记。
        $.invoke("controller2.method1").then([]() {
            qDebug() << QStringLiteral(u"method1调用完成，回归线程：") << QThread::currentThread();
        });
        //! invoke方法从第二个参数开始到最后一个参数，都会打包传递给最终的调用方法method2。then方法接受的lambda表达式可以指定一个参数，用来接收method2的返回值
        $.invoke("controller2.method2", QStringLiteral(u"第二个业务功能的参数")).then([](int ret) {
            qDebug() << QStringLiteral(u"method2调用完成，其返回值：") << ret;
        });
        QList<RequestVoPtr> reqs;
        RequestVoPtr req = RequestVoPtr::create();
        req->req = QStringLiteral(u"method3请求参数");
        reqs.append(req);
        $.invoke("controller2.method3", reqs).then([](const ReturnVoPtr ret) {
            qDebug() << QStringLiteral(u"method3调用完成，返回值：") << ret->ret;
        });
    });
}
