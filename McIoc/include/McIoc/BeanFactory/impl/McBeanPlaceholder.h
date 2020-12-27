#pragma once

#include "../../McGlobal.h"

class MCIOC_EXPORT McBeanPlaceholder : public QObject
{
    Q_OBJECT
public:
    QString getPlaceholder() const noexcept { return m_placeholder; }
    void setPlaceholder(const QString &val) noexcept { m_placeholder = val; }

private:
    QString m_placeholder;
};

MC_DECL_METATYPE(McBeanPlaceholder)
