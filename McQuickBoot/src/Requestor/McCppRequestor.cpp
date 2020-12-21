#include "McBoot/Requestor/McCppRequestor.h"

#include <QVariant>

#include "McBoot/Controller/impl/McCppResponse.h"

MC_AUTO_INIT(McCppRequestor)
MC_INIT_END

MC_DECL_PRIVATE_DATA(McCppRequestor)
MC_DECL_PRIVATE_DATA_END

McCppRequestor::McCppRequestor(QObject *parent) : McAbstractRequestor(parent)
{
    MC_NEW_PRIVATE_DATA(McCppRequestor);
}

McCppRequestor::~McCppRequestor()
{
    qDebug() << "~McCppRequestor";
}

McCppResponse &McCppRequestor::invoke(const QString &uri) noexcept
{
    auto response = new McCppResponse();
    run(response, uri, QVariant());
    return *response; //!< 没有指定父对象，该对象将在整个请求完毕时被析构
}

McCppResponse &McCppRequestor::invoke(const QString &uri, const QJsonObject &data) noexcept
{
    auto response = new McCppResponse();
    run(response, uri, data);
    return *response; //!< 没有指定父对象，该对象将在整个请求完毕时被析构
}
