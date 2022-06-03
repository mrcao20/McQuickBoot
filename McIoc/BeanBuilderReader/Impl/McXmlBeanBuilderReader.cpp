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
#include "McXmlBeanBuilderReader.h"

#include <QJsonDocument>
#include <QLibrary>
#include <QXmlStreamReader>

#include <McCore/PluginChecker/Impl/McNormalPluginChecker.h>

#include "BeanBuilder/Impl/McBeanConnector.h"
#include "BeanBuilder/Impl/McBeanEnum.h"
#include "BeanBuilder/Impl/McBeanPlaceholder.h"
#include "BeanBuilder/Impl/McBeanReference.h"
#include "BeanBuilder/Impl/McListBeanBuilder.h"
#include "BeanBuilder/Impl/McMapBeanBuilder.h"
#include "BeanBuilder/Impl/McSharedGadgetBeanBuilder.h"
#include "BeanBuilder/Impl/McSharedObjectClassBeanBuilder.h"
#include "BeanBuilder/Impl/McSharedObjectPluginBeanBuilder.h"
#include "BeanBuilder/Impl/McSimpleBeanBuilder.h"
#include "BeanBuilder/McCustomBeanBuilderContainer.h"
#include "BeanFactory/IMcBeanBuilderRegistry.h"

MC_DECL_PRIVATE_DATA(McXmlBeanBuilderReader)
QList<QIODevicePtr> devices;
QString flag;
int privateBeanNameIndex{0};
MC_DECL_PRIVATE_DATA_END

McXmlBeanBuilderReader::McXmlBeanBuilderReader(const QIODevicePtr &device, const QString &flag) noexcept
    : McXmlBeanBuilderReader(QList<QIODevicePtr>() << device, flag)
{
}

McXmlBeanBuilderReader::McXmlBeanBuilderReader(const QList<QIODevicePtr> &devices, const QString &flag) noexcept
{
    MC_NEW_PRIVATE_DATA(McXmlBeanBuilderReader);

    d->devices = devices;
    d->flag = flag;
}

McXmlBeanBuilderReader::~McXmlBeanBuilderReader() {}

void McXmlBeanBuilderReader::doReadBeanBuilder() noexcept
{
    for (auto &device : qAsConst(d->devices)) {
        device->seek(0);
        QXmlStreamReader reader(device.data());
        read(reader);
        if (reader.hasError()) {
            qCCritical(mcIoc(),
                "read xml occured error. line: %lld. column: %lld. character offset: %lld. "
                "error type: %d. error msg: %s",
                reader.lineNumber(), reader.columnNumber(), reader.characterOffset(), static_cast<int>(reader.error()),
                qUtf8Printable(reader.errorString()));
        }
    }
}

McObjectClassBeanBuilderPtr McXmlBeanBuilderReader::createObjectBeanBuilder(
    McMetaType metaType, bool isPointer) const noexcept
{
    Q_UNUSED(metaType)
    if (isPointer) {
        return McObjectClassBeanBuilderPtr::create();
    } else {
        return McSharedObjectClassBeanBuilderPtr::create();
    }
}

void McXmlBeanBuilderReader::read(QXmlStreamReader &reader) const noexcept
{
    if (reader.atEnd()) {
        return;
    }
    if (!reader.readNextStartElement()) {
        return;
    }
    if (reader.name() != Mc::Constant::Tag::Xml::BEANS) {
        reader.raiseError(QStringLiteral(u"XML Error: Root element tag is not 'beans'."));
        return;
    }
    while (!reader.atEnd()) {
        auto token = reader.readNext();
        if (token == QXmlStreamReader::EndElement && reader.name() == Mc::Constant::Tag::Xml::BEANS) {
            break;
        } else if (token == QXmlStreamReader::StartElement) {
            if (reader.name() != Mc::Constant::Tag::Xml::BEAN) {
                qCCritical(mcIoc(), "start node name must be 'bean'. current node name '%s'",
                    reader.name().toLatin1().constData());
                reader.skipCurrentElement();
                continue;
            }
            readBean(reader);
        }
    }
}

