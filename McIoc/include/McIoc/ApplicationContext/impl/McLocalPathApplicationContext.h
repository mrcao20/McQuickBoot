#pragma once

#include "McXmlApplicationContext.h"

class MCIOC_EXPORT McLocalPathApplicationContext : public McXmlApplicationContext
{
    Q_OBJECT
public:
    explicit McLocalPathApplicationContext(QObject *parent = nullptr);
    McLocalPathApplicationContext(const QString &location,
                                  const QString &flag = QString(),
                                  QObject *parent = nullptr);
    McLocalPathApplicationContext(const QStringList &locations,
                                  const QString &flag = QString(),
                                  QObject *parent = nullptr);
};
        
MC_DECL_POINTER(McLocalPathApplicationContext)
