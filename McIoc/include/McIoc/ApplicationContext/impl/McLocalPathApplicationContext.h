#pragma once

#include "McXmlApplicationContext.h"

class MCIOC_EXPORT McLocalPathApplicationContext : public McXmlApplicationContext
{
    Q_OBJECT
public:
    explicit McLocalPathApplicationContext(QObject *parent = nullptr);
    McLocalPathApplicationContext(const QString &location, QObject *parent = nullptr);
    McLocalPathApplicationContext(const QStringList &locations, QObject *parent = nullptr);
};
        
MC_DECL_POINTER(McLocalPathApplicationContext)
