#include "service-service.h"

ServiceService::ServiceService(QObject *parent):QObject(parent) {

    service=new Service(this);
    srcNode=new QRemoteObjectHost(QUrl(QStringLiteral("local:replica")));

    if(srcNode->enableRemoting(service)) {
        qDebug()<<"qwerty service enabled remoting";
    }
}

