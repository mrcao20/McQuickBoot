#include "McIoc/PropertyParser/impl/McDefaultPropertyConverter.h"

#include <QMetaEnum>
#include <QDebug>

#include "McIoc/BeanDefinition/IMcCustomPlaceholder.h"
#include "McIoc/BeanFactory/IMcBeanReferenceResolver.h"
#include "McIoc/BeanFactory/impl/McBeanEnum.h"
#include "McIoc/BeanFactory/impl/McBeanPlaceholder.h"
#include "McIoc/BeanFactory/impl/McBeanReference.h"

McDefaultPropertyConverter::McDefaultPropertyConverter(QObject *parent)
    : McAbstarctPropertyConverter(parent)
{
}

QVariant McDefaultPropertyConverter::convertRef(const QVariant &value) const noexcept 
{
    
    auto ref = value.value<McBeanReferencePtr>();
    if (!ref) {    //!< 判断是否能够成功转换
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

QVariant McDefaultPropertyConverter::convertEnum(const QVariant &value) const noexcept
{
    auto e = value.value<McBeanEnumPtr>();
    if(e.isNull()) { //!< 判断是否能够转换
        qCritical() << "cannot inject enum";
        return QVariant();
    }
    const QMetaObject *mo = nullptr;
    if(e->scope() == "Qt") {
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
        mo = qt_getQtMetaObject();
#else
        mo = &staticQtMetaObject;
#endif
    } else {
        mo = QMetaType::metaObjectForType(QMetaType::type(qPrintable(e->scope())));
    }
    if(mo == nullptr) {
        qCritical("cannot get meta object for type: %s\n", qPrintable(e->scope()));
        return QVariant();
    }
    auto enumIndex = mo->indexOfEnumerator(qPrintable(e->type()));
    if(enumIndex == -1) {
        qCritical("not exists enum '%s' for type '%s'", qPrintable(e->type()), qPrintable(e->scope()));
        return QVariant();
    }
    auto me = mo->enumerator(enumIndex);
    return QVariant(me.keysToValue(qPrintable(e->value())));
}

QVariant McDefaultPropertyConverter::convertList(const QVariant &value) const noexcept 
{
    QVariantList result;
    
    auto list = value.value<QVariantList>();
    
    for(const auto &var : list) {
        result << convert(resolver(), var);
    }
    
    return result;
}

QVariant McDefaultPropertyConverter::convertMap(const QVariant &value) const noexcept 
{
    QMap<QVariant, QVariant> result;
    
    auto map = value.value<QMap<QVariant, QVariant>>();
    
    QMapIterator<decltype (map)::key_type, decltype (map)::mapped_type> itr(map);
    while(itr.hasNext()) {
        auto item = itr.next();
        auto key = item.key();
        auto value = item.value();

        QVariant resultKey, resultValue;

        resultValue = convert(resolver(), value);
        if (key.userType() == qMetaTypeId<McBeanPlaceholderPtr>()) {
            auto valueObj = resultValue.value<QObjectPtr>();
            auto keyPlh = key.value<McBeanPlaceholderPtr>();
            auto plh = keyPlh->getPlaceholder();
            if (plh == "className") {
                resultKey = valueObj->metaObject()->className();
            } else if (plh == "objectName") {
                resultKey = valueObj->objectName();
            } else if (plh == "custom") {
                auto customPlh = resultValue.value<IMcCustomPlaceholderPtr>();
                resultKey = customPlh->getKey();
            } else {
                qCritical() << "the plh must be one of className/objectName/custom";
            }
        } else {
            resultKey = convert(resolver(), key);
        }

        if (resultKey.isValid() && resultValue.isValid()) {
            result.insert(resultKey, resultValue);
        }
    }
    
    return QVariant::fromValue(result);
}
