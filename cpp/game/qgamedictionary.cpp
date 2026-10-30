#include "qgamedictionary.h"

QGameDictionary::QGameDictionary(QObject *parent) :
    QObject(parent)
{
    qDebug() << "QGameDictionary constructor";
    // get stored settings
    lang = getLang();

    if (lang.isEmpty()) {
        QLocale::Language l = QLocale().language();

        if (l == QLocale::German) {
            setCurrentLang("de");
        } else if (l == QLocale::Spanish) {
            setCurrentLang("es");
        } else {
            setCurrentLang("en");
        }
    }

    // fill dictionary
    fillDictionary("AreYouSure", "Are you sure?", "Bist du sicher?", "¿Estás seguro?");
    fillDictionary("SurrenderForTurn", "Are you really want to surrender? Are you sure?", "Willst du wirklich aufgeben? Bist du sicher?", "¿De verdad quieres rendirte? ¿Estás seguro?");
    fillDictionary("EnterLoginPass", "Please enter Login and Password", "Bitte gib Login und Passwort ein", "Introduce tu usuario y contraseña");
    fillDictionary("OK", "OK", "OK", "OK");
    fillDictionary("Cancel", "Cancel", "Abbrechen", "Cancelar");
    fillDictionary("EnterProxy", "Enter proxy settigns", "Proxy-Einstellungen eingeben", "Introduce la configuración del proxy");
    fillDictionary("ProxyHost", "Proxy host", "Proxy-Host", "Host del proxy");
    fillDictionary("ProxyPort", "Proxy port", "Proxy-Port", "Puerto del proxy");
    fillDictionary("ProxyUser", "Proxy user", "Proxy-Benutzer", "Usuario del proxy");
    fillDictionary("ProxyPass", "Proxy password", "Proxy-Passwort", "Contraseña del proxy");
    fillDictionary("Close", "Close", "Schließen", "Cerrar");
    fillDictionary("LoginSett", "Login settings", "Anmeldeeinstellungen", "Ajustes de inicio de sesión");
    fillDictionary("NetworkSett", "Network settings", "Netzwerkeinstellungen", "Ajustes de red");
    fillDictionary("LangSett", "Language settings", "Spracheinstellungen", "Ajustes de idioma");
    fillDictionary("EnterNewUser", "Enter new gamer data", "Daten des neuen Spielers eingeben", "Introduce los datos del nuevo jugador");
    fillDictionary("Login", "Login", "Login", "Usuario");
    fillDictionary("Password", "Password", "Passwort", "Contraseña");
    fillDictionary("Email", "Email", "E-Mail", "Correo electrónico");
    fillDictionary("Im13", "I am a least 13 years old", "Ich bin mindestens 13 Jahre alt", "Tengo al menos 13 años");
    fillDictionary("Register", "Register", "Registrieren", "Registrarse");
    fillDictionary("LoginIn", "Login in", "Anmelden", "Iniciar sesión");
    fillDictionary("LangDE", "Deutsch", "Deutsch", "Deutsch");
    fillDictionary("LangEN", "English", "English", "English");
    fillDictionary("LangES", "Español", "Español", "Español");
    fillDictionary("Hint", "Hint", "Hinweis", "Pista");
    fillDictionary("Description", "Description", "Beschreibung", "Descripción");
    fillDictionary("Accept", "Accept", "Annehmen", "Aceptar");

    // game dictionary
    fillGameDictionary();
}

void QGameDictionary::fillDictionary(const QString& Code, const QString& en, const QString& de, const QString& es) {
    dictionary_en[Code] = en;
    dictionary_de[Code] = de;
    dictionary_es[Code] = es;
}

QString QGameDictionary::getStringByCode(const QString &Code) {
    // value(), not operator[]: the non-const operator inserts every missing code, and
    // this singleton is read by the UI thread and the bot service thread at once.
    QString result;
    if (lang.compare("de") == 0) {
        result = dictionary_de.value(Code);
    } else if (lang.compare("es") == 0) {
        result = dictionary_es.value(Code);
    }
    // English is the default, and stands in for a code added with an empty de/es text
    if (result.isEmpty()) {
        result = dictionary_en.value(Code);
    }
    if (result.isNull()) {
        qDebug() << "getStringByCode" << lang << Code;
        return Code;
    }
    return result;
}

QString QGameDictionary::getCurrentLang() {
    return lang;
}

void QGameDictionary::setCurrentLang(QString Lang) {
    if ((Lang.compare("en") != 0 && Lang.compare("de") != 0 && Lang.compare("es") != 0) || (Lang.compare(lang) == 0)) {
        return;
    }

    lang = Lang;

    storeLang();
}

void QGameDictionary::storeLang() {

}

void QGameDictionary::fillGameDictionary() {
    qDebug() << "QGameDictionary::fillGameDictionary";
}

QString QGameDictionary::getLang() {
    return lang;
}

QGameDictionary * QGameDictionary::getInstance() {
    qDebug() << "QGameDictionary::getInstance" << self;
    if (!self) {
        self = new QGameDictionary();
    }
    return self;
}

QGameDictionary* QGameDictionary::self = 0;
