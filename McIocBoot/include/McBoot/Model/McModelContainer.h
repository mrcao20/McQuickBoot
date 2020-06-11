#pragma once

#include "../McBootGlobal.h"

MC_FORWARD_DECL_PRIVATE_DATA(McModelContainer);

MC_FORWARD_DECL_CLASS(McIocBoot);

class MCIOCBOOT_EXPORT McModelContainer : public QObject 
{
    Q_OBJECT
public:
	explicit McModelContainer(QObject *parent = nullptr);
	~McModelContainer() override;

    void init(McIocBootConstPtrRef boot) noexcept;

private:
	MC_DECL_PRIVATE(McModelContainer)
};

MC_DECL_POINTER(McModelContainer)
