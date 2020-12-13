#pragma once

#include "McReadableApplicationContext.h"

QT_BEGIN_NAMESPACE
MC_FORWARD_DECL_CLASS(QIODevice);
QT_END_NAMESPACE

MC_FORWARD_DECL_CLASS(IMcBeanDefinitionReader);

class MCIOC_EXPORT McXmlApplicationContext 
        : public McReadableApplicationContext 
{
    
    Q_OBJECT
public:
    explicit McXmlApplicationContext(QObject *parent = nullptr);
    McXmlApplicationContext(QIODeviceConstPtrRef device, QObject *parent = nullptr);
    McXmlApplicationContext(const QList<QIODevicePtr> &devices, QObject *parent = nullptr);
    McXmlApplicationContext(IMcBeanDefinitionReaderConstPtrRef reader,
                            QObject *parent = nullptr);
    McXmlApplicationContext(IMcConfigurableBeanFactoryConstPtrRef factory,
                            IMcBeanDefinitionReaderConstPtrRef reader,
                            QObject *parent = nullptr);
    ~McXmlApplicationContext() override;
    
    void setDevice(QIODeviceConstPtrRef device) noexcept;
    void setDevices(const QList<QIODevicePtr> &devices) noexcept;
};

MC_DECL_POINTER(McXmlApplicationContext)
