#include "McIoc/PropertyParser/impl/McDefaultPropertyParser.h"

#include <QCoreApplication>
#include <QDomElement>
#include <QLibrary>
#include <QDir>
#include <QDebug>

#include "McIoc/BeanFactory/impl/McBeanReference.h"
#include "McIoc/BeanFactory/impl/McBeanEnum.h"

McDefaultPropertyParser::McDefaultPropertyParser(QObject *parent)
    : McAbstractPropertyParser(parent)
{
}

QVariant McDefaultPropertyParser::parseValue(const QDomElement &ele) const noexcept 
{
    if(ele.hasAttribute("value")) {
        return ele.attribute("value");
    }else{
        return ele.text();
    }
}

QVariant McDefaultPropertyParser::parseRef(const QDomElement &ele) const noexcept 
{
    McBeanReferencePtr ref = McBeanReferencePtr::create();
    if(ele.hasAttribute("ref")) {
        ref->setName(ele.attribute("ref"));
    }else if(ele.hasAttribute("bean")){
        ref->setName(ele.attribute("bean"));
    }else{
        ref->setName(ele.text());
    }
    return QVariant::fromValue(ref);
}

QVariant McDefaultPropertyParser::parseList(const QDomElement &ele) const noexcept 
{
    QVariantList list;
    
    if(ele.hasAttribute("plugins")) {
        QString pluginsPath = ele.attribute("plugins").simplified();
        pluginsPath = QDir::toNativeSeparators(pluginsPath);
        if(pluginsPath.startsWith(QString("%1%2").arg(".", QDir::separator()))
                || pluginsPath.startsWith(QString("%1%2").arg("..", QDir::separator()))) {
            pluginsPath = qApp->applicationDirPath() + "/" + pluginsPath;   //!< 补全为全路径
        }
        list = getList(pluginsPath);
    }
    
    auto childNodes = ele.childNodes();
    for(int i = 0; i < childNodes.size(); ++i) {
		auto childEle = childNodes.at(i).toElement();
        if (childEle.isNull()) {
            continue;
        }
        //! 递归解析
        auto value = parse(childEle);
        if(!value.isValid()) {
            qWarning("in list tag of '%s' cannot parse!!!\n", qPrintable(childEle.tagName()));
            continue;
        }
        list << value;
	}
    
    return list;
}

QVariant McDefaultPropertyParser::parseMap(const QDomElement &ele) const noexcept 
{
    QMap<QVariant, QVariant> map;
    
	auto childNodes = ele.childNodes();
	for (int i = 0; i < childNodes.size(); ++i) {
		auto ele = childNodes.at(i).toElement();
		if (ele.isNull() || ele.tagName() != "entry")
			continue;
        QVariant mapKey, mapValue;
        if(ele.hasAttribute("key")) {
            mapKey = ele.attribute("key");
        }
        if(ele.hasAttribute("value")) {
            mapValue = ele.attribute("value");
        }
        
        auto entryChildEle = ele.firstChildElement();
        while(!entryChildEle.isNull()) {
            if(entryChildEle.tagName() == "key") {
                auto keyChildEle = entryChildEle.firstChildElement();
                if(keyChildEle.isNull()) {
                    mapKey = entryChildEle.text();
                }else{
                    //! 递归解析
                    mapKey = parse(keyChildEle);
                }
            }else if(entryChildEle.tagName() == "value") {
                auto valueChildEle = entryChildEle.firstChildElement();
                if(valueChildEle.isNull()) {
                    mapValue = entryChildEle.text();
                }else{
                    //! 递归解析
                    mapValue = parse(valueChildEle);
                }
            }
            
            entryChildEle = entryChildEle.nextSiblingElement();
        }
        
        if(mapKey.isValid() && mapValue.isValid()) {
            map.insert(mapKey, mapValue);
        }
	}
    
    return QVariant::fromValue(map);
}

QVariant McDefaultPropertyParser::parseEnum(const QDomElement &ele) const noexcept
{
    McBeanEnumPtr e = McBeanEnumPtr::create();
    if(ele.hasAttribute("scope")) {
        e->setScope(ele.attribute("scope"));
    }
    if(ele.hasAttribute("type")) {
        e->setType(ele.attribute("type"));
    }
    if(ele.hasAttribute("value")) {
        e->setValue(ele.attribute("value"));
    }
    auto childNodes = ele.childNodes();
    for(int i = 0; i < childNodes.length(); ++i) {
        auto childNode = childNodes.at(i);
        auto childEle = childNode.toElement();
        if(childEle.isNull()) {
            continue;
        }
        if(childEle.tagName() == "scope") {
            if(childEle.hasAttribute("name")) {
                e->setScope(childEle.attribute("name"));
            }
            auto text = childEle.text().simplified();
            if(!text.isEmpty()) {
                e->setScope(text);
            }
        } else if(childEle.tagName() == "type") {
            if(childEle.hasAttribute("name")) {
                e->setType(childEle.attribute("name"));
            }
            auto text = childEle.text().simplified();
            if(!text.isEmpty()) {
                e->setType(text);
            }
        } else if(childEle.tagName() == "value") {
            if(childEle.hasAttribute("name")) {
                e->setValue(childEle.attribute("name"));
            }
            auto text = childEle.text().simplified();
            if(!text.isEmpty()) {
                e->setValue(text);
            }
        }
    }
    return QVariant::fromValue(e);
}

QVariantList McDefaultPropertyParser::getList(const QString &dirPath) const noexcept 
{
    QVariantList list;
    
    QDir dir(dirPath);
    if(!dir.exists()) {
        qCritical() << dirPath << "not exists";
        return list;
    }
    QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files);
    for(auto fileInfo : fileInfoList) {
        QString pluginPath = fileInfo.absoluteFilePath();
        if(!QLibrary::isLibrary(pluginPath)) {
            qCritical() << pluginPath << "not a plugin";
            continue;
        }
        QSharedPointer<McBeanReference> beanRef = QSharedPointer<McBeanReference>::create();
        beanRef->setPluginPath(pluginPath);
		list << QVariant::fromValue(beanRef);
    }
    
    return list;
}
