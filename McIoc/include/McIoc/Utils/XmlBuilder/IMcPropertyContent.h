#pragma once

#include "IMcXml.h"

namespace McXmlBuilder {

class IMcPropertyContent : public IMcXml
{
public:
    ~IMcPropertyContent() override = default;
};

MC_DECL_POINTER(IMcPropertyContent)

}
