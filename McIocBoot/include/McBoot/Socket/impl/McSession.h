#pragma once

#include "../IMcSession.h"

QT_BEGIN_NAMESPACE
class QJsonObject;
QT_END_NAMESPACE

class MCIOCBOOT_EXPORT McSession 
        : public QObject
        , public IMcSession 
{
    Q_OBJECT
    MC_DECL_INIT(McSession)
    MC_DEFINE_TYPELIST(QObject, MC_DECL_TYPELIST(IMcSession))
public:
    explicit McSession(QObject *parent = nullptr);
    ~McSession() override;
    
    void send(const QString &msg) noexcept override;
    void send(const QJsonObject &jsonObj) noexcept override;
    void send(QObject *obj) noexcept override;
    void close() noexcept override;
    
signals:
    void signal_send(const QVariant &);
    void signal_close();
};

MC_DECL_METATYPE(McSession)
