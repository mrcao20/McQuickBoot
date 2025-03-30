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

#include <QJsonObject>

#include <McCore/McGlobal.h>

MC_FORWARD_DECL_CLASS(IMcApplicationContext)

class XmlApplicationContextTest : public QObject
{
    Q_OBJECT
public:
    XmlApplicationContextTest(const IMcApplicationContextPtr &appCtx, bool flag);

private Q_SLOTS:
    void customCase();
    void podCase();
    void gadgetCase();
    void containerCase();
    void objectCase();
    void pluginCase();

private:
    IMcApplicationContextPtr m_appCtx;
    bool m_flag{true};
};

MC_DECL_METATYPE(QJsonObject)
