#pragma once

#include "../IMcQmlSocketContainer.h"

MC_FORWARD_DECL_PRIVATE_DATA(McQmlSocketContainer);

MC_FORWARD_DECL_CLASS(McIocBoot);
MC_FORWARD_DECL_CLASS(McInnerSocket);

class MCIOCBOOT_EXPORT McQmlSocketContainer 
        : public QObject
        , public IMcQmlSocketContainer 
{
    Q_OBJECT
public:
    McQmlSocketContainer(QObject *parent = nullptr);
    ~McQmlSocketContainer() override;
    
    void init(McIocBootConstPtrRef boot) noexcept;
    
    McQmlSocket *addConnect(const QString &uri) noexcept override;
    McQmlSocket *addConnect(const QString &uri, const QJSValue &data) noexcept override;
    
private:
    void addConnect(McQmlSocket *socket, const QString &uri) noexcept;
    McInnerSocketPtr getMethods(QObjectConstPtrRef bean) noexcept;
    
private:
    MC_DECL_PRIVATE(McQmlSocketContainer)
};

MC_DECL_POINTER(McQmlSocketContainer)
