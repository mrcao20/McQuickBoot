#pragma once

#include "McAbstractBeanDefinitionReader.h"

QT_BEGIN_NAMESPACE
MC_FORWARD_DECL_CLASS(QIODevice);
class QDomDocument;
class QDomNodeList;
class QDomElement;
QT_END_NAMESPACE

MC_FORWARD_DECL_CLASS(IMcBeanDefinition)
MC_FORWARD_DECL_CLASS(IMcPropertyParser)

MC_FORWARD_DECL_PRIVATE_DATA(McXmlBeanDefinitionReader)

class MCIOCCONTAINER_EXPORT McXmlBeanDefinitionReader : public McAbstractBeanDefinitionReader 
{
    Q_OBJECT
    using McAbstractBeanDefinitionReader::readBeanDefinition;
public:
    explicit McXmlBeanDefinitionReader(
        IMcPropertyParserConstPtrRef parser
        , QIODeviceConstPtrRef device, QObject *parent = nullptr);
    explicit McXmlBeanDefinitionReader(
        IMcPropertyParserConstPtrRef parser
        , const QList<QIODevicePtr> &devices, QObject *parent = nullptr);
    virtual ~McXmlBeanDefinitionReader() override;

protected:
    void doReadBeanDefinition() noexcept override;

private:
    void readBeanDefinition(QIODeviceConstPtrRef source) noexcept;
    void readBeanDefinition(const QDomDocument &doc) noexcept;
    void readBeanDefinition(const QDomNodeList &nodes) noexcept;
    void readBeanDefinition(const QDomNodeList &propNodes
                            , IMcBeanDefinitionConstPtrRef beanDefinition) noexcept;
    void readBeanDefinitionForProperty(const QDomElement &propEle
                                       , IMcBeanDefinitionConstPtrRef beanDefinition) noexcept;
    void readBeanDefinitionForConnect(const QDomElement &propEle
                                       , IMcBeanDefinitionConstPtrRef beanDefinition) noexcept;
    Qt::ConnectionType getConnectionType(const QString &typeStr) noexcept;
    Qt::ConnectionType connectionTypeStrToEnum(const QString &typeStr) noexcept;

private:
	MC_DECL_PRIVATE(McXmlBeanDefinitionReader)
};

MC_DECL_POINTER(McXmlBeanDefinitionReader)