QString McXmlBeanBuilderReader::readBean(QXmlStreamReader &reader) const noexcept
{
    auto attributes = reader.attributes();
    if (attributes.hasAttribute(Mc::Constant::Tag::Xml::FLAG)
        && attributes.value(Mc::Constant::Tag::Xml::FLAG) != d->flag) {
        reader.skipCurrentElement();
        return QString();
    }
    QString beanName;
    if (attributes.hasAttribute(Mc::Constant::Tag::Xml::NAME)) {
        beanName = attributes.value(Mc::Constant::Tag::Xml::NAME).toString();
        if (registry()->isContained(beanName)) {
            return QString(); //!< 如果存在则不再处理
        }
    } else {
        beanName = genarateUniqueName();
    }
    bool isSingleton = !attributes.hasAttribute(Mc::Constant::Tag::Xml::IS_SINGLETON)
                       || attributes.value(Mc::Constant::Tag::Xml::IS_SINGLETON) == QStringLiteral(u"true");
    bool isPointer = attributes.hasAttribute(Mc::Constant::Tag::Xml::IS_POINTER)
                     && attributes.value(Mc::Constant::Tag::Xml::IS_POINTER) == QStringLiteral(u"true");
    McBeanReferencePtr ref;
    if (attributes.hasAttribute(Mc::Constant::Tag::Xml::PARENT)) {
        ref = McBeanReferencePtr::create();
        ref->setName(attributes.value(Mc::Constant::Tag::Xml::PARENT).toString());
    }
    McAbstractBeanBuilderPtr builder;
    if (attributes.hasAttribute(Mc::Constant::Tag::Xml::CLASS)) {
        builder = readClass(reader, attributes.value(Mc::Constant::Tag::Xml::CLASS), ref, isPointer);
    } else if (attributes.hasAttribute(Mc::Constant::Tag::Xml::PLUGIN)) {
        builder = readPlugin(reader, attributes.value(Mc::Constant::Tag::Xml::PLUGIN), ref, isPointer);
    } else {
        qCCritical(mcIoc(), "bean node must contains attribute 'class/plugin'");
        reader.skipCurrentElement();
        return QString();
    }
    if (Q_UNLIKELY(builder.isNull())) {
        return QString();
    }
    builder->setSingleton(isSingleton);
    registry()->registerBeanBuilder(beanName, builder);
    return beanName;
}

McAbstractBeanBuilderPtr McXmlBeanBuilderReader::readClass(
    QXmlStreamReader &reader, QStringView className, const McBeanReferencePtr &ref, bool isPointer) const noexcept
{
    if (className == Mc::Constant::Tag::Xml::CLASS_LIST) {
        return readList(reader);
    } else if (className == Mc::Constant::Tag::Xml::CLASS_MAP) {
        return readMap(reader);
    } else if (McCustomBeanBuilderContainer::containsBuilder(className.toString())) {
        auto builder = McCustomBeanBuilderContainer::getBuilder(className.toString());
        while (!reader.atEnd()) {
            auto token = reader.readNext();
            if (token == QXmlStreamReader::EndElement && reader.name() == Mc::Constant::Tag::Xml::BEAN) {
                break;
            } else if (token == QXmlStreamReader::StartElement && reader.name() == Mc::Constant::Tag::Xml::PROPERTY) {
                readProperty(builder, reader);
            }
        }
        return builder;
    }
    auto metaType = McMetaType::fromTypeName(className.toLatin1());
    if (Q_UNLIKELY(!metaType.isValid())) {
        return McAbstractBeanBuilderPtr();
    }
    McAbstractBeanBuilderPtr builder;
    if (metaType.pMetaType().flags().testFlag(QMetaType::PointerToQObject)) {
        McObjectClassBeanBuilderPtr objectBuilder = createObjectBeanBuilder(metaType, isPointer);
        readObjectBean(objectBuilder, reader);
        objectBuilder->setParentBeanReference(ref);
        builder = objectBuilder;
    } else {
        McGadgetBeanBuilderPtr gadgetBuilder;
        if (isPointer) {
            gadgetBuilder = McGadgetBeanBuilderPtr::create();
        } else {
            gadgetBuilder = McSharedGadgetBeanBuilderPtr::create();
        }
        readGadgetBean(gadgetBuilder, reader);
        builder = gadgetBuilder;
    }
    builder->setMetaType(metaType);
    return builder;
}

