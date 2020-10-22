#pragma once

#include "../McBootGlobal.h"

MC_FORWARD_DECL_PRIVATE_DATA(McModelContainer);

MC_FORWARD_DECL_CLASS(McQuickBoot);

class MCQUICKBOOT_EXPORT McModelContainer : public QObject 
{
    Q_OBJECT
    MC_DECL_INIT(McModelContainer)
    MC_COMPONENT
    MC_BEANNAME("modelContainer")
public:
	Q_INVOKABLE explicit McModelContainer(QObject *parent = nullptr);
	~McModelContainer() override;

    void init(McQuickBootConstPtrRef boot) noexcept;

private:
	MC_DECL_PRIVATE(McModelContainer)
};

MC_DECL_METATYPE(McModelContainer)
