#pragma once

#include "../IMcBean.h"

namespace McXmlBuilder {

MC_FORWARD_DECL_CLASS(IMcBeanContent);

MC_FORWARD_DECL_PRIVATE_DATA(McBean);

class MCIOC_EXPORT McBean : public IMcBean
{
public:
    McBean() noexcept;
    ~McBean() override;
    
    void setBeanName(const QString &name) noexcept;
    void setClassName(const QString &name) noexcept;
    void setPluginPath(const QString &path) noexcept;
    void setSingleton(bool val) noexcept;
    void setPointer(bool val) noexcept;
    void addContent(IMcBeanContentConstPtrRef content) noexcept;
    
    QDomElement toDomElement(QDomDocument &doc) const noexcept override;
    
private:
    MC_DECL_PRIVATE(McBean)
};

MC_DECL_POINTER(McBean)

}
