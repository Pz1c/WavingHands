#ifndef QROSTERWIRE_H
#define QROSTERWIRE_H

#include <QByteArray>
#include <QMetaType>
#include <QList>
#include <QString>
#include <QStringList>

struct QRosterRow {
    QString name;                 // csv[1], site canonical spelling
    bool    registered = false;   // csv[0]
    int     ladder = 0, melee = 0, played = 0, won = 0, died = 0, elo = 0;   // csv[2..7]
    QString color;                // csv[8]
    qint64  idleSeconds = 0;      // csv[9]  -- A DURATION, not a timestamp
    bool    mobile = false;       // csv[10]
    int     warlockId = 0;        // csv[11] -- parsed then DISCARDED by the core (B.4)
    bool    serverOnline = false; // W field 1 -- advisory only
};

struct QRosterBlock {
    int      declaredCount = 0;   // header <n>
    bool     full = false;        // header <mode> == "FULL"
    QString  epoch;               // from END (authoritative)
    quint64  revision = 0;        // from END (authoritative)
    quint64  total = 0;           // header <total> -- DIAGNOSTICS ONLY
    QList<QRosterRow> rows;
    QStringList       removed;    // D lines
    int      bodyLines = 0;       // W + D lines actually seen
    int      badRows   = 0;       // rows skipped by row-level validation
};

// All functions are static, pure, and never throw. Every one returns false on
// any validation failure and leaves *out untouched.
class QRosterWire {
public:
    static const int kMaxDeclaredCount = 100000;
    static bool isEpochHex(const QString &s);            // [0-9a-f]{16}
    static bool parseHeader(const QList<QByteArray> &f, QRosterBlock *out);  // 6 tab fields
    static bool parseRow(const QList<QByteArray> &f, QRosterRow *out);       // 3 tab fields
    static bool parseRemoved(const QList<QByteArray> &f, QString *out);      // 2 tab fields
    static bool parseEnd(const QList<QByteArray> &f, QRosterBlock *io);      // 3 tab fields
    static bool parseCsv(const QByteArray &csv, QRosterRow *out);            // 12 comma fields
    static bool validWireName(const QString &n);         // 1..32 bytes, printable ASCII
    // Splits on 0x09. Never uses QString::split on the whole line, because the
    // CSV must be recovered verbatim (see D.6 "no truncation" rule).
    static QList<QByteArray> fields(const QByteArray &line);
};

// Every roster connection is same-thread and direct today, so this is not
// needed yet. It costs nothing and stops an accidental queued connection from
// failing silently at runtime.
Q_DECLARE_METATYPE(QRosterBlock)

#endif
