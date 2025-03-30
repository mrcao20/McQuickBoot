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

#include <McCore/McGlobal.h>

class IRegisterTest
{
    MC_DECL_INIT(IRegisterTest)
public:
    MC_BASE_DESTRUCTOR(IRegisterTest)
};

MC_DECL_METATYPE(IRegisterTest)

class RegisterTest : public QObject, public IRegisterTest
{
    Q_OBJECT
    MC_FULL_DEFINE(RegisterTest, QObject, IRegisterTest)
    Q_PROPERTY(QString text READ text WRITE setText)
public:
    QString text() { return m_text; }
    void setText(const QString &val) { m_text = val; }

private:
    QString m_text{"registerTest"};
};

MC_DECL_METATYPE(RegisterTest)
