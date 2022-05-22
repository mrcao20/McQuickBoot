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
#include "McWidgetBeanBuilder.h"

#include <QWidget>

void McWidgetBeanBuilder::complete(QVariant &bean, QThread *thread) noexcept
{
    //! Widget控件不能移动线程
    Q_UNUSED(thread)
    super::complete(bean, nullptr);
}

void McWidgetBeanBuilder::setParent(QObject *bean, QObject *parent) noexcept
{
    auto widgetBean = qobject_cast<QWidget *>(bean);
    auto widgetParent = qobject_cast<QWidget *>(parent);
    if (widgetBean == nullptr || widgetParent == nullptr) {
        super::setParent(bean, parent);
    } else {
        widgetBean->setParent(widgetParent);
    }
}

QVariant McWidgetBeanBuilder::convertRef(const QVariant &value, const QVariant &extra) const noexcept
{
    auto target = super::convertRef(value, extra);
    auto targetWidget = target.value<QWidget *>();
    if (targetWidget == nullptr || targetWidget->parentWidget() != nullptr) {
        return target;
    }
    auto parentWidget = extra.value<QWidget *>();
    if (parentWidget == nullptr) {
        return target;
    }
    targetWidget->setParent(parentWidget);
    return target;
}
