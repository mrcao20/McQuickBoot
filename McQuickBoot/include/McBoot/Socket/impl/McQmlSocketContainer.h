#pragma once

#include "../IMcQmlSocketContainer.h"

MC_FORWARD_DECL_PRIVATE_DATA(McQmlSocketContainer);

MC_FORWARD_DECL_CLASS(IMcQuickBoot);
MC_FORWARD_DECL_CLASS(McInnerSocket);

class MCQUICKBOOT_EXPORT McQmlSocketContainer 
        : public QObject
        , public IMcQmlSocketContainer 
{
    Q_OBJECT
    MC_DECL_INIT(McQmlSocketContainer)
    MC_COMPONENT
    MC_BEANNAME("socketContainer")
public:
    Q_INVOKABLE McQmlSocketContainer(QObject *parent = nullptr);
    ~McQmlSocketContainer() override;
    
    void init(IMcQuickBootConstPtrRef boot) noexcept;
    
    McQmlSocket *addConnect(const QString &uri) noexcept override;
    McQmlSocket *addConnect(const QString &uri, const QJSValue &data) noexcept override;
    
private:
    void addConnect(McQmlSocket *socket, const QString &uri) noexcept;
    McInnerSocketPtr getMethods(QObjectConstPtrRef bean) noexcept;
    
private:
    MC_DECL_PRIVATE(McQmlSocketContainer)
};

MC_DECL_METATYPE(McQmlSocketContainer)
