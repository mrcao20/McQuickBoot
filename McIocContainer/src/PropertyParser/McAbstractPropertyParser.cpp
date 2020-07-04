#include "McIoc/PropertyParser/impl/McAbstractPropertyParser.h"

#include <QDomElement>
#include <QDebug>

McAbstractPropertyParser::McAbstractPropertyParser(QObject *parent)
    : QObject(parent)
{
}

QVariant McAbstractPropertyParser::parse(const QDomElement &ele) const noexcept 
{
    QDomElement childEle;
    if((childEle = ele).tagName() == "list"
             || !(childEle = ele.firstChildElement("list")).isNull()) {
        
        return parseList(childEle);
    } else if((childEle = ele).tagName() == "map"
             || !(childEle = ele.firstChildElement("map")).isNull()) {
        
        return parseMap(childEle);
    } else if((childEle = ele).tagName() == "value"
             || (childEle = ele).hasAttribute("value") 
             || !(childEle = ele.firstChildElement("value")).isNull()) {
         
         return parseValue(childEle);
    } else if((childEle = ele).tagName() == "ref"
             || (childEle = ele).hasAttribute("ref") 
             || !(childEle = ele.firstChildElement("ref")).isNull()) {
        
        return parseRef(childEle);
    } else{
        qWarning("the tag for '%s' cannot parse!!\n", qPrintable(ele.tagName()));
        return QVariant();
    }
}
