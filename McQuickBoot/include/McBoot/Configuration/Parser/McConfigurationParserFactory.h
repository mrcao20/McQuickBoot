#pragma once

#include "IMcConfigurationParser.h"

class MCQUICKBOOT_EXPORT McConfigurationParserFactory
{
public:
    static QList<IMcConfigurationParserPtr> getParsers() noexcept;
    static void registerParser(const IMcConfigurationParserPtr &val) noexcept;
    template<typename T>
    static void registerParser() noexcept
    {
        auto p = QSharedPointer<T>::create();
        registerParser(p);
    }
};
