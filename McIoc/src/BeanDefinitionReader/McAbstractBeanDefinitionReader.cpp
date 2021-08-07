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
#include "McIoc/BeanDefinitionReader/impl/McAbstractBeanDefinitionReader.h"

#include <QDebug>

#include "McIoc/BeanFactory/IMcBeanDefinitionRegistry.h"

MC_DECL_PRIVATE_DATA(McAbstractBeanDefinitionReader)
IMcBeanDefinitionRegistry *registry{nullptr};
MC_DECL_PRIVATE_DATA_END

McAbstractBeanDefinitionReader::McAbstractBeanDefinitionReader(QObject *parent)
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(McAbstractBeanDefinitionReader);
}

McAbstractBeanDefinitionReader::~McAbstractBeanDefinitionReader()
{
}

IMcBeanDefinitionRegistry *McAbstractBeanDefinitionReader::registry() const noexcept 
{
    return  d->registry;
}

void McAbstractBeanDefinitionReader::readBeanDefinition(IMcBeanDefinitionRegistry *registry) noexcept 
{
    d->registry = registry;
    doReadBeanDefinition();
}

Qt::ConnectionType McAbstractBeanDefinitionReader::getConnectionType(const QString &typeStr) noexcept 
{
    Qt::ConnectionType type = Qt::ConnectionType::AutoConnection;
    
#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
    QStringList typeList = typeStr.simplified().split('|', Qt::SkipEmptyParts);
#else
    QStringList typeList = typeStr.simplified().split('|', QString::SkipEmptyParts);
#endif
    
    if(typeList.size() > 2) {
        qCritical() << "请按照QObject::connect的要求设置ConnectionType";
        return type;
    }
    
    if(typeList.size() == 1) {
        auto temp = connectionTypeStrToEnum(typeList.at(0));
        if(temp == Qt::ConnectionType::UniqueConnection)
            type = static_cast<Qt::ConnectionType>(type | temp);
        else {
            type = temp;
        }
    } else if(typeList.size() == 2) {
        auto temp1 = connectionTypeStrToEnum(typeList.at(0));
        auto temp2 = connectionTypeStrToEnum(typeList.at(1));
        if(temp1 != Qt::ConnectionType::UniqueConnection
                && temp2 != Qt::ConnectionType::UniqueConnection) {
            qCritical() << "如果指定两种连接方式，则至少有一种必须为Qt::UniqueConnection";
        }else if(temp1 == Qt::ConnectionType::UniqueConnection
                 && temp2 == Qt::ConnectionType::UniqueConnection) {
            type = static_cast<Qt::ConnectionType>(type | temp1);
        }else {
            type = static_cast<Qt::ConnectionType>(temp1 | temp2);
        }
    }
    return type;
}

Qt::ConnectionType McAbstractBeanDefinitionReader::connectionTypeStrToEnum(const QString &typeStr) noexcept 
{
    auto type = typeStr.simplified();
    if (type == "AutoConnection") {
        return Qt::ConnectionType::AutoConnection;
    } else if (type == "DirectConnection") {
        return Qt::ConnectionType::DirectConnection;
    } else if (type == "QueuedConnection") {
        return Qt::ConnectionType::QueuedConnection;
    } else if (type == "BlockingQueuedConnection") {
        return Qt::ConnectionType::BlockingQueuedConnection;
    } else {
        return Qt::ConnectionType::UniqueConnection;
    }
}
