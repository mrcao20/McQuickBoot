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
#include "McIoc/BeanFactory/impl/McMetaTypeId.h"

#include <QGlobalStatic>

MC_GLOBAL_STATIC(McPointerType, mcQObjectPointerIds)
MC_GLOBAL_STATIC(McPointerType, mcSharedPointerIds)
MC_GLOBAL_STATIC(McGadgetType, mcGadgetIds)
MC_GLOBAL_STATIC(McSequentialType, mcSequentialIds)
MC_GLOBAL_STATIC(McAssociativeType, mcAssociativeIds)
MC_GLOBAL_STATIC(McMetaIdMapType, mcMetaIdMap)
MC_GLOBAL_STATIC(McBeanNameMapType, mcBeanNameMap)

McPointerType McMetaTypeId::qobjectPointerIds() noexcept
{
    return *mcQObjectPointerIds;
}

void McMetaTypeId::addQObjectPointerIds(int id, int sharedId) noexcept
{
    McPointerType *ids = mcQObjectPointerIds;
    if(ids->contains(id)) {
        return;
    }
    McPointerMetaIdPtr pId = McPointerMetaIdPtr::create();
    pId->qobjectPointerId = id;
    pId->sharedPointerId = sharedId;
    ids->insert(id, pId);
}

int McMetaTypeId::getSharedForPointer(int id) noexcept
{
    McPointerType *ids = mcQObjectPointerIds;
    if (!ids->contains(id)) {
        return -1;
    }
    return ids->value(id)->sharedPointerId;
}

McPointerType McMetaTypeId::sharedPointerIds() noexcept
{
    return *mcSharedPointerIds;
}

void McMetaTypeId::addSharedPointerId(int id, int qobjectId) noexcept
{
    McPointerType *ids = mcSharedPointerIds;
    if(ids->contains(id)) {
        return;
    }
    McPointerMetaIdPtr pId = McPointerMetaIdPtr::create();
    pId->sharedPointerId = id;
    pId->qobjectPointerId = qobjectId;
    ids->insert(id, pId);
}

int McMetaTypeId::getPointerForShared(int id) noexcept
{
    McPointerType *ids = mcSharedPointerIds;
    if (!ids->contains(id)) {
        return -1;
    }
    return ids->value(id)->qobjectPointerId;
}

McGadgetType McMetaTypeId::gadgetIds() noexcept
{
    return *mcGadgetIds;
}

void McMetaTypeId::addGadget(int gId, int pId, int sId) noexcept
{
    McGadgetType *ids = mcGadgetIds;
    if (ids->contains(pId)) {
        return;
    }
    McGadgetMetaIdPtr id = McGadgetMetaIdPtr::create();
    id->gadgetId = gId;
    id->pointerId = pId;
    id->sharedId = sId;
    ids->insert(gId, id);
}

McSequentialType McMetaTypeId::sequentialIds() noexcept
{
    return *mcSequentialIds;
}

void McMetaTypeId::addSequentialId(int id, int valueId) noexcept
{
    McSequentialType *ids = mcSequentialIds;
    if(ids->contains(id)) {
        return;
    }
    auto seqId = McSequentialMetaIdPtr::create();
    seqId->id = id;
    seqId->valueId = valueId;
    ids->insert(id, seqId);
}

int McMetaTypeId::getSequentialValueId(int id) noexcept
{
    auto ids = sequentialIds();
    if (!ids.contains(id)) {
        return -1;
    }
    return ids.value(id)->valueId;
}

McAssociativeType McMetaTypeId::associativeIds() noexcept
{
    return *mcAssociativeIds;
}

void McMetaTypeId::addAssociativeId(int id, int keyId, int valueId) noexcept
{
    McAssociativeType *ids = mcAssociativeIds;
    if(ids->contains(id)) {
        return;
    }
    auto assId = McAssociativeMetaIdPtr::create();
    assId->id = id;
    assId->keyId = keyId;
    assId->valueId = valueId;
    ids->insert(id, assId);
}

McMetaIdMapType McMetaTypeId::metaIdMapType() noexcept
{
    return (*mcMetaIdMap);
}

void McMetaTypeId::addMetaIdMap(int mainMetaId, int dstMetaId) noexcept
{
    (*mcMetaIdMap)[mainMetaId].append(dstMetaId);
}

QList<int> McMetaTypeId::getDstMetaIds(int id) noexcept
{
    return mcMetaIdMap->value(id);
}

McBeanNameMapType McMetaTypeId::beanNameMapType() noexcept
{
    return (*mcBeanNameMap);
}

void McMetaTypeId::addBeanNameMap(int id, const QString &beanName) noexcept
{
    (*mcBeanNameMap)[id].append(beanName);
}

void McMetaTypeId::addBeanNameMap(const QList<int> &ids, const QString &beanName) noexcept
{
    for (auto id : ids) {
        addBeanNameMap(id, beanName);
    }
}

QStringList McMetaTypeId::getBeanNamesForId(int id) noexcept
{
    return mcBeanNameMap->value(id);
}
