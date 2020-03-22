#pragma once

#include "../IMcPropertyParser.h"

MC_BEGIN_NAMESPACE

class McAbstractPropertyParser
        : public QObject
        , public IMcPropertyParser {
    
    Q_OBJECT
public:
    explicit McAbstractPropertyParser(QObject *parent = nullptr);
    
    QVariant parse(const QDomElement &ele) const noexcept override;
    
protected:
    virtual QVariant parseValue(const QDomElement &ele) const noexcept = 0;
    virtual QVariant parseRef(const QDomElement &ele) const noexcept = 0;
    virtual QVariant parseList(const QDomElement &ele) const noexcept = 0;
    virtual QVariant parseMap(const QDomElement &ele) const noexcept = 0;
};

MC_END_NAMESPACE

MC_FORWARD_DECL_CLASS(McAbstractPropertyParser)
