#include "qlevelitem.h"

QLevelItem::QLevelItem(int id, int status, bool paid, QObject *parent) : QObject(parent) {
    _id = id;
    _status = status;
    _paid = paid;
    _paidup = false;
    _star = 0;
}

int QLevelItem::status() const
{
    return _status;
}

void QLevelItem::setStatus(int status)
{
    _status = status;
}

int QLevelItem::id() const
{
    return _id;
}

bool QLevelItem::paid() const
{
    return _paid;
}

bool QLevelItem::paidup() const
{
    return _paidup;
}

void QLevelItem::setPaidup(bool paidup)
{
    _paidup = paidup;
}

int QLevelItem::star() const
{
    return _star;
}

void QLevelItem::setStar(int star)
{
    _star = star;
}

QString QLevelItem::getJSON() {
    return QString("{\"id\":%1,\"s\":%2,\"st\":%3,\"p\":%4,\"pu\":%5}")
            .arg(QString::number(_id), QString::number(_status), QString::number(_star), _paid ? "1" : "0", _paidup ? "1" : "0");
}

void QLevelItem::load(QSettings *settings) {
    this->setStatus(settings->value("status").toInt());
    this->setStar(settings->value("star").toInt());
    this->setPaidup(settings->value("paidup").toBool());
}

void QLevelItem::save(QSettings *settings) {
    settings->setValue("status", this->status());
    settings->setValue("star", this->star());
    settings->setValue("paidup", this->paidup());
}
