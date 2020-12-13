#pragma once

#include "../IMcPropertyContent.h"

namespace McXmlBuilder {

MC_FORWARD_DECL_PRIVATE_DATA(McMap);

class MCIOC_EXPORT McMap : public IMcPropertyContent
{
public:
    McMap() noexcept;
    ~McMap() override;
    
    void addContent(IMcPropertyContentConstPtrRef key,
                    IMcPropertyContentConstPtrRef value) noexcept;
    void addContent(const QString &key, IMcPropertyContentConstPtrRef value) noexcept;
    
    QDomElement toDomElement(QDomDocument &doc) const noexcept override;
    
private:
    MC_DECL_PRIVATE(McMap)
};

MC_DECL_POINTER(McMap)

}