McAbstractBeanBuilderPtr McXmlBeanBuilderReader::readPlugin(
    QXmlStreamReader &reader, QStringView pluginPath, const McBeanReferencePtr &ref, bool isPointer) const noexcept
{
    auto builder = createPluginBeanBuilder(pluginPath.toString(), isPointer);
    if (builder.isNull()) {
        return McAbstractBeanBuilderPtr();
    }
    while (!reader.atEnd()) {
        auto token = reader.readNext();
        if (token == QXmlStreamReader::EndElement && reader.name() == Mc::Constant::Tag::Xml::BEAN) {
            break;
        } else if (token == QXmlStreamReader::StartElement) {
            if (reader.name() == Mc::Constant::Tag::Xml::PROPERTY) {
                readProperty(builder, reader);
            } else if (reader.name() == Mc::Constant::Tag::Xml::CONNECT) {
                readConnect(builder, reader);
            } else if (reader.name() == Mc::Constant::Tag::Xml::PLUGIN) {
                readPluginChecker(builder, reader);
            }
        }
    }
    builder->setParentBeanReference(ref);
    return builder;
}

void McXmlBeanBuilderReader::readObjectBean(
    const McObjectClassBeanBuilderPtr &builder, QXmlStreamReader &reader) const noexcept
{
    int autoArgIndex = 0;
    while (!reader.atEnd()) {
        auto token = reader.readNext();
        if (token == QXmlStreamReader::EndElement && reader.name() == Mc::Constant::Tag::Xml::BEAN) {
            break;
        } else if (token == QXmlStreamReader::StartElement) {
            if (reader.name() == Mc::Constant::Tag::Xml::PROPERTY) {
                readProperty(builder, reader);
            } else if (reader.name() == Mc::Constant::Tag::Xml::CONSTRUCTOR_ARG) {
                readConstructor(builder, autoArgIndex, reader);
                ++autoArgIndex;
            } else if (reader.name() == Mc::Constant::Tag::Xml::CONNECT) {
                readConnect(builder, reader);
            }
        }
    }
}

void McXmlBeanBuilderReader::readGadgetBean(
    const McGadgetBeanBuilderPtr &builder, QXmlStreamReader &reader) const noexcept
{
    int autoArgIndex = 0;
    while (!reader.atEnd()) {
        auto token = reader.readNext();
        if (token == QXmlStreamReader::EndElement && reader.name() == Mc::Constant::Tag::Xml::BEAN) {
            break;
        } else if (token == QXmlStreamReader::StartElement) {
            if (reader.name() == Mc::Constant::Tag::Xml::PROPERTY) {
                readProperty(builder, reader);
            } else if (reader.name() == Mc::Constant::Tag::Xml::CONSTRUCTOR_ARG) {
                readConstructor(builder, autoArgIndex, reader);
                ++autoArgIndex;
            }
        }
    }
}

void McXmlBeanBuilderReader::readProperty(
    const McAbstractBeanBuilderPtr &builder, QXmlStreamReader &reader) const noexcept
{
    if (builder.isNull()) {
        return;
    }
    auto attrs = reader.attributes();
    if (!attrs.hasAttribute(Mc::Constant::Tag::Xml::NAME)) {
        qCCritical(mcIoc(), "property tag must be has attribute 'name'");
        reader.skipCurrentElement();
        return;
    }
    auto proName = attrs.value(Mc::Constant::Tag::Xml::NAME).toString();
    if (attrs.hasAttribute(Mc::Constant::Tag::Xml::VALUE)) {
        builder->addProperty(proName, attrs.value(Mc::Constant::Tag::Xml::VALUE).toString());
    } else if (attrs.hasAttribute(Mc::Constant::Tag::Xml::REF)) {
        auto ref = McBeanReferencePtr::create();
        ref->setName(attrs.value(Mc::Constant::Tag::Xml::REF).toString());
        builder->addProperty(proName, QVariant::fromValue(ref));
    }
    while (!reader.atEnd()) {
        auto token = reader.readNext();
        if (token == QXmlStreamReader::EndElement && reader.name() == Mc::Constant::Tag::Xml::PROPERTY) {
            break;
        } else if (token == QXmlStreamReader::StartElement) {
            builder->addProperty(proName, readValue(reader));
        }
    }
}

