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
#include "McWidgetGlobal.h"

#include <QStackedWidget>

Q_LOGGING_CATEGORY(mcWidget, "mc.widget")

MC_STATIC(Mc::Widget::BuildInTypeRegistry)
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
