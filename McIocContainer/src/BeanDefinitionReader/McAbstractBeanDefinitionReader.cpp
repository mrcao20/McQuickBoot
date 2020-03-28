#include "include/BeanDefinitionReader/impl/McAbstractBeanDefinitionReader.h"

#include "include/BeanFactory/IMcBeanDefinitionRegistry.h"

MC_DECL_PRIVATE_DATA(McAbstractBeanDefinitionReader)
IMcBeanDefinitionRegistry *registry{nullptr};
MC_DECL_PRIVATE_DATA_END

McAbstractBeanDefinitionReader::McAbstractBeanDefinitionReader(QObject *parent)
	: QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McAbstractBeanDefinitionReader);
}

McAbstractBeanDefinitionReader::~McAbstractBeanDefinitionReader(){
}

IMcBeanDefinitionRegistry *McAbstractBeanDefinitionReader::registry() const noexcept {
    return  d->registry;
}

void McAbstractBeanDefinitionReader::readBeanDefinition(IMcBeanDefinitionRegistry *registry) noexcept {
    d->registry = registry;
    doReadBeanDefinition();
}
