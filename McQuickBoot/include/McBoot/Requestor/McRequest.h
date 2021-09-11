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

#include <QSharedData>

#include "../McBootGlobal.h"
#include "../Utils/McCancel.h"
#include "../Utils/McProgress.h"

namespace Mc::QuickBoot::Private {

template<typename T>
class CustomRequestBuilder;

}

struct MCQUICKBOOT_EXPORT McRequestSharedData : public QSharedData
{
    McCancel cancel;
    McProgress progress;
    QVariantList params;
};

class MCQUICKBOOT_EXPORT McRequest
{
    MC_DECL_INIT(McRequest)
public:
    McRequest() noexcept;
    ~McRequest();

    McCancel cancel() const noexcept;
    McProgress progress() const noexcept;

    int count() const noexcept;
    QVariant variant(int i) const noexcept;
    template<typename T>
    T at(int i) const noexcept
    {
        QVariant var = variant(i);
        return var.value<T>();
    }
    template<typename... Args>
    bool check() const noexcept
    {
        auto argSize = sizeof...(Args);
        if (argSize != d->params.size()) {
            return false;
        }
        return CheckHelper<Args...>::check(0, d->params);
    }

private:
    void setCancel(const McCancel &val) noexcept;
    void setProgress(const McProgress &val) noexcept;
    void setParams(const QVariantList &val) noexcept;
    template<typename...>
    struct CheckHelper;
    template<typename T, typename... Args>
    struct CheckHelper<T, Args...>
    {
        static bool check(int index, const QVariantList &params) noexcept
        {
            auto param = params.at(index);
            if (!param.canConvert<T>()) {
                return false;
            }
            return CheckHelper<Args...>::check(index + 1, params);
        }
    };

private:
    QExplicitlySharedDataPointer<McRequestSharedData> d;

    friend class McRequestRunner;
    friend class McControllerContainer;
    template<typename T>
    friend class Mc::QuickBoot::Private::CustomRequestBuilder;
};

template<>
struct McRequest::CheckHelper<>
{
    static bool check(int, const QVariantList &) noexcept { return true; }
};

Q_DECLARE_METATYPE(McRequest)

template<typename... Args>
class McCustomRequest : public McRequest
{};

namespace Mc::QuickBoot {
namespace Private {

class ICustomRequestBuilder
{
public:
    virtual ~ICustomRequestBuilder() = default;
    virtual QVariant build(const QVariantList &vals, const McRequest &request) noexcept = 0;
};
MC_DECL_POINTER(ICustomRequestBuilder);

template<typename T>
class CustomRequestBuilder : public ICustomRequestBuilder
{
public:
    ~CustomRequestBuilder() override = default;
    QVariant build(const QVariantList &vals, const McRequest &request) noexcept override
    {
        T t;
        t.setParams(vals);
        t.setCancel(request.cancel());
        t.setProgress(request.progress());
        return QVariant::fromValue(t);
    }
};
template<typename T>
using CustomRequestBuilderPtr = QSharedPointer<CustomRequestBuilder<T>>;

MCQUICKBOOT_EXPORT void registerCustomRequest(int mainId, const QList<int> &childrenId) noexcept;
MCQUICKBOOT_EXPORT void registerCustomRequestBuilder(
    int mainId, const ICustomRequestBuilderPtr &builder) noexcept;
MCQUICKBOOT_EXPORT QVariant buildCustomRequest(int mainId,
                                               const QVariantList &vals,
                                               const McRequest &request) noexcept;
MCQUICKBOOT_EXPORT QHash<int, QList<int>> getAllCustomRequestIds() noexcept;
MCQUICKBOOT_EXPORT QList<int> getCustomRequestId(int mainId) noexcept;
MCQUICKBOOT_EXPORT bool isContainedCustomRequest(int mainId) noexcept;

} // namespace Private

template<typename T>
struct CustomRequestRegister;

template<typename... Args>
struct CustomRequestRegister<McCustomRequest<Args...>>
{
    static void registerRequest() noexcept
    {
        using namespace Private;
        auto mainId = qMetaTypeId<McCustomRequest<Args...>>();
        registerCustomRequestBuilder(mainId,
                                     CustomRequestBuilderPtr<McCustomRequest<Args...>>::create());
        QList<int> childrenId;
        (childrenId << ... << qMetaTypeId<Args>());
        registerCustomRequest(mainId, childrenId);
    }
};

} // namespace Mc::QuickBoot

#define MC_BOOT_REGISTER_REQUEST(Type) \
    Mc::QuickBoot::CustomRequestRegister<Type>::registerRequest();
