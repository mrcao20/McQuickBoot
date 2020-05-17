#pragma once

#include <QThread>

#include <QDebug>

class ThreadTest : public QThread {
    Q_OBJECT
public:
    ThreadTest() {
        qDebug() << "ThreadTest";
    }
    
    ~ThreadTest() {
        qDebug() << "~ThreadTest";
    }
};
