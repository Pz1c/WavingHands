#include <QCoreApplication>
#include <QObject>
#include <QVariant>
#include <QDateTime>

#include "service-service.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    ServiceService serviceService;

    return app.exec();
}
