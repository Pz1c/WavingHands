#include "service.h"

Service::Service(QObject *parent):ServiceSource(parent) {
    timer=new QTimer(this);
    timer->setInterval(1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerTimeout()));
    counter=0;
}
void Service::reset() {
    qDebug()<<"qwerty Service::reset";
    counter=0;
    timer->start();
    reset_time=QTime::currentTime();
}
void Service::timerTimeout() {
    qDebug()<<"qwerty Service::timerTimeout counter:"<<counter;
    int secs=reset_time.secsTo(QTime::currentTime());
    counter++;
    emit elapsed(counter,secs);
}
