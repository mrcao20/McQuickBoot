#include "McIoc/ApplicationContext/impl/McReadableApplicationContext.h"

#include "McIoc/BeanDefinitionReader/IMcBeanDefinitionReader.h"
#include "McIoc/BeanFactory/impl/McDefaultBeanFactory.h"
#include "McIoc/PropertyParser/impl/McDefaultPropertyConverter.h"

MC_DECL_PRIVATE_DATA(McReadableApplicationContext)
IMcBeanDefinitionReaderPtr beanDefinitionReader;
MC_DECL_PRIVATE_DATA_END

McReadableApplicationContext::McReadableApplicationContext(QObject *parent)
    : McReadableApplicationContext(
          IMcConfigurableBeanFactoryPtr(
              McDefaultBeanFactoryPtr::create(McDefaultPropertyConverterPtr::create())), parent)
{
}

McReadableApplicationContext::McReadableApplicationContext(
        IMcConfigurableBeanFactoryConstPtrRef factory,
        QObject *parent)
    : McAbstractApplicationContext(factory, parent)
{
    MC_NEW_PRIVATE_DATA(McReadableApplicationContext);
}

McReadableApplicationContext::McReadableApplicationContext(
        IMcConfigurableBeanFactoryConstPtrRef factory,
        IMcBeanDefinitionReaderConstPtrRef reader,
        QObject *parent)
    : McReadableApplicationContext(factory, parent)
{
   setReader(reader);
}

McReadableApplicationContext::McReadableApplicationContext(
        IMcBeanDefinitionReaderConstPtrRef reader,
        QObject *parent)
    : McReadableApplicationContext(
          McDefaultBeanFactoryPtr::create(McDefaultPropertyConverterPtr::create()),
          reader, parent)
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
    
    /* 默认不调用此函数，即context默认仍然为懒加载，在IOCBoot中默认调用此函数来达到其
     * 预加载功能，即把是否懒加载的选择权交给用户代码
     */
//    refresh();
    readBeans();
}

void McReadableApplicationContext::doRefresh() noexcept 
{
    readBeans();
}
