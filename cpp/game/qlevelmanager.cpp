#include "qlevelmanager.h"

QLevelManager::QLevelManager(QObject *parent) : QObject(parent) {

}


QLevelManager::~QLevelManager() {
    foreach(QLevelItem *d, _itemlist) {
        d->deleteLater();
    }
    _itemlist.clear();
}

void QLevelManager::loadLevels(QSettings *settings) {
    int size = settings->beginReadArray("level");
    if (size > _itemlist.size()) {
        size = _itemlist.size();
    }
    for (int i = 0; i < size; ++i) {
        settings->setArrayIndex(i);
        _itemlist[i]->load(settings);
    }
    settings->endArray();
}

void QLevelManager::saveLevels(QSettings *settings) {
    settings->beginWriteArray("level");
    for (int i = 0; i < _itemlist.size(); ++i) {
        settings->setArrayIndex(i);
        _itemlist[i]->save(settings);
    }
    settings->endArray();

}

QString QLevelManager::getJSON() {
    QString res = "[";
    bool first = true;
    foreach(QLevelItem *l, _itemlist) {
        if (first) {
            first = false;
        } else {
            res.append(",");
        }
        res.append(l->getJSON());
    }
    res.append("]");
    return res;
}

QLevelItem* QLevelManager::getLevelByID(int id) {
    foreach(QLevelItem *l, _itemlist) {
        if (l->id() == id) {
            return l;
        }
    }

    return 0;
}
