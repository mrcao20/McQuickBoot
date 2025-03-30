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
#include "ExampleObject.h"

#include <QDebug>

MC_AUTO_INIT(ExampleObject)
mcRegisterContainer<QList<SimpleInterfacePtr>>();
mcRegisterContainer<QMap<QString, QString>>();
mcRegisterContainer<QMap<QString, SimpleInterfacePtr>>();
MC_INIT_END

void ExampleObject::simpleFunc()
{
    qDebug() << "ExampleObject::simpleFunc ==============================================================";
    qDebug() << "text:" << m_text;
    qDebug() << "pod:" << m_pod << m_pod->text;
    qDebug() << "gadget:" << m_gadget << m_gadget->text << m_gadget->text2;
    qDebug() << "gadgetPointer:" << m_gadgetPointer << m_gadgetPointer->text;
    for (auto v : m_list) {
        qDebug() << v;
        v->simpleFunc();
    }
    QMapIterator<QString, QString> itr(m_map);
    while (itr.hasNext()) {
        auto item = itr.next();
        qDebug() << item.key() << item.value();
    }
    qDebug() << "==============================================================";
}

QVariant ExampleObject::getKey() const noexcept
{
    return "ExampleObjectKey";
}
