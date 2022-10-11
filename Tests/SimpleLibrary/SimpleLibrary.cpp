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
#include <QDebug>

#include <McCore/McGlobal.h>

#ifdef Q_CC_MINGW
static int i = []() {
    qDebug() << "test load ioc";
    return 1;
}();
#else
MC_STATIC()
qDebug() << "test load ioc";
MC_DESTROY()
qDebug() << "test load ioc destory";
MC_STATIC_END
#endif

extern "C" Q_DECL_EXPORT void testLoadFunc()
{
    qDebug() << "testLoadFunc";
}