void McXmlBeanBuilderReader::readConstructor(
    const McAbstractBeanBuilderPtr &builder, int autoArgIndex, QXmlStreamReader &reader) const noexcept
{
    if (builder.isNull()) {
        return;
    }
    McAbstractBeanBuilder::ConstructorArg arg;
    auto attrs = reader.attributes();
    if (attrs.hasAttribute(Mc::Constant::Tag::Xml::INDEX)) {
        arg.index = attrs.value(Mc::Constant::Tag::Xml::INDEX).toInt();
    } else if (attrs.hasAttribute(Mc::Constant::Tag::Xml::NAME)) {
        arg.name = attrs.value(Mc::Constant::Tag::Xml::NAME).toLatin1();
    } else {
        arg.index = autoArgIndex;
    }
    if (attrs.hasAttribute(Mc::Constant::Tag::Xml::VALUE)) {
        arg.value = attrs.value(Mc::Constant::Tag::Xml::VALUE).toString();
    } else if (attrs.hasAttribute(Mc::Constant::Tag::Xml::REF)) {
        auto ref = McBeanReferencePtr::create();
        ref->setName(attrs.value(Mc::Constant::Tag::Xml::REF).toString());
        arg.value = QVariant::fromValue(ref);
    }
    while (!reader.atEnd()) {
        auto token = reader.readNext();
        if (token == QXmlStreamReader::EndElement && reader.name() == Mc::Constant::Tag::Xml::CONSTRUCTOR_ARG) {
            break;
        } else if (token == QXmlStreamReader::StartElement) {
            arg.value = readValue(reader);
        }
    }
    builder->addConstructorArg(arg);
}

void McXmlBeanBuilderReader::readConnect(const McObjectBeanBuilderPtr &builder, QXmlStreamReader &reader) const noexcept
{
    if (builder.isNull()) {
        return;
    }
    auto con = McBeanConnectorPtr::create();
    auto attrs = reader.attributes();
    if (attrs.hasAttribute(Mc::Constant::Tag::Xml::SENDER)) {
        con->setSender(attrs.value(Mc::Constant::Tag::Xml::SENDER).toString());
    }
    if (attrs.hasAttribute(Mc::Constant::Tag::Xml::SIGNAL_TAG)) {
        con->setSignal(attrs.value(Mc::Constant::Tag::Xml::SIGNAL_TAG).toString());
    }
    if (attrs.hasAttribute(Mc::Constant::Tag::Xml::RECEIVER)) {
        con->setReceiver(attrs.value(Mc::Constant::Tag::Xml::RECEIVER).toString());
    }
    if (attrs.hasAttribute(Mc::Constant::Tag::Xml::SLOT_TAG)) {
        con->setSlot(attrs.value(Mc::Constant::Tag::Xml::SLOT_TAG).toString());
    }
    auto e = McBeanEnumPtr::create();
    e->setScope("Qt");
    e->setType("ConnectionType");
    if (attrs.hasAttribute(Mc::Constant::Tag::Xml::TYPE)) {
        e->setValue(attrs.value(Mc::Constant::Tag::Xml::TYPE).toString());
    }
    while (!reader.atEnd()) {
        auto token = reader.readNext();
        if (token == QXmlStreamReader::EndElement && reader.name() == Mc::Constant::Tag::Xml::CONNECT) {
            break;
        } else if (token == QXmlStreamReader::StartElement) {
            QString attr;
            auto childAttrs = reader.attributes();
            if (childAttrs.hasAttribute(Mc::Constant::Tag::Xml::NAME)) {
                attr = childAttrs.value(Mc::Constant::Tag::Xml::NAME).toString();
                reader.skipCurrentElement();
            } else {
                attr = reader.readElementText();
            }
            if (reader.name() == Mc::Constant::Tag::Xml::SENDER) {
                con->setSender(attr);
            } else if (reader.name() == Mc::Constant::Tag::Xml::SIGNAL_TAG) {
                con->setSignal(attr);
            } else if (reader.name() == Mc::Constant::Tag::Xml::RECEIVER) {
                con->setReceiver(attr);
            } else if (reader.name() == Mc::Constant::Tag::Xml::SLOT_TAG) {
                con->setSlot(attr);
            } else if (reader.name() == Mc::Constant::Tag::Xml::TYPE) {
                e->setValue(attr);
            }
        }
    }
    con->setType(e);
    builder->addConnector(con);
}

