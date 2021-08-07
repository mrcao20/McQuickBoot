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

#include <QHash>
#include <QSharedPointer>

#include "../../McMacroGlobal.h"

struct McPointerMetaId
{
    int qobjectPointerId{-1};
    int sharedPointerId{-1};
};
MC_DECL_POINTER(McPointerMetaId)

struct McGadgetMetaId
{
    int gadgetId{-1};
    int pointerId{-1};
    int sharedId{-1};
};
MC_DECL_POINTER(McGadgetMetaId)

struct McSequentialMetaId
{
    int id{-1};
    int valueId{-1};
};
MC_DECL_POINTER(McSequentialMetaId)

struct McAssociativeMetaId
{
    int id{-1};
    int keyId{-1};
    int valueId{-1};
};
MC_DECL_POINTER(McAssociativeMetaId)

typedef QHash<int, McPointerMetaIdPtr> McPointerType;
typedef QHash<int, McGadgetMetaIdPtr> McGadgetType;
typedef QHash<int, McSequentialMetaIdPtr> McSequentialType;
typedef QHash<int, McAssociativeMetaIdPtr> McAssociativeType;
typedef QHash<int, QList<int>> McMetaIdMapType;
//! beanName应该用QByteArray保存
//! <sharedId, 某个共享指针类型对应的beanName>
typedef QHash<int, QStringList> McBeanNameMapType;

class MCIOC_EXPORT McMetaTypeId
{
public:
    static McPointerType qobjectPointerIds() noexcept;
    static void addQObjectPointerIds(int id, int sharedId) noexcept;
    static int getSharedForPointer(int id) noexcept;

    static McPointerType sharedPointerIds() noexcept;
    static void addSharedPointerId(int id, int qobjectId) noexcept;
    static int getPointerForShared(int id) noexcept;

    static McGadgetType gadgetIds() noexcept;
    static void addGadget(int gId, int pId, int sId) noexcept;

    static McSequentialType sequentialIds() noexcept;
    static void addSequentialId(int id, int valueId) noexcept;
    static int getSequentialValueId(int id) noexcept;

    static McAssociativeType associativeIds() noexcept;
    static void addAssociativeId(int id, int keyId, int valueId) noexcept;

    static McMetaIdMapType metaIdMapType() noexcept;
    static void addMetaIdMap(int mainMetaId, int dstMetaId) noexcept;
    static QList<int> getDstMetaIds(int id) noexcept;

    static McBeanNameMapType beanNameMapType() noexcept;
    static void addBeanNameMap(int id, const QString &beanName) noexcept;
    static void addBeanNameMap(const QList<int> &ids, const QString &beanName) noexcept;
    static QStringList getBeanNamesForId(int id) noexcept;
};
