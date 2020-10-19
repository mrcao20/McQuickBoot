#pragma once

#include "../../McGlobal.h"

#include <QDomElement>

class QDomDocument;

namespace McXmlBuilder {

class IMcXml
{
public:
    virtual ~IMcXml() = default;
    
    virtual QDomElement toDomElement(QDomDocument &doc) const noexcept = 0;
};

MC_DECL_POINTER(IMcXml)

}
