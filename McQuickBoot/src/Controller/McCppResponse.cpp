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
#include "McBoot/Controller/impl/McCppResponse.h"

#include <QVariant>

#include <McIoc/Utils/McScopedFunction.h>

#include "McBoot/Controller/impl/McResult.h"
#include "McBoot/Utils/McJsonUtils.h"

MC_DECL_PRIVATE_DATA(McCppResponse)
bool isQVariant{false};
int argumentId{-1};
const QObject *recever{nullptr};
QtPrivate::QSlotObjectBase *callback{nullptr};
QtPrivate::QSlotObjectBase *error{nullptr};
MC_DECL_PRIVATE_DATA_END

MC_INIT(McCppResponse)
MC_INIT_END

McCppResponse::McCppResponse(QObject *parent) : McAbstractResponse(parent)
{
    MC_NEW_PRIVATE_DATA(McCppResponse);
}

McCppResponse::~McCppResponse()
{
    if (d->callback == nullptr) {
        return;
    }
    d->callback->destroyIfLastRef();
    d->callback = nullptr;
}

QPointer<McCppResponse> McCppResponse::capture()
{
    QPointer<McCppResponse> p(this);
    return p;
}

void McCppResponse::callCallback() noexcept
{
    call(d->callback);
}

void McCppResponse::callError() noexcept
{
    call(d->error);
}

McCppResponse &McCppResponse::thenImpl(bool isQVariant,
                                       int argumentId,
                                       const QObject *recever,
                                       QtPrivate::QSlotObjectBase *callback) noexcept
{
    d->isQVariant = isQVariant;
    d->argumentId = argumentId;
    d->recever = recever;
    d->callback = callback;
    setAsyncCall(false);
    return *this;
}

McCppResponse &McCppResponse::syncThenImpl(bool isQVariant,
                                           int argumentId,
                                           const QObject *recever,
                                           QtPrivate::QSlotObjectBase *callback) noexcept
{
    return thenImpl(isQVariant, argumentId, recever, callback);
}

McCppResponse &McCppResponse::asyncThenImpl(bool isQVariant,
                                            int argumentId,
                                            const QObject *recever,
                                            QtPrivate::QSlotObjectBase *callback) noexcept
{
    d->isQVariant = isQVariant;
    d->argumentId = argumentId;
    d->recever = recever;
    d->callback = callback;
    setAsyncCall(true);
    return *this;
}

McCppResponse &McCppResponse::errorImpl(bool isQVariant,
                                        int argumentId,
                                        const QObject *recever,
                                        QtPrivate::QSlotObjectBase *func) noexcept
{
    d->isQVariant = isQVariant;
    d->argumentId = argumentId;
    d->recever = recever;
    d->error = func;
    setAsyncCall(false);
    return *this;
}

void McCppResponse::call(QtPrivate::QSlotObjectBase *func) noexcept
{
    if (func == nullptr) {
        return;
    }
    auto body = this->body();
    void *bodyStar = nullptr;
    if (d->isQVariant) {
        body = McJsonUtils::serialize(body);
        bodyStar = &body;
    } else if (d->argumentId != -1) {
        if (body.userType() != d->argumentId) {
            body = McJsonUtils::serialize(body);
            if (body.userType() == qMetaTypeId<QJsonObject>()) {
                body = McJsonUtils::deserialize(body, d->argumentId);
            }
            if (body.userType() != d->argumentId) {
                qCCritical(mcQuickBoot(),
                           "cannot construct object for typeId: %d. className: %s",
                           d->argumentId,
                           QMetaType::typeName(d->argumentId));
                return;
            }
        }
        bodyStar = body.data();
    }
    void *args[] = {nullptr, bodyStar};
    func->call(const_cast<QObject *>(d->recever), args);
}
