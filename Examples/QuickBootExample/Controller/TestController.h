/*
 * Copyright (c) 2021 mrcao20/mrcao20@163.com
 * McQuickBoot is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 */
#pragma once

#include <McQuickBoot/McQuickBootMacroGlobal.h>

#ifdef MC_USE_QT6
Q_MOC_INCLUDE("Service/TestService.h")
Q_MOC_INCLUDE("Domain/RequestVo.h")
#endif

class McRequest;

MC_FORWARD_DECL_CLASS(TestService)
MC_FORWARD_DECL_STRUCT(RequestVo)

class TestController : public QObject
{
    Q_OBJECT
    //! 声明为一个Controller，括号里为实例化后的类名
    MC_CONTROLLER("testController")
    //! 向IOC容器请求为属性m_testService注入一个名为testService的对象，这里的名字就是TestService类中MC_SERVICE("testService")包裹的名字。
    //! 注意，m_testService为cpp类TestController中的属性，而Q_PROPERTY宏声明的testService为元对象中的属性名，MC_AUTOWIRED表示向IOC容器请求注入。
    //! 这里MC_AUTOWIRED只有一个参数，是因为本类元对象中的属性名和IOC容器中的对象名同名，如果不同名的话，则需要用这种形式：
    //! MC_AUTOWIRED("元对象中属性名", "IOC容器中对象名")。
    //! 元对象中属性名即为Q_PROPERTY声明的名字，IOC容器中对象名为MC_SERVICE包裹的名字。 同时，如果同名的话还可以
    //! 使用MC_PROPERTY宏简化。即下面的两个代码可以用一行代替：MC_PROPERTY(TestServicePtr, testService, MEMBER m_testService)
    MC_AUTOWIRED("testService")
    Q_PROPERTY(TestServicePtr testService MEMBER m_testService)
public:
    Q_INVOKABLE QString testMethod();
    Q_INVOKABLE void testMethod2(const McRequest &request);
    Q_INVOKABLE void testMethod3(const QString &str);
    Q_INVOKABLE void testMethod4(const RequestVoPtr &vo);

private:
    TestServicePtr m_testService;
};

MC_DECL_METATYPE(TestController)
