#include "McIoc/Thread/impl/McDefaultDeleteThreadWhenQuit.h"

#include <QThread>

MC_DECL_PRIVATE_DATA(McDefaultDeleteThreadWhenQuit)
QThread *thread{nullptr};
MC_DECL_PRIVATE_DATA_END

McDefaultDeleteThreadWhenQuit::McDefaultDeleteThreadWhenQuit() noexcept 
{
    MC_NEW_PRIVATE_DATA(McDefaultDeleteThreadWhenQuit);
}

McDefaultDeleteThreadWhenQuit::~McDefaultDeleteThreadWhenQuit() 
{
    if(d->thread) {
        d->thread->quit();
        if(d->thread->thread() != QThread::currentThread()) {
            d->thread->wait();
        } else {
            while(!d->thread->isFinished() || d->thread->isRunning()) {
                QThread::msleep(100);
            }
        }
        delete d->thread;
    }
}

void McDefaultDeleteThreadWhenQuit::deleteWhenQuit() noexcept 
{
    QObject *obj = dynamic_cast<QObject *>(this);
    Q_ASSERT_X(obj
               , "McDefaultDeleteThreadWhenQuit"
               , "please make sure it's inherit from QObject");
    d->thread = obj->thread();
}
