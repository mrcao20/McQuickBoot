#include "McOrm/SqlSlot/impl/McSelectSqlSlot.h"

#include <QMetaMethod>
#include <QSqlQuery>
#include <QDebug>

#include <McIoc/BeanFactory/impl/McMetaTypeId.h>

#include "McOrm/McOrmMacroGlobal.h"

McSelectSqlSlot::McSelectSqlSlot(QObject *parent) noexcept
    : McAbstractSqlSlot(parent)
{
}

void McSelectSqlSlot::query(void **args, const QMetaMethod &sig, const QByteArray &additionalInfo) noexcept
{
    if(!additionalInfo.isEmpty()) {
        qDebug() << "additionalInfo is not empty";
        return;
    }
    auto returnType = sig.returnType();
    if(isPo(returnType)) {
        QVariant returnVar = queryForReturn(args, sig);
        QMetaType::convert(returnVar.data(), returnVar.userType(), args[0], returnType);
    } else {
        Q_ASSERT(args[1] != nullptr);
        QVariant var(sig.parameterType(0), args[1]);
        if(var.canConvert<QList<QVariant>>()) {
            auto list = var.value<QList<QVariant>>();
            queryForParam(list);
        } else if(var.canConvert<QObjectPtr>()) {
            auto obj = var.value<QObjectPtr>();
            queryForParam(obj);
        }
    }
}

QVariant McSelectSqlSlot::queryForReturn(void **args, const QMetaMethod &sig) noexcept
{
    QList<QVariant> params;
    //! 由于args是由sig对应的方法的参数列表构造出的
    //! 所以args的长度减一一定和method的parameterCount数量一致
    for(int i = 0; i < sig.parameterCount(); ++i) {
        QVariant param(sig.parameterType(i), args[i + 1]);
        params.append(param);
    }
    
    auto returnType = sig.returnType();
    auto mo = getMetaObject(returnType);
    auto paramNames = sig.parameterNames();
    //! QMap<property, column>
    auto proColMap = getProColMap(mo);
    QSqlQuery sqlQuery;
    QString sql = "SELECT";
    for(auto value : proColMap.values()) {
        sql += " `";
        sql += value;
        sql += "`,";
    }
    sql.remove(sql.length() - 1, 1);
    
    sql += " FROM `";
    sql += mo->classInfo(mo->indexOfClassInfo(MC_DB_TABLE_TAG)).value();
    sql += "`";
    
    if(!paramNames.isEmpty()) {
        sql += " WHERE";
        for(auto name : paramNames) {
            sql += " `";
            sql += proColMap.value(name);
            sql += "` = ? AND";
        }
        sql.remove(sql.length() - 4, 4);
    }
    sqlQuery.prepare(sql);
    for(auto param : params) {
        sqlQuery.addBindValue(param);
    }
    sqlQuery.exec();
    auto sharedPointer = McMetaTypeId::sharedPointerIds();
    if(sharedPointer.contains(returnType)) {
        QObjectPtr objPtr(mo->newInstance());
        sqlQuery.next();
        for(auto key : proColMap.keys()) {
            objPtr->setProperty(key.toLocal8Bit(), sqlQuery.value(proColMap.value(key)));
        }
        QVariant var = QVariant::fromValue(objPtr);
        return var;
    } else {
        QList<QVariant> list;
        while(sqlQuery.next()) {
            QObjectPtr objPtr(mo->newInstance());
            for(auto key : proColMap.keys()) {
                objPtr->setProperty(key.toLocal8Bit(), sqlQuery.value(proColMap.value(key)));
            }
            list.append(QVariant::fromValue(objPtr));
        }
        return list;
    }
    
    return QVariant();
}

void McSelectSqlSlot::queryForParam(QObjectConstPtrRef po) noexcept
{
    qDebug() << po;
}

void McSelectSqlSlot::queryForParam(const QVariantList &poVars) noexcept
{
    qDebug() << poVars;
}

QMap<QString, QString> McSelectSqlSlot::getProColMap(const QMetaObject *mo) noexcept
{
    QMap<QString, QString> map;
    for(int i = 0; i < mo->classInfoCount(); ++i) {
        auto classInfo = mo->classInfo(i);
        if(qstrcmp(classInfo.name(), MC_TABLE_COL_TAG) != 0) {
            continue;
        }
        QString value = classInfo.value();
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
        auto list = value.split('=', QString::SkipEmptyParts);
#else
        auto list = value.split('=', Qt::SkipEmptyParts);
#endif
        if(list.length() != 2) {
            continue;
        }
        map.insert(list.first().simplified(), list.last().simplified());
    }
    for(int i = QObject::staticMetaObject.propertyCount(); i < mo->propertyCount(); ++i) {
        auto pro = mo->property(i);
        if(map.contains(pro.name())) {
            continue;
        }
        map.insert(pro.name(), pro.name());
    }
    return map;
}
