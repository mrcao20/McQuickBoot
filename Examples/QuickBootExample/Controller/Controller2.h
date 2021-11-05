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
#pragma once

#include <McBoot/McBootGlobal.h>

Q_MOC_INCLUDE("Service/Service2.h")
Q_MOC_INCLUDE("Domain/RequestVo.h")

MC_FORWARD_DECL_CLASS(Service2)
MC_FORWARD_DECL_STRUCT(ReturnVo)
MC_FORWARD_DECL_STRUCT(RequestVo)

class Controller2 : public QObject
{
    Q_OBJECT
    //! 声明为一个Controller，括号里为实例化后的类名
    MC_CONTROLLER("controller2")
    //! 向IOC容器请求为属性m_service2注入一个名为service2的对象，这里的名字就是Service2类中MC_SERVICE("service2")包裹的名字。
    //! 注意，m_service2为cpp类Controller2中的属性，而Q_PROPERTY宏声明的service2为元对象中的属性名，MC_AUTOWIRED表示向IOC容器请求注入。
    //! 这里MC_AUTOWIRED只有一个参数，是因为本类元对象中的属性名和IOC容器中的对象名同名，如果不同名的话，则需要用这种形式MC_AUTOWIRED("元对象中属性名", "IOC容器中对象名")。
    //! 元对象中属性名即为Q_PROPERTY声明的名字，IOC容器中对象名为MC_SERVICE包裹的名字。
    //! 同时，如果同名的话还可以 使用MC_PROPERTY宏简化。即下面的两个代码可以用一行代替: MC_PROPERTY(Service2Ptr, service2, MEMBER m_service2)
    MC_AUTOWIRED("service2")
    Q_PROPERTY(Service2Ptr service2 MEMBER m_service2)
public:
    Q_INVOKABLE void method1();
    Q_INVOKABLE int method2(const QString &arg);
    Q_INVOKABLE ReturnVoPtr method3(const QList<RequestVoPtr> &vos);
    Q_INVOKABLE ReturnVoPtr method4(const RequestVoPtr &vo);

private:
    Service2Ptr m_service2;
};

MC_DECL_METATYPE(Controller2)
