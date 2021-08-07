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
    auto pns = sig.parameterNames();
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    QStringList paramNames;
    for (auto pn : pns) {
        paramNames.append(pn);
    }
#else
    QStringList paramNames(pns.constBegin(), pns.constEnd());
#endif
    return queryObjectFromDb(returnType, paramNames, params);
}

void McSelectSqlSlot::queryForParam(QObjectConstPtrRef po) noexcept
{
    qDebug() << po;
}

void McSelectSqlSlot::queryForParam(const QVariantList &poVars) noexcept
{
    qDebug() << poVars;
}

QVariant McSelectSqlSlot::queryObjectFromDb(int type,
                                            const QStringList &paramNames,
                                            const QVariantList &params) noexcept
{
    auto mo = getMetaObject(type);
    //! QMap<property, column>
    auto proColMap = getProColMap(mo);
    QSqlQuery sqlQuery;
    QString sql = "SELECT";
    for (auto value : proColMap.values()) {
        sql += " `";
        sql += value;
        sql += "`,";
    }
    sql.remove(sql.length() - 1, 1);

    sql += " FROM `";
    sql += mo->classInfo(mo->indexOfClassInfo(MC_DB_TABLE_TAG)).value();
    sql += "`";

    if (!paramNames.isEmpty()) {
        sql += " WHERE";
        for (auto name : paramNames) {
            sql += " `";
            sql += proColMap.value(name);
            sql += "` = ? AND";
        }
        sql.remove(sql.length() - 4, 4);
    }
    sqlQuery.prepare(sql);
    for (auto param : params) {
        sqlQuery.addBindValue(param);
    }
    sqlQuery.exec();
    auto sharedPointer = McMetaTypeId::sharedPointerIds();
    auto foreignKeys = getForeignKeyMap(mo);
    if (sharedPointer.contains(type)) {
        QObjectPtr objPtr(mo->newInstance());
        sqlQuery.next();
        for (auto key : proColMap.keys()) {
            QVariant value = sqlQuery.value(proColMap.value(key));
            if (foreignKeys.contains(key)) {
                auto pro = mo->property(mo->indexOfProperty(key.toLocal8Bit()));
                value = queryObjectFromDb(pro.userType(),
                                          QStringList() << foreignKeys.value(key),
                                          QVariantList() << value);
            }
            objPtr->setProperty(key.toLocal8Bit(), value);
        }
        QVariant var = QVariant::fromValue(objPtr);
        return var;
    } else {
        QList<QVariant> list;
        while (sqlQuery.next()) {
            QObjectPtr objPtr(mo->newInstance());
            for (auto key : proColMap.keys()) {
                QVariant value = sqlQuery.value(proColMap.value(key));
                if (foreignKeys.contains(key)) {
                    auto pro = mo->property(mo->indexOfProperty(key.toLocal8Bit()));
                    value = queryObjectFromDb(pro.userType(),
                                              QStringList() << foreignKeys.value(key),
                                              QVariantList() << value);
                }
                objPtr->setProperty(key.toLocal8Bit(), value);
            }
            list.append(QVariant::fromValue(objPtr));
        }
        return list;
    }
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
        auto list = value.split(MC_NORMAL_SPLIT_SYMBOL, QString::SkipEmptyParts);
#else
        auto list = value.split(MC_NORMAL_SPLIT_SYMBOL, Qt::SkipEmptyParts);
#endif
        Q_ASSERT(list.length() == 2);
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

QMap<QString, QString> McSelectSqlSlot::getForeignKeyMap(const QMetaObject *mo) noexcept
{
    QMap<QString, QString> map;
    for (int i = 0; i < mo->classInfoCount(); ++i) {
        auto classInfo = mo->classInfo(i);
        if (qstrcmp(classInfo.name(), MC_FOREIGN_KEY_TAG) != 0) {
            continue;
        }
        QString value = classInfo.value();
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
        auto list = value.split(MC_MAPPED_SPLIT_SYMBOL, QString::SkipEmptyParts);
#else
        auto list = value.split(MC_MAPPED_SPLIT_SYMBOL, Qt::SkipEmptyParts);
#endif
        Q_ASSERT(list.length() >= 1);
        value = list.first();
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
        list = value.split(MC_NORMAL_SPLIT_SYMBOL, QString::SkipEmptyParts);
#else
        list = value.split(MC_NORMAL_SPLIT_SYMBOL, Qt::SkipEmptyParts);
#endif
        Q_ASSERT(list.length() == 2);
        map.insert(list.first().simplified(), list.last().simplified());
    }
    return map;
}
