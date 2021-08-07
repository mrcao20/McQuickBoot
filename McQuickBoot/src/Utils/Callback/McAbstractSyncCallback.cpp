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
#include "McBoot/Utils/Callback/Impl/McAbstractSyncCallback.h"

namespace Mc {

struct CallbackEvent : public QEvent
{
    CallbackEvent(const QVariantList &val)
        : QEvent(static_cast<QEvent::Type>(QEvent::Type::User + 1)), varList(val)
    {}
    QVariantList varList;
};

} // namespace Mc

void McAbstractSyncCallback::destroy() noexcept
{
    deleteLater();
}

void McAbstractSyncCallback::call(const QVariantList &varList) noexcept
{
    qApp->postEvent(this, new Mc::CallbackEvent(varList));
}

void McAbstractSyncCallback::customEvent(QEvent *event)
{
    if (event->type() == QEvent::Type::User + 1) {
        Mc::CallbackEvent *e = static_cast<Mc::CallbackEvent *>(event);
        syncCall(e->varList);
    }
}
