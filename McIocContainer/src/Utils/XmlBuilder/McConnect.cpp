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
    QDomElement con = doc.createElement(MC_CONNECT);
    con.setAttribute(MC_SENDER, d->sender);
    con.setAttribute(MC_SIGNAL, d->signal);
    con.setAttribute(MC_RECEIVER, d->receiver);
    con.setAttribute(MC_SLOT, d->slot);
    con.setAttribute(MC_CONNECTION_TYPE, d->connectionType);
    return con;
}

}
