#pragma once

#include <QThread>

#include <QDebug>

class ThreadTest : public QThread {
    Q_OBJECT
public:
    ThreadTest() {
        fprintf(stderr, "ThreadTest");
        qDebug() << "ThreadTest";
    }
    
    ~ThreadTest() {
        fprintf(stderr, "~ThreadTest");
        qDebug() << "~ThreadTest";
    }
};
