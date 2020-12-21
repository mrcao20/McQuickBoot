#pragma once

#include "McAbstractPropertyParser.h"

class MCIOC_EXPORT McDefaultPropertyParser : public McAbstractPropertyParser
{
    Q_OBJECT
public:
    explicit McDefaultPropertyParser(QObject *parent = nullptr);
    
protected:
    QVariant parseValue(const QDomElement &ele) const noexcept override;
    QVariant parseRef(const QDomElement &ele) const noexcept override;
    QVariant parseList(const QDomElement &ele) const noexcept override;
    QVariant parseMap(const QDomElement &ele) const noexcept override;
    QVariant parseEnum(const QDomElement &ele) const noexcept override;
    
private:
    QVariantList getList(const QString &dirPath) const noexcept;
};

MC_DECL_POINTER(McDefaultPropertyParser)
