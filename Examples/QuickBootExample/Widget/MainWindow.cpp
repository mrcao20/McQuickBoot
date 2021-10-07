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
#include "MainWindow.h"

#include <QHBoxLayout>

#include <McWidgetIoc/McWidgetGlobal.h>

MC_STATIC()
qRegisterMetaType<MainWindow *>();
//! WidgetIoc内部已经对QList<QWidget *>进行了注册，所以这里不再需要注册
//! MC_REGISTER_LIST_CONVERTER(QList<QWidget *>)
MC_STATIC_END

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setCentralWidget(new QWidget(this));
    auto layout = new QHBoxLayout(centralWidget());
    centralWidget()->setLayout(layout);
}

QList<QWidget *> MainWindow::widgets() const
{
    QList<QWidget *> ws;
    auto count = centralWidget()->layout()->count();
    for (int i = 0; i < count; ++i) {
        auto item = centralWidget()->layout()->itemAt(i);
        if (item->widget() == nullptr) {
            continue;
        }
        ws.append(item->widget());
    }
    return ws;
}

void MainWindow::setWidgets(const QList<QWidget *> &val)
{
    for (auto w : val) {
        centralWidget()->layout()->addWidget(w);
    }
}
