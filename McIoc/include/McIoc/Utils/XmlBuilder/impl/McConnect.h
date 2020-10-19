#pragma once

#include "../IMcBeanContent.h"

namespace McXmlBuilder {

MC_FORWARD_DECL_PRIVATE_DATA(McConnect);

class MCIOC_EXPORT McConnect : public IMcBeanContent
{
public:
    McConnect() noexcept;
    ~McConnect() override;
    
    void setSender(const QString &val) noexcept;
    void setSignal(const QString &val) noexcept;
    void setReceiver(const QString &val) noexcept;
    void setSlot(const QString &val) noexcept;
    void setConnectionType(const QString &val) noexcept;
    
    QDomElement toDomElement(QDomDocument &doc) const noexcept override;
    
private:
    MC_DECL_PRIVATE(McConnect)
};

MC_DECL_POINTER(McConnect)

}
