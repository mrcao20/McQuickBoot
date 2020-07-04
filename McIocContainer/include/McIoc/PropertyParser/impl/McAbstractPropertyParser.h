#pragma once

#include "../IMcPropertyParser.h"

class McAbstractPropertyParser
        : public QObject
        , public IMcPropertyParser 
{
    Q_OBJECT
public:
    explicit McAbstractPropertyParser(QObject *parent = nullptr);
    
    QVariant parse(const QDomElement &ele) const noexcept override;
    
protected:
    virtual QVariant parseValue(const QDomElement &ele) const noexcept = 0;
    virtual QVariant parseRef(const QDomElement &ele) const noexcept = 0;
    virtual QVariant parseList(const QDomElement &ele) const noexcept = 0;
    virtual QVariant parseMap(const QDomElement &ele) const noexcept = 0;
    virtual QVariant parseEnum(const QDomElement &ele) const noexcept = 0;
};

MC_DECL_POINTER(McAbstractPropertyParser)
