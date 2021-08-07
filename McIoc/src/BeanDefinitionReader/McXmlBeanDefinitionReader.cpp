/*
 * MIT License
 *
 * Copyright (c) 2021 mrcao20
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
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
QString flag;
MC_DECL_PRIVATE_DATA_END

McXmlBeanDefinitionReader::McXmlBeanDefinitionReader(IMcPropertyParserConstPtrRef parser,
                                                     QIODeviceConstPtrRef device,
                                                     const QString &flag,
                                                     QObject *parent)
    : McXmlBeanDefinitionReader(parser, QList<QIODevicePtr>() << device, flag, parent)
{
}

McXmlBeanDefinitionReader::McXmlBeanDefinitionReader(IMcPropertyParserConstPtrRef parser,
                                                     const QList<QIODevicePtr> &devices,
                                                     const QString &flag,
                                                     QObject *parent)
    : McAbstractBeanDefinitionReader(parent)
{
    MC_NEW_PRIVATE_DATA(McXmlBeanDefinitionReader);
    
    d->parser = parser;
    d->devices = devices;
    d->flag = flag;
}

McXmlBeanDefinitionReader::~McXmlBeanDefinitionReader()
{
}

void McXmlBeanDefinitionReader::doReadBeanDefinition() noexcept 
{
    for (auto device : qAsConst(d->devices)) {
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
    if (root.tagName() != Mc::Constant::Tag::Xml::beans) {
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
        if (ele.tagName() != Mc::Constant::Tag::Xml::bean || ele.attribute("name").isEmpty()
            || (ele.attribute("class").isEmpty() && ele.attribute("plugin").isEmpty())) {
            qCritical() << "node name must be 'bean', and it's not only contains attribute 'name' and 'class/plugin' but also this attribute not is able null!!";
            continue;
        }
        if (ele.hasAttribute(Mc::Constant::Tag::Xml::flag)
            && ele.attribute(Mc::Constant::Tag::Xml::flag) != d->flag) {
            continue;
        }
        //! 获取给定元素的 name 属性
        QString name = ele.attribute("name");
        if (registry()->isContained(name)) {
            continue; //!< 如果存在则不再处理
        }
        bool isSingleton = !ele.hasAttribute("isSingleton")
                           || ele.attribute("isSingleton") == "true";
        //! 创建一个bean定义对象
        McRootBeanDefinitionPtr beanDefinition = McRootBeanDefinitionPtr::create();
        beanDefinition->setSingleton(isSingleton);
        if(!parseBeanClass(ele, beanDefinition)) {
            return;
        }
        readBeanDefinition(ele.childNodes(), beanDefinition);
        registry()->registerBeanDefinition(name, beanDefinition);
    }
}

bool McXmlBeanDefinitionReader::parseBeanClass(const QDomElement &ele, IMcBeanDefinitionConstPtrRef beanDefinition) noexcept
{
    if (ele.hasAttribute("class")) { //!< 如果指定的class，则通过class创建对象
        //! 设置bean 定义对象的 全限定类名
        beanDefinition->setClassName(ele.attribute("class"));
        beanDefinition->setPointer(ele.attribute("isPointer", "false") == "true");
    } else if (ele.hasAttribute("plugin")) { //!< 如果指定的是plugin，则通过插件创建对象
        QString pluginPath = ele.attribute("plugin");
        pluginPath = Mc::toAbsolutePath(pluginPath);
        if(!QLibrary::isLibrary(pluginPath)){
            qCritical() << pluginPath << "is not a plugin. please check!!!";
            return false;
        }
        beanDefinition->setPluginPath(pluginPath);
        beanDefinition->setSingleton(true);     //!< 插件必须是单例
        beanDefinition->setPointer(ele.attribute("isPointer", "true") == "true");
    } else {
        qCritical() << "bean must be class or plugin, please check!!!";
        return false;
    }
    return true;
}

void McXmlBeanDefinitionReader::readBeanDefinition(
        const QDomNodeList &propNodes,
        IMcBeanDefinitionConstPtrRef beanDefinition) noexcept 
{
    for (int i = 0; i < propNodes.size(); ++i) {
        QDomElement propEle = propNodes.at(i).toElement();
        if (propEle.isNull())
            continue;
        if (propEle.hasAttribute(Mc::Constant::Tag::Xml::flag)
            && propEle.attribute(Mc::Constant::Tag::Xml::flag) != d->flag) {
            continue;
        }
        if (propEle.tagName() == Mc::Constant::Tag::Xml::connect) {
            readBeanDefinitionForConnect(propEle, beanDefinition);
        } else { //! 不判定标签是否为property，即除了connect以外均可以解析为property
            readBeanDefinitionForProperty(propEle, beanDefinition);
        }
    }
}

void McXmlBeanDefinitionReader::readBeanDefinitionForProperty(
        const QDomElement &propEle,
        IMcBeanDefinitionConstPtrRef beanDefinition) noexcept 
{
    //! 获取给定元素的 name 属性
    QString propName = propEle.attribute("name");
    if (propName.isEmpty()) {
        qCritical() << "property name not be able null!!";
        return;
    }
    
    QVariant value;
    QDomElement childEle;
    if ((childEle = propEle).tagName() == "bean"
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
        if (childEle.hasAttribute("isSingleton")) {
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
        const QDomElement &propEle,
        IMcBeanDefinitionConstPtrRef beanDefinition) noexcept 
{
    McBeanConnectorPtr connector = McBeanConnectorPtr::create();
    connector->setSender(Mc::Constant::Tag::Xml::self); //!< 如果没有指定sender，则默认为对象本身
    connector->setReceiver(Mc::Constant::Tag::Xml::self); //!< 如果没有指定receiver，则默认为对象本身
    connector->setType(Qt::ConnectionType::AutoConnection); //!< 默认为自动连接
    
    // connect attribute
    if (propEle.hasAttribute(Mc::Constant::Tag::Xml::sender)) {
        connector->setSender(propEle.attribute(Mc::Constant::Tag::Xml::sender));
    }
    if (propEle.hasAttribute(Mc::Constant::Tag::Xml::signal)) {
        connector->setSignal(propEle.attribute(Mc::Constant::Tag::Xml::signal));
    }
    if (propEle.hasAttribute(Mc::Constant::Tag::Xml::receiver)) {
        connector->setReceiver(propEle.attribute(Mc::Constant::Tag::Xml::receiver));
    }
    if (propEle.hasAttribute(Mc::Constant::Tag::Xml::slot)) {
        connector->setSlot(propEle.attribute(Mc::Constant::Tag::Xml::slot));
    }
    if (propEle.hasAttribute(Mc::Constant::Tag::Xml::type)) {
        QString typeStr = propEle.attribute(Mc::Constant::Tag::Xml::type);
        connector->setType(getConnectionType(typeStr));
    }

    // connect childNodes attribute and text
    auto childEle = propEle.firstChildElement();
    while(!childEle.isNull()) {
        if (childEle.tagName() == Mc::Constant::Tag::Xml::sender) {
            auto text = connector->getSender();
            if (childEle.hasAttribute("name")) {
                text = childEle.attribute("name");
            }
            if(!childEle.text().isEmpty()) {
                text = childEle.text();
            }
            connector->setSender(text);
        } else if (childEle.tagName() == Mc::Constant::Tag::Xml::signal) {
            auto text = connector->getSignal();
            if (childEle.hasAttribute("name")) {
                text = childEle.attribute("name");
            }
            if(!childEle.text().isEmpty()) {
                text = childEle.text();
            }
            connector->setSignal(text);
        } else if (childEle.tagName() == Mc::Constant::Tag::Xml::receiver) {
            auto text = connector->getReceiver();
            if (childEle.hasAttribute("name")) {
                text = childEle.attribute("name");
            }
            if(!childEle.text().isEmpty()) {
                text = childEle.text();
            }
            connector->setReceiver(text);
        } else if (childEle.tagName() == Mc::Constant::Tag::Xml::slot) {
            auto text = connector->getSlot();
            if (childEle.hasAttribute("name")) {
                text = childEle.attribute("name");
            }
            if(!childEle.text().isEmpty()) {
                text = childEle.text();
            }
            connector->setSlot(text);
        } else if (childEle.tagName() == Mc::Constant::Tag::Xml::type) {
            auto text = connector->getType();
            if (childEle.hasAttribute("name")) {
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
