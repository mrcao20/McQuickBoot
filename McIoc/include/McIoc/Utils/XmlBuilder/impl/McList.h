#pragma once

#include "../IMcPropertyContent.h"

namespace McXmlBuilder {

MC_FORWARD_DECL_PRIVATE_DATA(McList);

class MCIOC_EXPORT McList : public IMcPropertyContent
{
public:
    McList() noexcept;
    ~McList() override;
    
    void addContent(IMcPropertyContentConstPtrRef val) noexcept;
    void addContent(const QString &val) noexcept;
    
    QDomElement toDomElement(QDomDocument &doc) const noexcept override;
    
private:
    MC_DECL_PRIVATE(McList)
};

MC_DECL_POINTER(McList)

}
