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
#include <QTest>

#include <McIoc/ApplicationContext/Impl/McLocalPathApplicationContext.h>

#include "AnnotationApplicationContextTest.h"
#include "BeanFactoryTest.h"
#include "BeanReaderTest.h"
#include "XmlApplicationContextTest.h"
#include "XmlBuilder.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    BeanFactoryTest bft;
    int code = 0;
    code = QTest::qExec(&bft, argc, argv);
    BeanReaderTest brt;
    code |= QTest::qExec(&brt, argc, argv);
    XmlApplicationContextTest xact(McLocalPathApplicationContextPtr::create(":/iocTest.xml"), true);
    code |= QTest::qExec(&xact, argc, argv);
    AnnotationApplicationContextTest aact;
    code |= QTest::qExec(&aact, argc, argv);
    XmlApplicationContextTest xact2(XmlBuilder().build(), false);
    code |= QTest::qExec(&xact2, argc, argv);
    return code;
}
