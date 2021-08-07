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
#include "McIoc/PropertyParser/impl/McDefaultPropertyParser.h"

#include <QCoreApplication>
#include <QDomElement>
#include <QLibrary>
#include <QDir>
#include <QDebug>

#include "McIoc/BeanFactory/impl/McBeanEnum.h"
#include "McIoc/BeanFactory/impl/McBeanPlaceholder.h"
#include "McIoc/BeanFactory/impl/McBeanReference.h"

McDefaultPropertyParser::McDefaultPropertyParser(QObject *parent)
    : McAbstractPropertyParser(parent)
{
}

QVariant McDefaultPropertyParser::parseValue(const QDomElement &ele) const noexcept 
{
    if (ele.hasAttribute("value")) {
        return ele.attribute("value");
    } else {
        return ele.text();
    }
}

QVariant McDefaultPropertyParser::parseRef(const QDomElement &ele) const noexcept 
{
    McBeanReferencePtr ref = McBeanReferencePtr::create();
    if (ele.hasAttribute("ref")) {
        ref->setName(ele.attribute("ref"));
    } else if (ele.hasAttribute("bean")) {
        ref->setName(ele.attribute("bean"));
    } else {
        ref->setName(ele.text());
    }
    return QVariant::fromValue(ref);
}

QVariant McDefaultPropertyParser::parseList(const QDomElement &ele) const noexcept 
{
    QVariantList list;

    if (ele.hasAttribute("plugins")) {
        QString pluginsPath = ele.attribute("plugins").simplified();
        pluginsPath = Mc::toAbsolutePath(pluginsPath);
        list = getList(pluginsPath, ele.attribute("isPointer", "false") == "true");
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
    if (ele.hasAttribute("plh")) {
        auto plh = ele.attribute("plh");
        for (int i = 0; i < childNodes.length(); ++i) {
            auto ele = childNodes.at(i).toElement();
            if (ele.isNull() || ele.tagName() != "list")
                continue;
            auto list = parse(ele).toList();
            for (auto value : list) {
                if (value.userType() != qMetaTypeId<McBeanReferencePtr>()) {
                    qCritical() << "if you want to used plh in map tag."
                                   "please make sure the value be ref tag.";
                    continue;
                }
                McBeanPlaceholderPtr beanPlh = McBeanPlaceholderPtr::create();
                beanPlh->setPlaceholder(plh);
                map.insert(QVariant::fromValue(beanPlh), value);
            }
            break;
        }
    } else {
        for (int i = 0; i < childNodes.length(); ++i) {
            auto ele = childNodes.at(i).toElement();
            if (ele.isNull() || ele.tagName() != "entry")
                continue;
            QVariant mapKey, mapValue;
            if (ele.hasAttribute("key")) {
                mapKey = ele.attribute("key");
            }
            if (ele.hasAttribute("value")) {
                mapValue = ele.attribute("value");
            }

            auto entryChildEle = ele.firstChildElement();
            while (!entryChildEle.isNull()) {
                if (entryChildEle.tagName() == "key") {
                    auto keyChildEle = entryChildEle.firstChildElement();
                    if (keyChildEle.isNull()) {
                        mapKey = entryChildEle.text();
                    } else if (keyChildEle.tagName() == "plh") {
                        McBeanPlaceholderPtr beanPlh = McBeanPlaceholderPtr::create();
                        beanPlh->setPlaceholder(keyChildEle.text());
                        mapKey = QVariant::fromValue(beanPlh);
                    } else {
                        //! 递归解析
                        mapKey = parse(keyChildEle);
                    }
                } else if (entryChildEle.tagName() == "value") {
                    auto valueChildEle = entryChildEle.firstChildElement();
                    if (valueChildEle.isNull()) {
                        mapValue = entryChildEle.text();
                    } else {
                        //! 递归解析
                        mapValue = parse(valueChildEle);
                    }
                }

                entryChildEle = entryChildEle.nextSiblingElement();
            }

            if (mapKey.isValid() && mapValue.isValid()) {
                map.insert(mapKey, mapValue);
            }
        }
    }

    return QVariant::fromValue(map);
}

QVariant McDefaultPropertyParser::parseEnum(const QDomElement &ele) const noexcept
{
    McBeanEnumPtr e = McBeanEnumPtr::create();
    if (ele.hasAttribute("scope")) {
        e->setScope(ele.attribute("scope"));
    }
    if (ele.hasAttribute("type")) {
        e->setType(ele.attribute("type"));
    }
    if (ele.hasAttribute("value")) {
        e->setValue(ele.attribute("value"));
    }
    auto childNodes = ele.childNodes();
    for(int i = 0; i < childNodes.length(); ++i) {
        auto childNode = childNodes.at(i);
        auto childEle = childNode.toElement();
        if(childEle.isNull()) {
            continue;
        }
        if (childEle.tagName() == "scope") {
            if (childEle.hasAttribute("name")) {
                e->setScope(childEle.attribute("name"));
            }
            auto text = childEle.text().simplified();
            if(!text.isEmpty()) {
                e->setScope(text);
            }
        } else if (childEle.tagName() == "type") {
            if (childEle.hasAttribute("name")) {
                e->setType(childEle.attribute("name"));
            }
            auto text = childEle.text().simplified();
            if(!text.isEmpty()) {
                e->setType(text);
            }
        } else if (childEle.tagName() == "value") {
            if (childEle.hasAttribute("name")) {
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

QVariantList McDefaultPropertyParser::getList(const QString &dirPath, bool isPointer) const noexcept
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
        beanRef->setPointer(isPointer);
        list << QVariant::fromValue(beanRef);
    }
    
    return list;
}
