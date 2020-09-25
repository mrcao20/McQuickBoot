#include "McIoc/BeanDefinitionReader/impl/McXmlBeanDefinitionReader.h"

#include <QFile>
#include <QDir>
#include <qdom.h>
#include <QLibrary>
#include <QDebug>

#include "McIoc/BeanFactory/IMcBeanDefinitionRegistry.h"
#include "McIoc/BeanDefinition/impl/McRootBeanDefinition.h"
#include "McIoc/BeanFactory/impl/McBeanReference.h"
#include "McIoc/BeanFactory/impl/McBeanConnector.h"
#include "McIoc/PropertyParser/IMcPropertyParser.h"

MC_DECL_PRIVATE_DATA(McXmlBeanDefinitionReader)
IMcPropertyParserPtr parser;
QList<QIODevicePtr> devices;
MC_DECL_PRIVATE_DATA_END

McXmlBeanDefinitionReader::McXmlBeanDefinitionReader(
        IMcPropertyParserConstPtrRef parser
        , QIODeviceConstPtrRef device
        , QObject *parent)
    : McXmlBeanDefinitionReader(parser, QList<QIODevicePtr>() << device, parent)
{
}

McXmlBeanDefinitionReader::McXmlBeanDefinitionReader(
        IMcPropertyParserConstPtrRef parser
        , const QList<QIODevicePtr> &devices
        , QObject *parent)
    : McAbstractBeanDefinitionReader(parent)
{
    MC_NEW_PRIVATE_DATA(McXmlBeanDefinitionReader);
    
    d->parser = parser;
    d->devices = devices;
}

McXmlBeanDefinitionReader::~McXmlBeanDefinitionReader()
{
}

void McXmlBeanDefinitionReader::doReadBeanDefinition() noexcept 
{
    for(auto device : d->devices) {
        device->seek(0);
        readBeanDefinition(device);
    }
}

void McXmlBeanDefinitionReader::readBeanDefinition(QIODeviceConstPtrRef source) noexcept 
{
	QDomDocument doc;
	QString errorMsg;
	int errorLine;
	int errorCol;
	if (!doc.setContent(source.data(), &errorMsg, &errorLine, &errorCol)) {
        qCritical() << "error occured. Line:" << errorLine
                    << "Column:" << errorCol
                    << "Error msg:" << errorMsg;
		return;
	}
	readBeanDefinition(doc);
}

void McXmlBeanDefinitionReader::readBeanDefinition(const QDomDocument &doc) noexcept 
{
	QDomElement root = doc.documentElement();
	if (root.isNull()) {
		qCritical() << "XML Error: Root element is NULL.";
		return;
	}
	if (root.tagName() != MC_BEANS) {
		qCritical() << "XML Error: Root element is not beans.";
		return;
	}
	readBeanDefinition(root.childNodes());
}

void McXmlBeanDefinitionReader::readBeanDefinition(const QDomNodeList &nodes) noexcept 
{
	for (int i = 0; i < nodes.size(); ++i) {
		QDomElement ele = nodes.at(i).toElement();
        if(ele.isNull()) {
            continue;
        }
		if (ele.tagName() != MC_BEAN || ele.attribute("name").isEmpty() ||
			(ele.attribute("class").isEmpty() && ele.attribute("plugin").isEmpty())) {
			qCritical() << "node name must be 'bean', and it's not only contains attribute 'name' and 'class/plugin' but also this attribute not is able null!!";
			continue;
		}
		//! 获取给定元素的 name 属性
		QString name = ele.attribute("name");
        bool isSingleton = !ele.hasAttribute("isSingleton") 
                || ele.attribute("isSingleton") == "true";
        //! 创建一个bean定义对象
        McRootBeanDefinitionPtr beanDefinition = McRootBeanDefinitionPtr::create();
        beanDefinition->setSingleton(isSingleton);
        if(!parseBeanClass(ele, beanDefinition)) {
            return;
        }
		readBeanDefinition(ele.childNodes(), beanDefinition);
		//! 向注册容器 添加bean名称和bean定义. 如果存在则替换
		registry()->registerBeanDefinition(name, beanDefinition);
	}
}

