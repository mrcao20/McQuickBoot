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
#include "TestController.h"

#include <McCore/Utils/McRequest.h>

#include "Domain/RequestVo.h"
#include "Service/TestService.h"

MC_STATIC2()
{
    mcRegisterMetaType<TestController>();
}

QString TestController::testMethod()
{
    return m_testService->testMethod();
}

void TestController::testMethod2(const McRequest &request)
{
    qDebug() << "TestController::testMethod2被调用" << request.rollAt<QString>();
}

void TestController::testMethod3(const QString &str)
{
    qDebug() << "TestController::testMethod3被调用:" << str;
}

void TestController::testMethod4(const RequestVoPtr &vo)
{
    qDebug() << "TestController::testMethod4被调用:" << vo->req;
}
