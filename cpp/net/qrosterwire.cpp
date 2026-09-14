#include "qrosterwire.h"

#include <limits>

// Pure wire parser for the caster server's WHO reply (spec sections A.4, B.1, B.3).
// Nothing here touches a socket, a QObject, QSettings or a global: every function is
// a total function from bytes to bool + an out parameter, so it is safe to call from
// anywhere and trivial to feed from a fuzz corpus.
//
// The three-dialect trap (spec B.2): the CSV parsed here is the WIRE dialect, where
// field 9 is IDLE SECONDS and field 11 is the server-side warlockId. It must never be
// handed to QWarlockStat(QString, bool) or to any parseXxxAndInit().

namespace {

const int kEpochHexLength = 16;
const int kCsvFieldCount = 12;
const int kMaxNameBytes = 32;
const int kMaxDigits = 20;
// Field 9 is a duration in seconds; the "unknown" sentinel the server sends is
// 20000100. The cap keeps `now - idleSeconds` inside qint64 sanity and rejects the
// values a 32-bit parse would have wrapped into a timestamp in the future.
const qint64 kMaxIdleSeconds = Q_INT64_C(2147483648);   // 2^31

// Decimal, unsigned, no sign, no padding, 1..20 digits, overflow-safe.
bool parseDigits(const QByteArray &b, quint64 *out) {
    if (b.isEmpty() || b.size() > kMaxDigits) {
        return false;
    }
    quint64 v = 0;
    for (const char c : b) {
        if (c < '0' || c > '9') {
            return false;
        }
        const quint64 d = static_cast<quint64>(c - '0');
        if (v > (std::numeric_limits<quint64>::max() - d) / 10) {
            return false;
        }
        v = v * 10 + d;
    }
    *out = v;
    return true;
}

}

bool QRosterWire::isEpochHex(const QString &s) {
    if (s.size() != kEpochHexLength) {
        return false;
    }
    for (const QChar &c : s) {
        const char16_t u = c.unicode();
        if ((u < u'0' || u > u'9') && (u < u'a' || u > u'f')) {
            return false;   // lowercase hex only
        }
    }
    return true;
}

QList<QByteArray> QRosterWire::fields(const QByteArray &line) {
    // 0x09 only. The CSV in a W line stays one field, byte for byte, so a comma,
    // a space or anything else inside it survives untouched.
    return line.split('\t');
}

bool QRosterWire::validWireName(const QString &n) {
    // Mirrors validName()/wireSafe() on the server side (spec C.1) so the client
    // never rejects a row the server was entitled to send. The limit is 32 BYTES,
    // not 32 QChars: anything that is not printable ASCII is rejected outright by
    // the loop below, so a multi-byte character can never sneak past the length.
    const QByteArray raw = n.toUtf8();
    if (raw.isEmpty() || raw.size() > kMaxNameBytes) {
        return false;
    }
    for (const char c : raw) {
        const unsigned char u = static_cast<unsigned char>(c);
        if (u < 0x20 || u > 0x7E) {
            return false;
        }
    }
    return !n.trimmed().isEmpty();
}

bool QRosterWire::parseHeader(const QList<QByteArray> &f, QRosterBlock *out) {
    if (out == nullptr || f.size() != 6) {
        return false;
    }
    if (f.at(0) != "WHO") {
        return false;
    }
    quint64 n = 0, revision = 0, total = 0;
    if (!parseDigits(f.at(1), &n) || n > static_cast<quint64>(kMaxDeclaredCount)) {
        return false;
    }
    bool full;
    if (f.at(2) == "FULL") {
        full = true;
    } else if (f.at(2) == "DELTA") {
        full = false;
    } else {
        return false;   // the mode is literal, never inferred from n
    }
    const QString epoch = QString::fromLatin1(f.at(3));
    if (epoch.compare(QStringLiteral("-")) != 0 && !isEpochHex(epoch)) {
        return false;
    }
    if (!parseDigits(f.at(4), &revision) || !parseDigits(f.at(5), &total)) {
        return false;
    }

    QRosterBlock b;   // a header always starts a fresh block
    b.declaredCount = static_cast<int>(n);
    b.full = full;
    b.epoch = epoch;
    b.revision = revision;
    b.total = total;   // diagnostics only, no client control flow may depend on it
    *out = b;
    return true;
}

