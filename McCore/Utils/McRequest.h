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

#include "McCancel.h"
#include "McPause.h"
#include "McProgress.h"

MC_FORWARD_DECL_PRIVATE_DATA(McRequest)

class MC_CORE_EXPORT McRequest
{
public:
    McRequest() noexcept;
    ~McRequest();

    McRequest(const McRequest &o) noexcept;
    McRequest &operator=(const McRequest &o) noexcept;
    McRequest(McRequest &&o) noexcept;
    McRequest &operator=(McRequest &&o) noexcept;

    McCancel cancel() const noexcept;
    McPause pause() const noexcept;
    McProgress progress() const noexcept;
    QVariantList params() const noexcept;
    qsizetype count() const noexcept;
    template<typename T>
    T at(qsizetype i) const noexcept
    {
        QVariant var = at(i);
        return McPrivate::toRealValue<T>(var);
    }
    QVariant at(qsizetype i) const noexcept;
    template<typename T>
    T rollAt() const noexcept
    {
        QVariant var = rollAt();
        return McPrivate::toRealValue<T>(var);
    }
    QVariant rollAt() const noexcept;
    template<typename... Args>
    bool check() const noexcept
    {
        auto argSize = sizeof...(Args);
        if (argSize != count()) {
            return false;
        }
        return CheckHelper<Args...>::check(0, params());
    }

    void setCancel(const McCancel &val) noexcept;
    void setPause(const McPause &val) noexcept;
    void setProgress(const McProgress &val) noexcept;
    void setParams(const QVariantList &val) noexcept;
    template<typename T>
    void addParam(const T &t) noexcept
    {
        addParam(McPrivate::toQVariant(t));
    }
    void addParam(const QVariant &var) noexcept;

private:
    template<typename...>
    struct CheckHelper;
    template<typename T, typename... Args>
    struct CheckHelper<T, Args...>
    {
        static bool check(qsizetype index, const QVariantList &params) noexcept
        {
            auto param = params.at(index);
            if (!param.canConvert<T>()) {
                return false;
            }
            return CheckHelper<Args...>::check(index + 1, params);
        }
    };

private:
    MC_DECL_SHARED_PRIVATE(McRequest)
};

template<>
struct McRequest::CheckHelper<>
{
    static bool check(qsizetype, const QVariantList &) noexcept { return true; }
};

#ifdef MC_USE_QT5
Q_DECLARE_METATYPE(McRequest)
#endif

/////////////////////////////////////////////////////////////////
template<typename... Args>
class McCustomRequest : public McRequest
{
};

namespace McPrivate {
class ICustomRequestBuilder
{
    MC_DEFINE_INTERFACE(ICustomRequestBuilder)
public:
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
        t.setPause(request.pause());
        return QVariant::fromValue(t);
    }
};
template<typename T>
using CustomRequestBuilderPtr = QSharedPointer<CustomRequestBuilder<T>>;

#ifdef MC_USE_QT5
MC_CORE_EXPORT void registerCustomRequest(int mainMetaType, const QList<int> &childMetaTypes) noexcept;
MC_CORE_EXPORT void registerCustomRequestBuilder(int mainMetaType, const ICustomRequestBuilderPtr &builder) noexcept;
MC_CORE_EXPORT QVariant buildCustomRequest(
    int mainMetaType, const QVariantList &vals, const McRequest &request) noexcept;
MC_CORE_EXPORT QHash<int, QList<int>> getAllCustomRequestMetaTypes() noexcept;
MC_CORE_EXPORT QList<int> getCustomRequestMetaType(int mainMetaType) noexcept;
MC_CORE_EXPORT bool isContainedCustomRequest(int mainMetaType) noexcept;
#else
MC_CORE_EXPORT void registerCustomRequest(QMetaType mainMetaType, const QList<QMetaType> &childMetaTypes) noexcept;
MC_CORE_EXPORT void registerCustomRequestBuilder(
    QMetaType mainMetaType, const ICustomRequestBuilderPtr &builder) noexcept;
MC_CORE_EXPORT QVariant buildCustomRequest(
    QMetaType mainMetaType, const QVariantList &vals, const McRequest &request) noexcept;
MC_CORE_EXPORT QHash<QMetaType, QList<QMetaType>> getAllCustomRequestMetaTypes() noexcept;
MC_CORE_EXPORT QList<QMetaType> getCustomRequestMetaType(QMetaType mainMetaType) noexcept;
MC_CORE_EXPORT bool isContainedCustomRequest(QMetaType mainMetaType) noexcept;
#endif

template<typename T>
struct CustomRequestRegister;

template<typename... Args>
struct CustomRequestRegister<McCustomRequest<Args...>>
{
    static void registerRequest() noexcept
    {
#ifdef MC_USE_QT5
        auto metaType = qMetaTypeId<McCustomRequest<Args...>>();
#else
        auto metaType = QMetaType::fromType<McCustomRequest<Args...>>();
#endif
        registerCustomRequestBuilder(metaType, CustomRequestBuilderPtr<McCustomRequest<Args...>>::create());
        auto childMetaTypes = McPrivate::MetaTypeHelper<QtPrivate::List<Args...>>::metaTypes();
        registerCustomRequest(metaType, childMetaTypes);
    }
};
} // namespace McPrivate

template<typename T>
void mcRegisterRequest() noexcept
{
    McPrivate::CustomRequestRegister<T>::registerRequest();
}
