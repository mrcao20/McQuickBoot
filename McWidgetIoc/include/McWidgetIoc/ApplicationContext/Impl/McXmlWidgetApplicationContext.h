#pragma once

#include "McAbstractWidgetApplicationContext.h"

QT_BEGIN_NAMESPACE
MC_FORWARD_DECL_CLASS(QIODevice);
QT_END_NAMESPACE

MC_FORWARD_DECL_PRIVATE_DATA(McXmlWidgetApplicationContext);

class MCWIDGETIOC_EXPORT McXmlWidgetApplicationContext : public McAbstractWidgetApplicationContext
{
    Q_OBJECT
public:
    explicit McXmlWidgetApplicationContext(QObject *parent = nullptr);
    McXmlWidgetApplicationContext(QIODeviceConstPtrRef device,
                                  const QString &flag = QString(),
                                  QObject *parent = nullptr);
    McXmlWidgetApplicationContext(const QList<QIODevicePtr> &devices,
                                  const QString &flag = QString(),
                                  QObject *parent = nullptr);
    McXmlWidgetApplicationContext(const QString &location,
                                  const QString &flag = QString(),
                                  QObject *parent = nullptr);
    McXmlWidgetApplicationContext(const QStringList &locations,
                                  const QString &flag = QString(),
                                  QObject *parent = nullptr);
    ~McXmlWidgetApplicationContext() override;

    void setDevices(const QList<QIODevicePtr> &devices, const QString &flag = QString()) noexcept;

private:
    MC_DECL_PRIVATE(McXmlWidgetApplicationContext)
};

MC_DECL_POINTER(McXmlWidgetApplicationContext)
