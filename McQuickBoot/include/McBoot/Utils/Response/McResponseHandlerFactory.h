#pragma once

#include "IMcResponseHandler.h"

class MCQUICKBOOT_EXPORT McResponseHandlerFactory
{
public:
    static QList<IMcResponseHandlerPtr> getHandlers() noexcept;
    static void addHanlder(const IMcResponseHandlerPtr &val) noexcept;
};

class MCQUICKBOOT_EXPORT McResponseHandlerRegistry
{
public:
    void addHanlder(const IMcResponseHandlerPtr &val) noexcept
    {
        McResponseHandlerFactory::addHanlder(val);
    }
};
