#include "McLog/Utils/McDeleteThreadWhenQuit.h"

#include <QThread>

MC_DECL_PRIVATE_DATA(McDeleteThreadWhenQuit)
QThread *thread{nullptr};
MC_DECL_PRIVATE_DATA_END

McDeleteThreadWhenQuit::McDeleteThreadWhenQuit() noexcept 
{
    MC_NEW_PRIVATE_DATA(McDeleteThreadWhenQuit);
}

McDeleteThreadWhenQuit::~McDeleteThreadWhenQuit() noexcept 
{
    if(d->thread) {
        d->thread->quit();
//        if(d->thread->thread() != QThread::currentThread()) {
//            d->thread->wait();
//        } else {
//            while(!d->thread->isFinished() || d->thread->isRunning()) {
//                QThread::msleep(100);
//            }
//        }
        //! 析构不掉
        d->thread->deleteLater();
    }
}

void McDeleteThreadWhenQuit::deleteWhenQuit() noexcept 
{
    QObject *obj = dynamic_cast<QObject *>(this);
    Q_ASSERT_X(obj
               , "McDeleteThreadWhenQuit"
               , "please make sure it's inherit from QObject");
    d->thread = obj->thread();
}
