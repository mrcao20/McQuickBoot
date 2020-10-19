#pragma once

#include "../IMcPropertyContent.h"

namespace McXmlBuilder {

MC_FORWARD_DECL_PRIVATE_DATA(McRef);

class MCIOC_EXPORT McRef : public IMcPropertyContent
{
public:
    McRef() noexcept;
    ~McRef() override;
    
    void setBeanName(const QString &val) noexcept;
    
    QDomElement toDomElement(QDomDocument &doc) const noexcept override;
    
private:
    MC_DECL_PRIVATE(McRef)
};

MC_DECL_POINTER(McRef)

}
