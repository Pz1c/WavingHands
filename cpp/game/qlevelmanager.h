#ifndef QLEVELMANAGER_H
#define QLEVELMANAGER_H

#include <QObject>
#include <QDebug>
#include "qgameutils.h"
#include "qlevelitem.h"


class QLevelManager : public QObject
{
    Q_OBJECT

public:
    explicit QLevelManager(QObject *parent = 0);
    ~QLevelManager();

signals:
	
public slots:
    void loadLevels(QSettings *settings);
    void saveLevels(QSettings *settings);
    QString getJSON();

    QLevelItem* getLevelByID(int id);

protected slots:

protected:
    QList<QLevelItem *> _itemlist;

private:
    
};

#endif
