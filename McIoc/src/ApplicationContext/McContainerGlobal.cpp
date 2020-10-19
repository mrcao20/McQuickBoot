#include "McIoc/ApplicationContext/McContainerGlobal.h"

void mcConnect(const QString &beanName
            , const QString &sender
            , const QString &signal
            , const QString &receiver
            , const QString &slot
            , Qt::ConnectionType type) noexcept 
{
    McAnnotationApplicationContext::addConnect(beanName, sender, signal, receiver, slot, type);
}