void McXmlBeanBuilderReader::readPluginChecker(
    const McObjectPluginBeanBuilderPtr &builder, QXmlStreamReader &reader) const noexcept
{
    if (builder.isNull()) {
        return;
    }
    bool isCheckerFlag = false;
    auto ref = McBeanReferencePtr::create();
    while (!reader.atEnd()) {
        auto token = reader.readNext();
        if (token == QXmlStreamReader::EndElement && reader.name() == Mc::Constant::Tag::Xml::PLUGIN) {
            break;
        } else if (token == QXmlStreamReader::StartElement) {
            auto attrs = reader.attributes();
            if (reader.name() == Mc::Constant::Tag::Xml::METADATA) {
                QByteArray metaData;
                if (attrs.hasAttribute(Mc::Constant::Tag::Xml::VALUE)) {
                    metaData = attrs.value(Mc::Constant::Tag::Xml::VALUE).toUtf8();
                    reader.skipCurrentElement();
                } else {
                    metaData = reader.readElementText().toUtf8();
                }
                QJsonParseError error;
                auto jsonDoc = QJsonDocument::fromJson(metaData, &error);
                if (error.error != QJsonParseError::NoError) {
                    qCCritical(mcIoc(), "parse plugin checker metaData occurred error: %s",
                        qUtf8Printable(error.errorString()));
                    continue;
                }
                auto checker = McNormalPluginCheckerPtr::create(jsonDoc.object());
                auto checkerBuilder = McSimpleBeanBuilderPtr::create(QVariant::fromValue(checker));
                auto checkerBeanName = genarateUniqueName();
                registry()->registerBeanBuilder(checkerBeanName, checkerBuilder);
                ref->setName(checkerBeanName);
            } else if (reader.name() == Mc::Constant::Tag::Xml::CHECKER) {
                if (attrs.hasAttribute(Mc::Constant::Tag::Xml::CLASS)) {
                    auto className = attrs.value(Mc::Constant::Tag::Xml::CLASS);
                    auto checkerBuilder = McSharedGadgetBeanBuilderPtr::create();
                    checkerBuilder->setSingleton(true);
                    checkerBuilder->setClassName(className.toLatin1());
                    auto checkerBeanName = genarateUniqueName();
                    registry()->registerBeanBuilder(checkerBeanName, checkerBuilder);
                    ref->setName(checkerBeanName);
                    reader.skipCurrentElement();
                } else if (attrs.hasAttribute(Mc::Constant::Tag::Xml::REF)) {
                    ref->setName(attrs.value(Mc::Constant::Tag::Xml::REF).toString());
                    reader.skipCurrentElement();
                } else {
                    isCheckerFlag = true;
                }
            } else if (reader.name() == Mc::Constant::Tag::Xml::REF && isCheckerFlag) {
                ref = readValue(reader).value<McBeanReferencePtr>();
            }
        }
    }
    builder->setPluginChecker(ref);
}

