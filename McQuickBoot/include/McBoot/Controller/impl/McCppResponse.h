#pragma once

#include "McAbstractResponse.h"

MC_FORWARD_DECL_PRIVATE_DATA(McCppResponse);

class MCQUICKBOOT_EXPORT McCppResponse : public McAbstractResponse
{
    Q_OBJECT
    MC_DECL_INIT(McCppResponse)
public:
    explicit McCppResponse(QObject *parent = nullptr);
    ~McCppResponse() override;

    McCppResponse &then(const std::function<void(const QVariant &)> &callback) noexcept;
    McCppResponse &syncThen(const std::function<void(const QVariant &)> &callback) noexcept;
    McCppResponse &asyncThen(const std::function<void(const QVariant &)> &callback) noexcept;

protected:
    void callCallback() noexcept override;

private:
    MC_DECL_PRIVATE(McCppResponse)
};
