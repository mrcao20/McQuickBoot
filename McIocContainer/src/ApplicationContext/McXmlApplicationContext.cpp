#include "include/ApplicationContext/impl/McXmlApplicationContext.h"

#include <QIODevice>

#include "include/BeanDefinitionReader/impl/McXmlBeanDefinitionReader.h"
#include "include/PropertyParser/impl/McDefaultPropertyParser.h"

MC_BEGIN_NAMESPACE

McXmlApplicationContext::McXmlApplicationContext(QObject *parent)
    : McReadableApplicationContext(parent)
{
}

McXmlApplicationContext::McXmlApplicationContext(
        QIODeviceConstPtrRef device
        , QObject *parent)
    : McXmlApplicationContext(QList<QIODevicePtr>() << device, parent)
{
}

McXmlApplicationContext::McXmlApplicationContext(
        const QList<QIODevicePtr> &devices
        , QObject *parent)
    : McXmlApplicationContext(parent)
{
    setDevices(devices);
}

McXmlApplicationContext::McXmlApplicationContext(
        IMcBeanDefinitionReaderConstPtrRef reader
        , QObject *parent)
    : McReadableApplicationContext(reader, parent)
{
}

McXmlApplicationContext::McXmlApplicationContext(
        IMcConfigurableBeanFactoryConstPtrRef factory
        , IMcBeanDefinitionReaderConstPtrRef reader
        , QObject *parent)
    : McReadableApplicationContext(factory, reader, parent)
{
}

McXmlApplicationContext::~McXmlApplicationContext(){
}

void McXmlApplicationContext::setDevice(QIODeviceConstPtrRef device) noexcept {
    setDevices(QList<QIODevicePtr>() << device);
}

void McXmlApplicationContext::setDevices(const QList<QIODevicePtr> &devices) noexcept {
    setReader(McXmlBeanDefinitionReaderPtr::create(
                  McDefaultPropertyParserPtr::create(), devices));
}

MC_END_NAMESPACE
