#pragma once

#include "../IMcBeanDefinitionReader.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractBeanDefinitionReader)

class McAbstractBeanDefinitionReader
        : public QObject
        , public IMcBeanDefinitionReader 
{
    
    Q_OBJECT
public:
    explicit McAbstractBeanDefinitionReader(QObject *parent = nullptr);
	~McAbstractBeanDefinitionReader() override;
    
    IMcBeanDefinitionRegistry *registry() const noexcept;
    
    void readBeanDefinition(IMcBeanDefinitionRegistry *registry) noexcept override;
    
protected:
    virtual void doReadBeanDefinition() noexcept = 0;
    
private:
    MC_DECL_PRIVATE(McAbstractBeanDefinitionReader)
};

MC_DECL_POINTER(McAbstractBeanDefinitionReader)
