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
#include "MetaTypeRegister.h"

#include <QDebug>
#include <QList>
#include <QMap>

MC_AUTO_INIT2(IRegisterTest)
{
    qDebug() << "test MC_AUTO_INIT2";
}

MC_AUTO_INIT(RegisterTest, Mc::RoutinePriority::RoutineNormal - 1)
qDebug() << "static block low level";
mcRegisterContainer<QList<RegisterTestPtr>>();
mcRegisterContainer<QMap<int, RegisterTestPtr>>();
MC_DESTROY()
qDebug() << "RegisterTest destroy";
MC_INIT_END

MC_STATIC2()
{
    qDebug() << "test MC_STATIC2";
}

MC_DESTROY2()
{
    qDebug() << "test MC_DESTROY2";
}
