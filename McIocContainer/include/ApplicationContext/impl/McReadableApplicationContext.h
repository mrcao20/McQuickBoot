#pragma once

#include "../IMcReadableApplicationContext.h"
#include "McAbstractApplicationContext.h"

MC_FORWARD_DECL_PRIVATE_DATA(McReadableApplicationContext);

class MCIOCCONTAINER_EXPORT McReadableApplicationContext
        : public McAbstractApplicationContext
        , public IMcReadableApplicationContext {
    
    Q_OBJECT
public:
    explicit McReadableApplicationContext(QObject *parent = nullptr);
    McReadableApplicationContext(IMcConfigurableBeanFactoryConstPtrRef factory
                                 , QObject *parent = nullptr);
    McReadableApplicationContext(IMcConfigurableBeanFactoryConstPtrRef factory
                                 , IMcBeanDefinitionReaderConstPtrRef reader
                                 , QObject *parent = nullptr);
    McReadableApplicationContext(IMcBeanDefinitionReaderConstPtrRef reader
                                 , QObject *parent = nullptr);
    ~McReadableApplicationContext() override;
    
    void readBeans() noexcept override;
    
    void setReader(IMcBeanDefinitionReaderConstPtrRef reader) noexcept override;
    
protected:
    void doRefresh() noexcept override;
    
private:
    MC_DECL_PRIVATE(McReadableApplicationContext)
};

MC_DECL_POINTER(McReadableApplicationContext)
