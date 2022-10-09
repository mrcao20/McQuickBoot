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

#include <McIoc/BeanBuilder/IMcCustomPlaceholder.h>
#include <McIoc/McIocGlobal.h>

#include "../CoreExample/SimpleInterface.h"
#include "SimpleGadget.h"
#include "SimplePod.h"

class ExampleObject
    : public QObject
    , public SimpleInterface
    , public IMcCustomPlaceholder
{
    Q_OBJECT
    Q_INTERFACES(SimpleInterface)
    MC_FULL_DEFINE(ExampleObject, QObject, SimpleInterface)
    Q_PROPERTY(QString text MEMBER m_text)
    Q_PROPERTY(SimplePodPtr pod MEMBER m_pod)
    Q_PROPERTY(SimpleGadgetPtr gadget MEMBER m_gadget)
    Q_PROPERTY(SimpleGadgetPointer *gadgetPointer MEMBER m_gadgetPointer)
    Q_PROPERTY(QList<SimpleInterfacePtr> list MEMBER m_list)
    Q_PROPERTY(QMap<QString, QString> map MEMBER m_map)
public:
    void simpleFunc() override;
    QVariant getKey() const noexcept override;

private:
    QString m_text;
    SimplePodPtr m_pod;
    SimpleGadgetPtr m_gadget;
    SimpleGadgetPointer *m_gadgetPointer{nullptr};
    QList<SimpleInterfacePtr> m_list;
    QMap<QString, QString> m_map;
};

MC_DECL_METATYPE(ExampleObject)
