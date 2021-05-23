#include <McWidgetIoc/ApplicationContext/Impl/McXmlWidgetApplicationContext.h>

#include <QFile>

#include <McIoc/BeanDefinitionReader/impl/McXmlBeanDefinitionReader.h>
#include <McIoc/BeanFactory/impl/McPointerBeanFactory.h>
#include <McIoc/BeanFactory/impl/McSharedBeanFactory.h>
#include <McIoc/PropertyParser/impl/McDefaultPropertyConverter.h>
#include <McIoc/PropertyParser/impl/McDefaultPropertyParser.h>

#include "McWidgetIoc/BeanFactory/Impl/McDefaultWidgetBeanFactory.h"

MC_DECL_PRIVATE_DATA(McXmlWidgetApplicationContext)
QList<IMcBeanFactoryPtr> factorys;
MC_DECL_PRIVATE_DATA_END

McXmlWidgetApplicationContext::McXmlWidgetApplicationContext(QObject *parent)
    : McAbstractWidgetApplicationContext(parent)
{
    MC_NEW_PRIVATE_DATA(McXmlWidgetApplicationContext);

    auto sharedFactory = McSharedBeanFactoryPtr::create();
    auto pointerFactory = McPointerBeanFactoryPtr::create();
    auto factory = McDefaultWidgetBeanFactoryPtr::create();
    auto converter = McDefaultPropertyConverterPtr::create();
    converter->addReferenceResolver(sharedFactory.data());
    converter->addReferenceResolver(pointerFactory.data());
    sharedFactory->setPropertyConverter(converter);
    pointerFactory->setPropertyConverter(converter);
    factory->setConverter(converter);
    setBeanFactory(factory);
    d->factorys.append(sharedFactory);
    d->factorys.append(pointerFactory);
}

McXmlWidgetApplicationContext::McXmlWidgetApplicationContext(QIODeviceConstPtrRef device,
                                                             const QString &flag,
                                                             QObject *parent)
    : McXmlWidgetApplicationContext(QList<QIODevicePtr>() << device, flag, parent)
{}

McXmlWidgetApplicationContext::McXmlWidgetApplicationContext(const QList<QIODevicePtr> &devices,
                                                             const QString &flag,
                                                             QObject *parent)
    : McXmlWidgetApplicationContext(parent)
{
    setDevices(devices, flag);
}

McXmlWidgetApplicationContext::McXmlWidgetApplicationContext(const QString &location,
                                                             const QString &flag,
                                                             QObject *parent)
    : McXmlWidgetApplicationContext(QStringList() << location, flag, parent)
{}

McXmlWidgetApplicationContext::McXmlWidgetApplicationContext(const QStringList &locations,
                                                             const QString &flag,
                                                             QObject *parent)
    : McXmlWidgetApplicationContext(parent)
{
    QList<QIODevicePtr> devices;
    for (auto location : locations) {
        auto filePath = Mc::toAbsolutePath(location);
        if (!QFile::exists(filePath)) {
            qCCritical(mcWidgetIoc()) << "file '" << filePath << "' not exists";
            continue;
        }
        auto device = new QFile(filePath);
        if (!device->open(QIODevice::ReadOnly | QIODevice::Text)) {
            qCCritical(mcWidgetIoc())
                << "open file '" << filePath << "' failure."
                << "error code:" << device->error() << "error msg:" << device->errorString();
            device->deleteLater();
            continue;
        }
        devices.append(QIODevicePtr(device));
    }
    setDevices(devices, flag);
}

McXmlWidgetApplicationContext::~McXmlWidgetApplicationContext() {}

void McXmlWidgetApplicationContext::setDevices(const QList<QIODevicePtr> &devices,
                                               const QString &flag) noexcept
{
    IMcBeanDefinitionReaderPtr reader
        = McXmlBeanDefinitionReaderPtr::create(McDefaultPropertyParserPtr::create(), devices, flag);
    reader->readBeanDefinition(this);
}
