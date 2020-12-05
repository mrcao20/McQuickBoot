#pragma once

#include "IMcXml.h"

namespace McXmlBuilder {

class IMcBeanContent : public IMcXml
{
public:
    ~IMcBeanContent() override = default;
};

MC_DECL_POINTER(IMcBeanContent)

}
