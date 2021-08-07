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
#include "McOrm/Parser/impl/McAbstractParser.h"

#include <QMetaObject>
#include <QMetaClassInfo>

#include "McOrm/SqlSlot/IMcSqlSlot.h"
#include "McOrm/McOrmMacroGlobal.h"

MC_INIT(McAbstractParser)
MC_REGISTER_BEAN_FACTORY(McAbstractParser)
MC_INIT_END

void McAbstractParser::parse(const QList<QObjectPtr> &beans) noexcept
{
    for(auto bean : beans) {
        auto metaObj = bean->metaObject();
        QList<QMetaMethod> methods;
        for(int i = 0, count = metaObj->methodCount(); i < count; ++i) {
            auto method = metaObj->method(i);
            if(method.methodType() != QMetaMethod::Signal) {
                continue;
            }
            methods.append(method);
        }
        for(int i = 0, count = metaObj->classInfoCount(); i < count; ++i) {
            auto classInfo = metaObj->classInfo(i);
            QScopedPointer<IMcSqlSlot> sqlSlot;
            sqlSlot.reset(createSqlSlot(classInfo.name()));
            if(sqlSlot.isNull()) {
                continue;
            }
            QByteArray methodName;
            QByteArray additionalInfo;
            QByteArray classInfoValue = classInfo.value();
            int symbolIndex = classInfoValue.indexOf(MC_SQL_SPLIT_SYMBOL);
            if(symbolIndex == -1) {
                methodName = classInfoValue;
            } else {
                methodName = classInfoValue.left(symbolIndex);
                additionalInfo = classInfoValue.mid(symbolIndex + qstrlen(MC_SQL_SPLIT_SYMBOL));
            }
            QMetaMethod sig;
            for(int j = 0; j < methods.length(); ++j) {
                auto method = methods.at(j);
                if(method.name() != QMetaObject::normalizedSignature(methodName)) {
                    continue;
                }
                sig = method;
                methods.removeAt(j);
                break;
            }
            if(!sig.isValid()) {
                qCritical("'%s' is not a signal!!\n", methodName.data());
                continue;
            }
            sqlSlot->init(bean.data(), sig, additionalInfo);
            sqlSlot.take();
        }
    }
}
