#pragma once

#include "IMcPropertyContent.h"

namespace McXmlBuilder {

class IMcBean : public IMcPropertyContent
{
public:
    ~IMcBean() override = default;
};

MC_DECL_POINTER(IMcBean)

}
