#pragma once

#include "../IMcBeanContent.h"

namespace McXmlBuilder {

MC_FORWARD_DECL_CLASS(IMcPropertyContent);

MC_FORWARD_DECL_PRIVATE_DATA(McProperty);

class MCIOCCONTAINER_EXPORT McProperty : public IMcBeanContent
{
public:
    McProperty() noexcept;
    ~McProperty() override;
    
    void setContent(const QString &name, IMcPropertyContentConstPtrRef val) noexcept;
    void setContent(const QString &name, const QString &value) noexcept;
    
    QDomElement toDomElement(QDomDocument &doc) const noexcept override;
    
private:
    MC_DECL_PRIVATE(McProperty)
};

MC_DECL_POINTER(McProperty)

}
