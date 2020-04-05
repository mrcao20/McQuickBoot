#pragma once

#include "../McBootGlobal.h"

#include <QJsonObject>

QT_BEGIN_NAMESPACE
class QRunable;
class QJSValue;
class QJSEngine;
QT_END_NAMESPACE

MC_FORWARD_DECL_PRIVATE_DATA(McQmlRequestor);

MC_FORWARD_DECL_CLASS(IMcControllerContainer);
MC_FORWARD_DECL_CLASS(IMcQmlSocketContainer);

class McQmlResponse;
class McQmlSocket;

class MCIOCBOOT_EXPORT McQmlRequestor : public QObject {
    Q_OBJECT
public:
    explicit McQmlRequestor(QJSEngine *jsEngine, QObject *parent = nullptr);
    ~McQmlRequestor() override;

    qint64 maxThreadCount() const noexcept;
    void setMaxThreadCount(int val) noexcept;
    void setControllerContainer(IMcControllerContainerConstPtrRef val) noexcept;
    void setSocketContainer(IMcQmlSocketContainerConstPtrRef val) noexcept;
    
    Q_INVOKABLE McQmlResponse *invoke(const QString &uri) noexcept;
	Q_INVOKABLE McQmlResponse *invoke(const QString &uri, const QJsonObject &data) noexcept;
    Q_INVOKABLE McQmlSocket *addConnect(const QString &uri) noexcept;
    Q_INVOKABLE McQmlSocket *addConnect(const QString &uri, const QJSValue &data) noexcept;
    
protected:
    void customEvent(QEvent *event) override;
    
private:
    void run(McQmlResponse *response, const QString &uri, const QVariant &body) noexcept;
    
private:
    MC_DECL_PRIVATE(McQmlRequestor)
};

MC_DECL_POINTER(McQmlRequestor)
