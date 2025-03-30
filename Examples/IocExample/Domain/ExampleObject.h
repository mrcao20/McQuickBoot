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
