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
#include "McWidgetGlobal.h"

#include <QStackedWidget>

Q_LOGGING_CATEGORY(mcWidget, "mc.widget")

MC_STATIC(Mc::RoutinePriority::RoutineWidgetBuildInTypeRegistry)
mcRegisterContainer<QWidgetList>();
mcAddCustomWidgetBuilderFactory<QStackedWidget>([](QStackedWidget *w, const QString &name, const QVariant &var) {
    if (name == QLatin1String("widgets")) {
        auto widgets = var.value<QWidgetList>();
        for (auto &widget : qAsConst(widgets)) {
            w->addWidget(widget);
        }
    } else {
        w->setProperty(name.toLatin1(), var);
    }
});
MC_STATIC_END