McAbstractBeanBuilderPtr McXmlBeanBuilderReader::readList(QXmlStreamReader &reader) const noexcept
{
    auto builder = McListBeanBuilderPtr::create();
    while (!reader.atEnd()) {
        auto token = reader.readNext();
        if (token == QXmlStreamReader::EndElement && reader.name() == Mc::Constant::Tag::Xml::BEAN) {
            break;
        } else if (token == QXmlStreamReader::StartElement && reader.name() == Mc::Constant::Tag::Xml::LIST) {
            auto values = readListValue(reader);
            builder->setValues(values);
        }
    }
    return builder;
}

McAbstractBeanBuilderPtr McXmlBeanBuilderReader::readMap(QXmlStreamReader &reader) const noexcept
{
    auto builder = McMapBeanBuilderPtr::create();
    while (!reader.atEnd()) {
        auto token = reader.readNext();
        if (token == QXmlStreamReader::EndElement && reader.name() == Mc::Constant::Tag::Xml::BEAN) {
            break;
        } else if (token == QXmlStreamReader::StartElement && reader.name() == Mc::Constant::Tag::Xml::MAP) {
            auto values = readMapValue(reader);
            builder->setValues(values);
        }
    }
    return builder;
}

QVariantList McXmlBeanBuilderReader::readListValue(QXmlStreamReader &reader) const noexcept
{
    QVariantList values;
    auto attrs = reader.attributes();
    if (attrs.hasAttribute(Mc::Constant::Tag::Xml::PLUGINS)) {
        auto pluginsPath = attrs.value(Mc::Constant::Tag::Xml::PLUGINS).toString().simplified();
        bool isPointer = attrs.hasAttribute(Mc::Constant::Tag::Xml::IS_POINTER)
                         && attrs.value(Mc::Constant::Tag::Xml::IS_POINTER) == QStringLiteral(u"true");
        values = getList(Mc::toAbsolutePath(pluginsPath), isPointer);
    }
    while (!reader.atEnd()) {
        auto token = reader.readNext();
        if (token == QXmlStreamReader::EndElement && reader.name() == Mc::Constant::Tag::Xml::LIST) {
            break;
        } else if (token == QXmlStreamReader::StartElement) {
            auto tagName = reader.name().toString();
            auto value = readValue(reader);
            if (!value.isValid()) {
                qCWarning(mcIoc(), "in list tag of '%s' cannot parse!!!", qPrintable(tagName));
                continue;
            }
            values.append(value);
        }
    }
    return values;
}

QMap<QVariant, QVariant> McXmlBeanBuilderReader::readMapValue(QXmlStreamReader &reader) const noexcept
{
    QMap<QVariant, QVariant> values;
    auto attrs = reader.attributes();
    if (attrs.hasAttribute(Mc::Constant::Tag::Xml::PLH)) {
        auto plh = attrs.value(Mc::Constant::Tag::Xml::PLH).toString();
        while (!reader.atEnd()) {
            auto token = reader.readNext();
            if (token == QXmlStreamReader::EndElement && reader.name() == Mc::Constant::Tag::Xml::MAP) {
                break;
            } else if (token == QXmlStreamReader::StartElement && reader.name() == Mc::Constant::Tag::Xml::LIST) {
                auto list = readListValue(reader);
                for (auto value : list) {
                    if (value.metaType() != QMetaType::fromType<McBeanReferencePtr>()) {
                        qCCritical(mcIoc())
                            << "if you want to used plh in map tag. please make sure the value be ref tag.";
                        continue;
                    }
                    McBeanPlaceholderPtr beanPlh = McBeanPlaceholderPtr::create();
                    beanPlh->setPlaceholder(plh);
                    values.insert(QVariant::fromValue(beanPlh), value);
                }
                break;
            }
        }
    } else {
        while (!reader.atEnd()) {
            auto token = reader.readNext();
            if (token == QXmlStreamReader::EndElement && reader.name() == Mc::Constant::Tag::Xml::MAP) {
                break;
            } else if (token == QXmlStreamReader::StartElement && reader.name() == Mc::Constant::Tag::Xml::ENTRY) {
                auto entry = readMapEntry(reader);
                if (entry.first.isValid() && entry.second.isValid()) {
                    values.insert(entry.first, entry.second);
                }
            }
        }
    }
    return values;
}

