#pragma once

#include "../IMcPropertyContent.h"

namespace McXmlBuilder {

MC_FORWARD_DECL_PRIVATE_DATA(McValue);

class MCIOC_EXPORT McValue : public IMcPropertyContent
{
public:
    McValue() noexcept;
    ~McValue() override;
    
    void setContent(const QString &val) noexcept;
    
    QDomElement toDomElement(QDomDocument &doc) const noexcept override;
    
private:
    MC_DECL_PRIVATE(McValue)
};

MC_DECL_POINTER(McValue)

}
