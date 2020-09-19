#pragma once

#include "../../../McGlobal.h"

#include <QDomDocument>

namespace McXmlBuilder {

MC_FORWARD_DECL_CLASS(IMcBean);

MC_FORWARD_DECL_PRIVATE_DATA(McBeanCollection);

class MCIOCCONTAINER_EXPORT McBeanCollection
{
public:
    McBeanCollection() noexcept;
    ~McBeanCollection();
    
    void addBean(IMcBeanConstPtrRef bean) noexcept;
    
    QDomDocument toDomDocument() const noexcept;
    void writeToDevice(QIODevice *dev, int indent = 4) const noexcept;
    
private:
    MC_DECL_PRIVATE(McBeanCollection)
};

MC_DECL_POINTER(McBeanCollection)

}