bool QRosterWire::parseRow(const QList<QByteArray> &f, QRosterRow *out) {
    if (out == nullptr || f.size() != 3) {
        return false;
    }
    if (f.at(0) != "W") {
        return false;
    }
    if (f.at(1) != "0" && f.at(1) != "1") {
        return false;
    }
    QRosterRow r;
    if (!parseCsv(f.at(2), &r)) {
        return false;
    }
    // The server's opinion at send time only. The core recomputes online from the
    // idle seconds with its own clock and drops this bit.
    r.serverOnline = (f.at(1) == "1");
    *out = r;
    return true;
}

bool QRosterWire::parseRemoved(const QList<QByteArray> &f, QString *out) {
    if (out == nullptr || f.size() != 2) {
        return false;
    }
    if (f.at(0) != "D") {
        return false;
    }
    const QString name = QString::fromUtf8(f.at(1));
    if (!validWireName(name)) {
        return false;
    }
    *out = name;
    return true;
}

bool QRosterWire::parseEnd(const QList<QByteArray> &f, QRosterBlock *io) {
    if (io == nullptr || f.size() != 3) {
        return false;
    }
    if (f.at(0) != "END") {
        return false;
    }
    // The terminator repeats the header's own two locals. Anything else means the
    // block was spliced, truncated or interleaved: a structural failure, never a
    // reason to commit a watermark.
    if (QString::fromLatin1(f.at(1)).compare(io->epoch) != 0) {
        return false;
    }
    quint64 revision = 0;
    if (!parseDigits(f.at(2), &revision) || revision != io->revision) {
        return false;
    }
    if (io->bodyLines != io->declaredCount) {
        return false;
    }
    // Taken from END, which is authoritative (spec A.5). Equal to the header's by
    // the checks above; assigned anyway so the intent is visible here.
    io->epoch = QString::fromLatin1(f.at(1));
    io->revision = revision;
    return true;
}

bool QRosterWire::parseCsv(const QByteArray &csv, QRosterRow *out) {
    if (out == nullptr) {
        return false;
    }
    const QList<QByteArray> data = csv.split(',');
    if (data.size() != kCsvFieldCount) {
        return false;   // checked BEFORE any at(): out of range is UB in release
    }
    bool ok = false;
    QRosterRow r;

    const int registered = data.at(0).toInt(&ok);
    if (!ok) {
        return false;
    }
    r.registered = (registered != 0);

    r.name = QString::fromUtf8(data.at(1));
    if (!validWireName(r.name)) {
        return false;
    }

    r.ladder = data.at(2).toInt(&ok);
    if (!ok) {
        return false;
    }
    r.melee = data.at(3).toInt(&ok);
    if (!ok) {
        return false;
    }
    r.played = data.at(4).toInt(&ok);
    if (!ok) {
        return false;
    }
    r.won = data.at(5).toInt(&ok);
    if (!ok) {
        return false;
    }
    r.died = data.at(6).toInt(&ok);
    if (!ok) {
        return false;
    }
    r.elo = data.at(7).toInt(&ok);
    if (!ok) {
        return false;
    }

    // The colour ends up in QWarlockStat::toString(), which is the comma
    // separated ini format, and in the QML palette. A comma, tab or newline
    // cannot reach here (they would have broken the field split first), but
    // non-ASCII still can, so keep it to what the site actually publishes.
    // Mirrors the server's wireSafe(), which constrains the colour to printable
    // ASCII and nothing else. Do NOT add an emptiness or length rule here: the
    // watermark advances past a skipped row, so anything the client rejects but
    // the server was willing to send becomes invisible until it changes again.
    const QByteArray color = data.at(8);
    foreach(char c, color) {
        if ((c < 0x20) || (c > 0x7E)) {
            return false;
        }
    }
    r.color = QString::fromLatin1(color);

    // A DURATION, not a timestamp. toLongLong, never toInt: the unknown sentinel is
    // 20000100 and a 32-bit overflow here would silently produce a _lastActivity in
    // the future, which reads as permanently online.
    r.idleSeconds = data.at(9).toLongLong(&ok);
    if (!ok || r.idleSeconds < 0 || r.idleSeconds > kMaxIdleSeconds) {
        return false;
    }

    const int mobile = data.at(10).toInt(&ok);
    if (!ok) {
        return false;
    }
    r.mobile = (mobile != 0);

    // Parsed for completeness and validation only. The core DELIBERATELY never
    // applies it: _warlockId is owned by finishScanWarlock (scraped from the profile
    // page) and round-trips through QSettings, so the wire's structural 0 would only
    // wipe it (spec B.4). It has no bearing on online or is_bot.
    r.warlockId = data.at(11).toInt(&ok);
    if (!ok) {
        return false;
    }

    *out = r;
    return true;
}
