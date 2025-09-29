#ifndef QLEVELITEM_H
#define QLEVELITEM_H

#include <QObject>
#include <QList>
#include <QDebug>
#include <QSettings>

#define GAME_LEVEL_STATUS_BLOCKED 0
#define GAME_LEVEL_STATUS_OPEN 1
#define GAME_LEVEL_STATUS_PLAYED 2

class QLevelItem : public QObject
{
    Q_OBJECT

public:
    explicit QLevelItem(int id, int status, bool paid, QObject *parent = 0);

    int status() const;
    void setStatus(int status);

    int id() const;

    bool paid() const;

    bool paidup() const;
    void setPaidup(bool paidup);

    int star() const;
    void setStar(int star);

    virtual QString getJSON();

    virtual void save(QSettings *settings);
    virtual void load(QSettings *settings);
signals:

public slots:
    
protected slots:

protected:
    int _id;
    int _status; // 0 blocked, 1 open, 2 played
    int _star; // 0 - 5
    bool _paid;
    bool _paidup;
};

#endif