bool McXmlBeanDefinitionReader::parseBeanClass(const QDomElement &ele, IMcBeanDefinitionConstPtrRef beanDefinition) noexcept
{
    if (ele.hasAttribute("class"))	//!< 如果指定的class，则通过class创建对象
        //! 设置bean 定义对象的 全限定类名
        beanDefinition->setClassName(ele.attribute("class"));
    else if(ele.hasAttribute("plugin")){    //!< 如果指定的是plugin，则通过插件创建对象
        QString pluginPath = ele.attribute("plugin");
        pluginPath = Mc::toAbsolutePath(pluginPath);
        if(!QLibrary::isLibrary(pluginPath)){
            qCritical() << pluginPath << "is not a plugin. please check!!!";
            return false;
        }
        beanDefinition->setPluginPath(pluginPath);
        beanDefinition->setSingleton(true);     //!< 插件必须是单例
    }else{
        qCritical() << "bean must be class or plugin, please check!!!";
        return false;
    }
    return true;
}

void McXmlBeanDefinitionReader::readBeanDefinition(
        const QDomNodeList &propNodes
        , IMcBeanDefinitionConstPtrRef beanDefinition) noexcept 
{
	for (int i = 0; i < propNodes.size(); ++i) {
		QDomElement propEle = propNodes.at(i).toElement();
		if (propEle.isNull())
			continue;
        if(propEle.tagName() == MC_CONNECT) {
            readBeanDefinitionForConnect(propEle, beanDefinition);
        }else{    //! 不判定标签是否为property，即除了connect以外均可以解析为property
            readBeanDefinitionForProperty(propEle, beanDefinition);
        }
	}
}

void McXmlBeanDefinitionReader::readBeanDefinitionForProperty(
        const QDomElement &propEle
        , IMcBeanDefinitionConstPtrRef beanDefinition) noexcept 
{
    //! 获取给定元素的 name 属性
    QString propName = propEle.attribute("name");
    if (propName.isEmpty()) {
        qCritical() << "property name not be able null!!";
        return;
    }
    
    QVariant value;
    QDomElement childEle;
    if((childEle = propEle).tagName() == "bean"
             || !(childEle = propEle.firstChildElement("bean")).isNull()) {
        
        McBeanReferencePtr ref = McBeanReferencePtr::create();
        QString beanName = childEle.attribute("name");
        if(beanName.isEmpty()) {
            QString className = beanDefinition->getClassName();
            if(className.isEmpty()) {
                QFileInfo fileInfo(beanDefinition->getPluginPath());
                className = fileInfo.baseName();
            }
            beanName = QString("__mc__%1_%2").arg(className, propName);
            auto tmpBeanName = beanName;
            int index = 1;
            while(registry()->isContained(tmpBeanName)) {
                tmpBeanName = beanName;
                tmpBeanName.append('_');
                tmpBeanName.append(QString::number(index++));
            }
            beanName = tmpBeanName;
        }
        bool isSingleton = beanDefinition->isSingleton();
        if(childEle.hasAttribute("isSingleton")) {
            isSingleton = childEle.attribute("isSingleton") == "true" ? true : false;
        }
        //! 创建子bean
        McRootBeanDefinitionPtr childBeanDefinition = McRootBeanDefinitionPtr::create();
        childBeanDefinition->setSingleton(isSingleton);
        if(!parseBeanClass(childEle, childBeanDefinition)) {
            return;
        }
		readBeanDefinition(childEle.childNodes(), childBeanDefinition);
		//! 向注册容器 添加bean名称和bean定义. 如果存在则替换
		registry()->registerBeanDefinition(beanName, childBeanDefinition);
        ref->setName(beanName);
        value.setValue(ref);
    } else {
        value = d->parser->parse(propEle);
    }
    if(value.isValid())
        beanDefinition->addProperty(propName, value);
    else
        qCritical() << QString("the named '%1' property's value is invalid!!").arg(propName);
}

