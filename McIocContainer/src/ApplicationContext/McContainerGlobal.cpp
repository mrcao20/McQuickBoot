#include "include/ApplicationContext/McContainerGlobal.h"

MC_BEGIN_NAMESPACE

void connect(const QString &beanName
            , const QString &sender
            , const QString &signal
            , const QString &receiver
            , const QString &slot
            , Qt::ConnectionType type) noexcept {
    
    McAnnotationApplicationContext::addConnect(beanName, sender, signal, receiver, slot, type);
}

MC_END_NAMESPACE
