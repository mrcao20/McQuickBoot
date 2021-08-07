#pragma once

#include <McBoot/Utils/Response/IMcResponseHandler.h>

class HandlerTest : public QObject, public IMcResponseHandler
{
    Q_OBJECT
    MC_COMPONENT()
    MC_INTERFACES(IMcResponseHandler)
public:
    bool handler(QVariant &body) noexcept override;
};

MC_DECL_METATYPE(HandlerTest)

class HandlerTest2 : public IMcResponseHandler
{
    MC_INTERFACES(IMcResponseHandler)
public:
    bool handler(QVariant &body) noexcept override;
};

MC_DECL_METATYPE(HandlerTest2)
