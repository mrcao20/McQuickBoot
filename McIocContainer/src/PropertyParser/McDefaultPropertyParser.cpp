#include "include/PropertyParser/impl/McDefaultPropertyParser.h"

#include <QCoreApplication>
#include <QDomElement>
#include <QLibrary>
#include <QDir>
#include <QDebug>

#include "include/BeanFactory/impl/McBeanReference.h"

MC_BEGIN_NAMESPACE

McDefaultPropertyParser::McDefaultPropertyParser(QObject *parent)
    : McAbstractPropertyParser(parent)
{
}

QVariant McDefaultPropertyParser::parseValue(const QDomElement &ele) const noexcept {
    if(ele.hasAttribute("value")) {
        return ele.attribute("value");
    }else{
        return ele.text();
    }
}

QVariant McDefaultPropertyParser::parseRef(const QDomElement &ele) const noexcept {
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

QVariant McDefaultPropertyParser::parseList(const QDomElement &ele) const noexcept {
    QVariantList list;
    
    if(ele.hasAttribute("plugins")) {
        QString pluginsPath = ele.attribute("plugins").simplified();
        pluginsPath = QDir::toNativeSeparators(pluginsPath);
        if(pluginsPath.startsWith(QString("%1%2").arg(".", QDir::separator()))) {
            pluginsPath = pluginsPath.remove(0, 1);   // 移除最前面的.
            pluginsPath = qApp->applicationDirPath() + pluginsPath;   // 补全为全路径
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

QVariant McDefaultPropertyParser::parseMap(const QDomElement &ele) const noexcept {
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
                    mapKey = parse(valueChildEle);
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

QVariantList McDefaultPropertyParser::getList(const QString &dirPath) const noexcept {
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

MC_END_NAMESPACE
