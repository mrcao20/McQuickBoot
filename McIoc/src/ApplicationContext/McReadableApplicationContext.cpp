#include "McIoc/ApplicationContext/impl/McReadableApplicationContext.h"

#include "McIoc/BeanDefinitionReader/IMcBeanDefinitionReader.h"
#include "McIoc/BeanFactory/impl/McPointerBeanFactory.h"
#include "McIoc/BeanFactory/impl/McSharedBeanFactory.h"
#include "McIoc/PropertyParser/impl/McDefaultPropertyConverter.h"

MC_DECL_PRIVATE_DATA(McReadableApplicationContext)
IMcBeanDefinitionReaderPtr beanDefinitionReader;
MC_DECL_PRIVATE_DATA_END

McReadableApplicationContext::McReadableApplicationContext(QObject *parent)
    : McReadableApplicationContext(McSharedBeanFactoryPtr::create(), parent)
{
}

McReadableApplicationContext::McReadableApplicationContext(
        IMcConfigurableBeanFactoryConstPtrRef factory,
        QObject *parent)
    : McAbstractApplicationContext(factory, parent)
{
    MC_NEW_PRIVATE_DATA(McReadableApplicationContext);

    auto related = McPointerBeanFactoryPtr::create();
    auto converter = McDefaultPropertyConverterPtr::create();
    converter->addReferenceResolver(factory.staticCast<McAbstractBeanFactory>().data());
    converter->addReferenceResolver(related.data());
    factory->setPropertyConverter(converter);
    related->setPropertyConverter(converter);
    addRelatedBeanFactory(related);
}

McReadableApplicationContext::McReadableApplicationContext(
        IMcConfigurableBeanFactoryConstPtrRef factory,
        IMcBeanDefinitionReaderConstPtrRef reader,
        QObject *parent)
    : McReadableApplicationContext(factory, parent)
{
   setReader(reader);
}

McReadableApplicationContext::McReadableApplicationContext(IMcBeanDefinitionReaderConstPtrRef reader,
                                                           QObject *parent)
    : McReadableApplicationContext(McSharedBeanFactoryPtr::create(), reader, parent)
{
}

McReadableApplicationContext::~McReadableApplicationContext() 
{
}

void McReadableApplicationContext::readBeans() noexcept 
{
    d->beanDefinitionReader->readBeanDefinition(this);
}

void McReadableApplicationContext::setReader(
        IMcBeanDefinitionReaderConstPtrRef reader) noexcept 
{
    d->beanDefinitionReader = reader;
    
    readBeans();
}
