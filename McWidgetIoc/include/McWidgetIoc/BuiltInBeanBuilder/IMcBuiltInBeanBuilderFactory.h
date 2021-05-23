#pragma once

#include <McIoc/McGlobal.h>

MC_FORWARD_DECL_CLASS(IMcBuiltInBeanBuilder)

class IMcBuiltInBeanBuilderFactory
{
public:
    virtual ~IMcBuiltInBeanBuilderFactory() = default;

    virtual IMcBuiltInBeanBuilderPtr create() noexcept = 0;
};

MC_DECL_POINTER(IMcBuiltInBeanBuilderFactory)
