#pragma once

#include <QObject>

#include "../McBootGlobal.h"

MC_FORWARD_DECL_CLASS(IMcQuickBoot)

MC_FORWARD_DECL_PRIVATE_DATA(McConfigurationContainer);

class McConfigurationContainer : public QObject
{
    Q_OBJECT
    MC_COMPONENT("configurationContainer")
public:
    Q_INVOKABLE explicit McConfigurationContainer(QObject *parent = nullptr) noexcept;
    ~McConfigurationContainer() override;

    void init(const IMcQuickBoot *boot) noexcept;

private:
    QVector<QVariant> buildArguments(IMcApplicationContextConstPtrRef appCtx,
                                     const QMetaMethod &method) const noexcept;

private:
    MC_DECL_PRIVATE(McConfigurationContainer)
};

MC_DECL_METATYPE(McConfigurationContainer)