QPair<QVariant, QVariant> McXmlBeanBuilderReader::readMapEntry(QXmlStreamReader &reader) const noexcept
{
    QPair<QVariant, QVariant> entry;
    auto attrs = reader.attributes();
    if (attrs.hasAttribute(Mc::Constant::Tag::Xml::KEY)) {
        entry.first = attrs.value(Mc::Constant::Tag::Xml::KEY).toString();
    }
    if (attrs.hasAttribute(Mc::Constant::Tag::Xml::VALUE)) {
        entry.second = attrs.value(Mc::Constant::Tag::Xml::VALUE).toString();
    }
    while (!reader.atEnd()) {
        auto token = reader.readNext();
        if (token == QXmlStreamReader::EndElement && reader.name() == Mc::Constant::Tag::Xml::ENTRY) {
            break;
        } else if (token == QXmlStreamReader::StartElement) {
            if (reader.name() == Mc::Constant::Tag::Xml::KEY) {
                entry.first = readMapEntryKey(reader);
            } else if (reader.name() == Mc::Constant::Tag::Xml::VALUE) {
                entry.second = readMapEntryValue(reader);
            }
        }
    }
    return entry;
}

QVariant McXmlBeanBuilderReader::readMapEntryKey(QXmlStreamReader &reader) const noexcept
{
    QVariant key;
    bool flag = false;
    while (!reader.atEnd()) {
        auto token = reader.readNext();
        if (token == QXmlStreamReader::EndElement && reader.name() == Mc::Constant::Tag::Xml::KEY) {
            break;
        } else if (token == QXmlStreamReader::StartElement) {
            if (reader.name() == Mc::Constant::Tag::Xml::PLH) {
                McBeanPlaceholderPtr beanPlh = McBeanPlaceholderPtr::create();
                beanPlh->setPlaceholder(reader.readElementText());
                key = QVariant::fromValue(beanPlh);
            } else {
                key = readValue(reader);
            }
            flag = true;
        } else if (token == QXmlStreamReader::Characters && !flag) {
            key = reader.text().toString();
        }
    }
    return key;
}

QVariant McXmlBeanBuilderReader::readMapEntryValue(QXmlStreamReader &reader) const noexcept
{
    QVariant value;
    bool flag = false;
    while (!reader.atEnd()) {
        auto token = reader.readNext();
        if (token == QXmlStreamReader::EndElement && reader.name() == Mc::Constant::Tag::Xml::VALUE) {
            break;
        } else if (token == QXmlStreamReader::StartElement) {
            value = readValue(reader);
            flag = true;
        } else if (token == QXmlStreamReader::Characters && !flag) {
            value = reader.text().toString();
        }
    }
    return value;
}

McObjectPluginBeanBuilderPtr McXmlBeanBuilderReader::createPluginBeanBuilder(
    const QString &pluginPath, bool isPointer) const noexcept
{
    auto absPluginPath = Mc::toAbsolutePath(pluginPath);
    if (!QLibrary::isLibrary(absPluginPath)) {
        return McObjectPluginBeanBuilderPtr();
    }
    McObjectPluginBeanBuilderPtr builder;
    if (isPointer) {
        builder = McObjectPluginBeanBuilderPtr::create();
    } else {
        builder = McSharedObjectPluginBeanBuilderPtr::create();
    }
    builder->setPluginPath(absPluginPath);
    return builder;
}

