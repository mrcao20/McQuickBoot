#pragma once

#include "../../McGlobal.h"

class MCIOCCONTAINER_EXPORT McBeanEnum : public QObject {
    Q_OBJECT
    MC_DECL_INIT(McBeanEnum)
public:
    explicit McBeanEnum(QObject *parent = nullptr)
        : QObject(parent)
    {}
    ~McBeanEnum() override = default;
    
    QString scope() const noexcept
    { return m_scope; }
    void setScope(const QString &val) noexcept
    { m_scope = val; }
    
    QString type() const noexcept
    { return m_type; }
    void setType(const QString &val) noexcept
    { m_type = val; }
    
    QString value() const noexcept
    { return m_value; }
    void setValue(const QString &val) noexcept
    { m_value = val; }

private:
    QString m_scope;
    QString m_type;
    QString m_value;
};

MC_DECL_METATYPE(McBeanEnum)
