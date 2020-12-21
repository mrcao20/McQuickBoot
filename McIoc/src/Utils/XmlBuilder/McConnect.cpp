#include "McIoc/Utils/XmlBuilder/impl/McConnect.h"

namespace McXmlBuilder {

MC_DECL_PRIVATE_DATA(McConnect)
QString sender;
QString signal;
QString receiver;
QString slot;
QString connectionType;
MC_DECL_PRIVATE_DATA_END

McConnect::McConnect() noexcept
{
    MC_NEW_PRIVATE_DATA(McConnect);
}

McConnect::~McConnect()
{
}

void McConnect::setSender(const QString &val) noexcept
{
    d->sender = val;
}

void McConnect::setSignal(const QString &val) noexcept
{
    d->signal = val;
}

void McConnect::setReceiver(const QString &val) noexcept
{
    d->receiver = val;
}

void McConnect::setSlot(const QString &val) noexcept
{
    d->slot = val;
}

void McConnect::setConnectionType(const QString &val) noexcept
{
    d->connectionType = val;
}

QDomElement McConnect::toDomElement(QDomDocument &doc) const noexcept
{
    QDomElement con = doc.createElement(Mc::Constant::Tag::Xml::connect);
    con.setAttribute(Mc::Constant::Tag::Xml::sender, d->sender);
    con.setAttribute(Mc::Constant::Tag::Xml::signal, d->signal);
    con.setAttribute(Mc::Constant::Tag::Xml::receiver, d->receiver);
    con.setAttribute(Mc::Constant::Tag::Xml::slot, d->slot);
    con.setAttribute(Mc::Constant::Tag::Xml::type, d->connectionType);
    return con;
}

}
