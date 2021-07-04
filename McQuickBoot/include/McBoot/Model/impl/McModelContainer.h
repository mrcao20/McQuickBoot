#pragma once

#include "../IMcModelContainer.h"

MC_FORWARD_DECL_PRIVATE_DATA(McModelContainer);

MC_FORWARD_DECL_CLASS(IMcQuickBoot);

class MCQUICKBOOT_EXPORT McModelContainer : public QObject, public IMcModelContainer
{
    Q_OBJECT
    MC_DECL_INIT(McModelContainer)
    MC_INTERFACES(IMcModelContainer)
    MC_COMPONENT
    MC_BEANNAME("modelContainer")
public:
    Q_INVOKABLE explicit McModelContainer(QObject *parent = nullptr);
    ~McModelContainer() override;

    void init(const IMcQuickBoot *boot) noexcept;

    QObject *getModel(const QString &beanName) noexcept override;

private:
    MC_DECL_PRIVATE(McModelContainer)
};

MC_DECL_METATYPE(McModelContainer)
