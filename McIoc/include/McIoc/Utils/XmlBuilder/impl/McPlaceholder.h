#pragma once

#include "../IMcPropertyContent.h"

namespace McXmlBuilder {

MC_FORWARD_DECL_PRIVATE_DATA(McPlaceholder);

class MCIOC_EXPORT McPlaceholder : public IMcPropertyContent
{
public:
    McPlaceholder() noexcept;
    ~McPlaceholder() override;

    void setPlaceholder(const QString &val) noexcept;

    QDomElement toDomElement(QDomDocument &doc) const noexcept override;

private:
    MC_DECL_PRIVATE(McPlaceholder)
};

MC_DECL_POINTER(McPlaceholder)

} // namespace McXmlBuilder
