#include "McIoc/PropertyParser/impl/McDefaultPropertyConverter.h"

#include <QDebug>

#include "McIoc/BeanFactory/impl/McBeanReference.h"
#include "McIoc/BeanFactory/IMcBeanReferenceResolver.h"

McDefaultPropertyConverter::McDefaultPropertyConverter(QObject *parent)
    : McAbstarctPropertyConverter(parent)
{
}

QVariant McDefaultPropertyConverter::convertRef(const QVariant &value) const noexcept {
    
    auto ref = value.value<McBeanReferencePtr>();
    if (!ref) {	//!< 判断是否能够成功转换
		//! 失败，记录错误信息
		qCritical() << "cannot inject beanReference";
		return QVariant();
	}
    //! 调用IMcBeanReferenceResolver的resolveBeanReferenceToQVariant方法，根据bean获取实例，此处即是递归
    auto objVar = resolver()->resolveBeanReferenceToQVariant(ref);
    if (!objVar.isValid()) {
		//! 失败，记录错误信息
		qCritical() << "cannot get bean from beanReference";
		return QVariant();
	}
    return objVar;
}

QVariant McDefaultPropertyConverter::convertList(const QVariant &value) const noexcept {
    QVariantList result;
    
    auto list = value.value<QVariantList>();
    
    for(const auto &var : list) {
        result << convert(resolver(), var);
    }
    
    return result;
}

QVariant McDefaultPropertyConverter::convertMap(const QVariant &value) const noexcept {
    QMap<QVariant, QVariant> result;
    
    auto map = value.value<QMap<QVariant, QVariant>>();
    
    QMapIterator<decltype (map)::key_type, decltype (map)::mapped_type> itr(map);
    while(itr.hasNext()) {
        auto item = itr.next();
        auto key = item.key();
        auto value = item.value();
        
        QVariant resultKey, resultValue;
        
        resultKey = convert(resolver(), key);
        resultValue = convert(resolver(), value);
        
        if(resultKey.isValid() && resultValue.isValid()) {
            result.insert(resultKey, resultValue);
        }
    }
    
    return QVariant::fromValue(result);
}