void McXmlBeanDefinitionReader::readBeanDefinitionForConnect(
        const QDomElement &propEle
        , IMcBeanDefinitionConstPtrRef beanDefinition) noexcept 
{
    McBeanConnectorPtr connector = McBeanConnectorPtr::create();
    connector->setSender(MC_THIS);       //!< 如果没有指定sender，则默认为对象本身
    connector->setReceiver(MC_THIS);     //!< 如果没有指定receiver，则默认为对象本身
    connector->setType(Qt::ConnectionType::AutoConnection); //!< 默认为自动连接
    
    // connect attribute
    if(propEle.hasAttribute(MC_SENDER)) {
        connector->setSender(propEle.attribute(MC_SENDER));
    }
    if(propEle.hasAttribute(MC_SIGNAL)) {
        connector->setSignal(propEle.attribute(MC_SIGNAL));
    }
    if(propEle.hasAttribute(MC_RECEIVER)) {
        connector->setReceiver(propEle.attribute(MC_RECEIVER));
    }
    if(propEle.hasAttribute(MC_SLOT)) {
        connector->setSlot(propEle.attribute(MC_SLOT));
    }
    if(propEle.hasAttribute(MC_CONNECTION_TYPE)) {
        QString typeStr = propEle.attribute(MC_CONNECTION_TYPE);
        connector->setType(getConnectionType(typeStr));
    }
    
    // connect childNodes attribute and text
    auto childEle = propEle.firstChildElement();
    while(!childEle.isNull()) {
        if(childEle.tagName() == MC_SENDER) {
            auto text = connector->getSender();
            if(childEle.hasAttribute("name")) {
                text = childEle.attribute("name");
            }
            if(!childEle.text().isEmpty()) {
                text = childEle.text();
            }
            connector->setSender(text);
        }else if(childEle.tagName() == MC_SIGNAL) {
            auto text = connector->getSignal();
            if(childEle.hasAttribute("name")) {
                text = childEle.attribute("name");
            }
            if(!childEle.text().isEmpty()) {
                text = childEle.text();
            }
            connector->setSignal(text);
        }else if(childEle.tagName() == MC_RECEIVER) {
            auto text = connector->getReceiver();
            if(childEle.hasAttribute("name")) {
                text = childEle.attribute("name");
            }
            if(!childEle.text().isEmpty()) {
                text = childEle.text();
            }
            connector->setReceiver(text);
        }else if(childEle.tagName() == MC_SLOT) {
            auto text = connector->getSlot();
            if(childEle.hasAttribute("name")) {
                text = childEle.attribute("name");
            }
            if(!childEle.text().isEmpty()) {
                text = childEle.text();
            }
            connector->setSlot(text);
        }else if(childEle.tagName() == MC_CONNECTION_TYPE) {
            auto text = connector->getType();
            if(childEle.hasAttribute("name")) {
                text = getConnectionType(childEle.attribute("name"));
            }
            if(!childEle.text().isEmpty()) {
                text = getConnectionType(childEle.text());
            }
            connector->setType(text);
        }
        
        childEle = childEle.nextSiblingElement();
    }
    
    QVariant var;
    var.setValue(connector);
    beanDefinition->addConnector(var);
}

Qt::ConnectionType McXmlBeanDefinitionReader::getConnectionType(const QString &typeStr) noexcept 
{
    Qt::ConnectionType type = Qt::ConnectionType::AutoConnection;
    
#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
    QStringList typeList = typeStr.simplified().split('|', Qt::SkipEmptyParts);
#else
    QStringList typeList = typeStr.simplified().split('|', QString::SkipEmptyParts);
#endif
    
    if(typeList.size() > 2) {
        qCritical() << "请按照QObject::connect的要求设置ConnectionType";
        return type;
    }
    
    if(typeList.size() == 1) {
        auto temp = connectionTypeStrToEnum(typeList.at(0));
        if(temp == Qt::ConnectionType::UniqueConnection)
            type = static_cast<Qt::ConnectionType>(type | temp);
        else {
            type = temp;
        }
    } else if(typeList.size() == 2) {
        auto temp1 = connectionTypeStrToEnum(typeList.at(0));
        auto temp2 = connectionTypeStrToEnum(typeList.at(1));
        if(temp1 != Qt::ConnectionType::UniqueConnection
                && temp2 != Qt::ConnectionType::UniqueConnection) {
            qCritical() << "如果指定两种连接方式，则至少有一种必须为Qt::UniqueConnection";
        }else if(temp1 == Qt::ConnectionType::UniqueConnection
                 && temp2 == Qt::ConnectionType::UniqueConnection) {
            type = static_cast<Qt::ConnectionType>(type | temp1);
        }else {
            type = static_cast<Qt::ConnectionType>(temp1 | temp2);
        }
    }
    return type;
}

Qt::ConnectionType McXmlBeanDefinitionReader::connectionTypeStrToEnum(const QString &typeStr) noexcept 
{
    auto type = typeStr.simplified();
    if(type == "AutoConnection") {
        return Qt::ConnectionType::AutoConnection;
    }else if(type == "DirectConnection") {
        return Qt::ConnectionType::DirectConnection;
    }else if(type == "QueuedConnection") {
        return Qt::ConnectionType::QueuedConnection;
    }else if(type == "BlockingQueuedConnection") {
        return Qt::ConnectionType::BlockingQueuedConnection;
    }else {
        return Qt::ConnectionType::UniqueConnection;
    }
}
