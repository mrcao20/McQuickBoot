#pragma once

#include "../IMcPropertyContent.h"

namespace McXmlBuilder {

MC_FORWARD_DECL_PRIVATE_DATA(McEnum);

class MCIOCCONTAINER_EXPORT McEnum : public IMcPropertyContent
{
public:
    McEnum() noexcept;
    ~McEnum() override;
    
    void setScope(const QString &val) noexcept;
    void setType(const QString &val) noexcept;
    void setValue(const QString &val) noexcept;
    
    QDomElement toDomElement(QDomDocument &doc) const noexcept override;
    
private:
    MC_DECL_PRIVATE(McEnum)
};

MC_DECL_POINTER(McEnum)

}
