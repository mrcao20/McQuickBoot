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
#include "McBoot/Model/impl/McModelContainer.h"

#include <QDebug>
#include <QQmlEngine>

#include <McIoc/ApplicationContext/IMcApplicationContext.h>

#include "McBoot/IMcQuickBoot.h"

MC_INIT(McModelContainer)
MC_REGISTER_BEAN_FACTORY(McModelContainer)
MC_INIT_END

MC_DECL_PRIVATE_DATA(McModelContainer)
QMap<QString, QObjectPtr> models; //!< 键为beanName，值为model对象
MC_DECL_PRIVATE_DATA_END

McModelContainer::McModelContainer(QObject *parent)
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McModelContainer);
}

McModelContainer::~McModelContainer() 
{
}

void McModelContainer::init(const IMcQuickBoot *boot) noexcept
{
    d->models.clear();
    auto appCtx = boot->getApplicationContext();
    auto beanNames = Mc::getComponents(appCtx, MC_MODEL_TAG);
    for (const auto &beanName : beanNames) {
        auto obj = appCtx->getBean(beanName);
        if(!obj) {
            qCritical() << QString("model for named '%1' not exists").arg(beanName);
            continue;
        }
        if (d->models.contains(beanName)) {
            qCritical() << QString("model for named '%1' is repeated").arg(beanName);
            continue;
        }
        QQmlEngine::setObjectOwnership(obj.data(), QQmlEngine::CppOwnership);
        d->models.insert(beanName, obj);
    }
}

QObject *McModelContainer::getModel(const QString &beanName) noexcept
{
    return d->models.value(beanName, QObjectPtr()).data();
}