QVariant McXmlBeanBuilderReader::readValue(QXmlStreamReader &reader) const noexcept
{
    QVariant result;
    if (reader.name() == Mc::Constant::Tag::Xml::VALUE) {
        result = reader.readElementText();
    } else if (reader.name() == Mc::Constant::Tag::Xml::REF) {
        auto attrs = reader.attributes();
        QString refName;
        if (attrs.hasAttribute(Mc::Constant::Tag::Xml::BEAN)) {
            refName = attrs.value(Mc::Constant::Tag::Xml::BEAN).toString();
            reader.skipCurrentElement();
        } else {
            refName = reader.readElementText();
        }
        auto ref = McBeanReferencePtr::create();
        ref->setName(refName);
        result.setValue(ref);
    } else if (reader.name() == Mc::Constant::Tag::Xml::ENUM) {
        McBeanEnumPtr e = McBeanEnumPtr::create();
        auto attrs = reader.attributes();
        if (attrs.hasAttribute(Mc::Constant::Tag::Xml::SCOPE)) {
            e->setScope(attrs.value(Mc::Constant::Tag::Xml::SCOPE).toString());
        }
        if (attrs.hasAttribute(Mc::Constant::Tag::Xml::TYPE)) {
            e->setType(attrs.value(Mc::Constant::Tag::Xml::TYPE).toString());
        }
        if (attrs.hasAttribute(Mc::Constant::Tag::Xml::VALUE)) {
            e->setValue(attrs.value(Mc::Constant::Tag::Xml::VALUE).toString());
        }
        while (!reader.atEnd()) {
            auto token = reader.readNext();
            if (token == QXmlStreamReader::EndElement && reader.name() == Mc::Constant::Tag::Xml::ENUM) {
                break;
            } else if (token == QXmlStreamReader::StartElement) {
                auto tagName = reader.name().toString();
                auto childAttrs = reader.attributes();
                QString nameAttr;
                if (childAttrs.hasAttribute(Mc::Constant::Tag::Xml::NAME)) {
                    nameAttr = childAttrs.value(Mc::Constant::Tag::Xml::NAME).toString();
                    reader.skipCurrentElement();
                } else {
                    nameAttr = reader.readElementText();
                }
                if (tagName == Mc::Constant::Tag::Xml::SCOPE) {
                    e->setScope(nameAttr);
                } else if (tagName == Mc::Constant::Tag::Xml::TYPE) {
                    e->setType(nameAttr);
                } else if (tagName == Mc::Constant::Tag::Xml::VALUE) {
                    e->setValue(nameAttr);
                }
            }
        }
        result.setValue(e);
    } else if (reader.name() == Mc::Constant::Tag::Xml::LIST) {
        result = readListValue(reader);
    } else if (reader.name() == Mc::Constant::Tag::Xml::MAP) {
        result.setValue(readMapValue(reader));
    } else if (reader.name() == Mc::Constant::Tag::Xml::BEAN) {
        auto beanName = readBean(reader);
        if (!beanName.isEmpty()) {
            auto ref = McBeanReferencePtr::create();
            ref->setName(beanName);
            result.setValue(ref);
        }
    }
    return result;
}

QVariantList McXmlBeanBuilderReader::getList(const QString &dirPath, bool isPointer) const noexcept
{
    QVariantList list;

    QDir dir(dirPath);
    if (!dir.exists()) {
        qCCritical(mcIoc()) << dirPath << "not exists";
        return list;
    }
    QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files);
    for (auto &fileInfo : qAsConst(fileInfoList)) {
        QString pluginPath = fileInfo.absoluteFilePath();
        if (!QLibrary::isLibrary(pluginPath)) {
            qCCritical(mcIoc()) << pluginPath << "not a plugin";
            continue;
        }
        auto beanName = genarateUniqueName();
        auto builder = createPluginBeanBuilder(pluginPath, isPointer);
        registry()->registerBeanBuilder(beanName, builder);
        QSharedPointer<McBeanReference> beanRef = QSharedPointer<McBeanReference>::create();
        beanRef->setName(beanName);
        list << QVariant::fromValue(beanRef);
    }

    return list;
}

QString McXmlBeanBuilderReader::genarateUniqueName() const noexcept
{
    QString tmp = QLatin1String("__mc__privateBeanName");
    auto beanName = tmp;
    while (registry()->isContained(beanName)) {
        beanName = tmp;
        beanName.append('_');
        beanName.append(QString::number(d->privateBeanNameIndex++));
    }
    return beanName;
}
