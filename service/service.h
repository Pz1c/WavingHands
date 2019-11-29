#ifndef SERVICE_H
#define SERVICE_H

#include <QtCore>
#include "rep_service_source.h"

class Service: public ServiceSource {
    Q_OBJECT
public:
    Service(QObject *parent=0);
public slots:
    void reset() override;
signals:
 //   void timeElapsed(int counter,int time);
private slots:
    void timerTimeout();
private:
    QTimer          *timer;
    int             counter;
    QTime           reset_time;
};

#endif // SERVICE_H
