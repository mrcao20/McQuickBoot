#include "McIoc/ApplicationContext/impl/McXmlApplicationContext.h"

#include <QIODevice>

#include "McIoc/BeanDefinitionReader/impl/McXmlBeanDefinitionReader.h"
#include "McIoc/PropertyParser/impl/McDefaultPropertyParser.h"

McXmlApplicationContext::McXmlApplicationContext(QObject *parent)
    : McReadableApplicationContext(parent)
{
}

McXmlApplicationContext::McXmlApplicationContext(QIODeviceConstPtrRef device,
                                                 const QString &flag,
                                                 QObject *parent)
    : McXmlApplicationContext(QList<QIODevicePtr>() << device, flag, parent)
{
}

McXmlApplicationContext::McXmlApplicationContext(const QList<QIODevicePtr> &devices,
                                                 const QString &flag,
                                                 QObject *parent)
    : McXmlApplicationContext(parent)
{
    setDevices(devices, flag);
}

McXmlApplicationContext::McXmlApplicationContext(IMcBeanDefinitionReaderConstPtrRef reader,
                                                 QObject *parent)
    : McReadableApplicationContext(reader, parent)
{
}

McXmlApplicationContext::McXmlApplicationContext(IMcConfigurableBeanFactoryConstPtrRef factory,
                                                 IMcBeanDefinitionReaderConstPtrRef reader,
                                                 QObject *parent)
    : McReadableApplicationContext(factory, reader, parent)
{
}

McXmlApplicationContext::~McXmlApplicationContext()
{
}

void McXmlApplicationContext::setDevice(QIODeviceConstPtrRef device, const QString &flag) noexcept
{
    setDevices(QList<QIODevicePtr>() << device, flag);
}

void McXmlApplicationContext::setDevices(const QList<QIODevicePtr> &devices,
                                         const QString &flag) noexcept
{
    setReader(
        McXmlBeanDefinitionReaderPtr::create(McDefaultPropertyParserPtr::create(), devices, flag));
}
