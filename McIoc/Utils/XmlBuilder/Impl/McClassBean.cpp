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
#include "McClassBean.h"

#include <QXmlStreamWriter>

#include "McIocConstantGlobal.h"

namespace Mc::XmlBuilder {
MC_DECL_PRIVATE_DATA(ClassBean)
QString className;
MC_DECL_PRIVATE_DATA_END

ClassBean::ClassBean() noexcept
{
    MC_NEW_PRIVATE_DATA(ClassBean);
}

ClassBean::ClassBean(const QString &name) noexcept
    : ClassBean()
{
    setClassName(name);
}

ClassBean::ClassBean(const QString &className, const QString &beanName) noexcept
    : ClassBean(className)
{
    setBeanName(beanName);
}

ClassBean::~ClassBean() {}

void ClassBean::setClassName(const QString &name) noexcept
{
    d->className = name;
}

void ClassBean::completeAttribute(QXmlStreamWriter &writer) const noexcept
{
    if (!d->className.isEmpty()) {
        writer.writeAttribute(Mc::Constant::Tag::Xml::CLASS, d->className);
    }
}
} // namespace Mc::XmlBuilder
