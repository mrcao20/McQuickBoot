#pragma once

#include "McReadableApplicationContext.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAnnotationApplicationContext);

class MCIOC_EXPORT McAnnotationApplicationContext : public McReadableApplicationContext
{
    Q_OBJECT
public:
    McAnnotationApplicationContext(IMcConfigurableBeanFactoryConstPtrRef factory,
                                   IMcBeanDefinitionReaderConstPtrRef reader,
                                   QObject *parent = nullptr);
    McAnnotationApplicationContext(IMcBeanDefinitionReaderConstPtrRef reader,
                                   QObject *parent = nullptr);
    McAnnotationApplicationContext(QObject *parent = nullptr);

    ~McAnnotationApplicationContext() override;

    void generateReader() noexcept;

    static void init() noexcept;

    static void initBeanDefinition(int type) noexcept;

    static void addConnect(const QString &beanName,
                           const QString &sender,
                           const QString &signal,
                           const QString &receiver,
                           const QString &slot,
                           Qt::ConnectionType type) noexcept;
    
private:
    MC_DECL_PRIVATE(McAnnotationApplicationContext)
};

MC_DECL_POINTER(McAnnotationApplicationContext)
