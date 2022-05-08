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
