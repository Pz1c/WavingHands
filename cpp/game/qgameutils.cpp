#include "qgameutils.h"
#include "qgamedictionary.h"

QGameUtils::QGameUtils()
{

}

bool QGameUtils::_is_seed_set = false;
uint QGameUtils::_seed = 0;

void QGameUtils::initRand(uint seed) {
    if (_is_seed_set) {
        return;
    }

    _is_seed_set = true;
    _seed = seed;
    //QRandomGenerator::global()->seed(_seed);
}

int QGameUtils::getCurrTimestamp2021() {
    return static_cast<int>(QDateTime::currentSecsSinceEpoch() - SECONDS_AT_20210901);
}

int QGameUtils::getTs20021DIffInSec(int ts) {
    return static_cast<int>(getCurrTimestamp2021() - ts);
}

int QGameUtils::rand(int min, int max) {
    return qFloor(QRandomGenerator::global()->generateDouble() * ((max + 1) - min) + min);
}

qreal QGameUtils::rand(qreal min, qreal max) {
    return rand((int)(min * 1000), (int)(max * 1000)) / 1000;
}

QString QGameUtils::rand(int Ln) {
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");

    QString randomString;
    for(int i = 0; i < Ln; ++i)
    {
        int index = rand(0, possibleCharacters.length());
        QChar nextChar = possibleCharacters.at(index);
        randomString.append(nextChar);
    }
    return randomString;
}
