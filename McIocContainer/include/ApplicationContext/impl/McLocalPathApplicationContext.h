#pragma once

#include "McXmlApplicationContext.h"

MC_BEGIN_NAMESPACE

class MCIOCCONTAINER_EXPORT McLocalPathApplicationContext 
        : public McXmlApplicationContext {
    
    Q_OBJECT
public:
    explicit McLocalPathApplicationContext(QObject *parent = nullptr);
    McLocalPathApplicationContext(const QString &location, QObject *parent = nullptr);
    McLocalPathApplicationContext(const QStringList &locations, QObject *parent = nullptr);
};
        
MC_END_NAMESPACE

MC_FORWARD_DECL_CLASS(McLocalPathApplicationContext)
