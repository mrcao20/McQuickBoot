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

#include <QObject>

#include <McCore/McGlobal.h>

struct GadgetData
{
    Q_GADGET
public:
    MC_POCO_PROPERTY(QString, text, {"gadgetData"})
    MC_POCO_PROPERTY(QSharedPointer<GadgetData>, child)
};

MC_DECL_METATYPE(GadgetData)

class ObjectData : public QObject
{
    Q_OBJECT
    MC_JSON_SERIALIZATION()
public:
    MC_POCO_PROPERTY(QString, text, {"objectData"})
    MC_POCO_PROPERTY(QList<GadgetDataPtr>, gadgets)
};

MC_DECL_METATYPE(ObjectData)

class TestCore : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void coreCase();
    void jsonCase();
    void pathPlaceholderCase();
    void eventDispatcherCase();
    void metaTypeCase();
    void loadPluginCase();
    void loadLibraryCase();
    void loadMemoryLibraryCase();
    void callbackCase();
};
