#include "qgamedictionary.h"

QGameDictionary::QGameDictionary(QObject *parent) :
    QObject(parent)
{
    qDebug() << "QGameDictionary constructor";
    // get stored settings
    lang = getLang();

    if (lang.isEmpty()) {
        QLocale *locale = new QLocale();
        int c = locale->territory();
        delete locale;

        if (c == 222) { // Ukraine
            setCurrentLang("ua");
        } else if (c == 178) { // RussianPederation
            setCurrentLang("ru");
        } else {
            setCurrentLang("en");
        }
    }

    // fill dictionary
    fillDictionary("AreYouSure", "Ви впевненні?", "Вы уверенны?", "Are you sure?");
    fillDictionary("SurrenderForTurn", "Ви хочете здатись? Точно?", "Вы хотите сдаться? Точно?", "Are you really want to surrender? Are you sure?");
    fillDictionary("EnterLoginPass", "Введіть логін та пароль, будь ласка", "Введите логин и пароль, пожалуйста", "Please enter Login and Password");
    fillDictionary("OK", "OK", "OK", "OK");
    fillDictionary("Cancel", "Відміна", "Отмена", "Cancel");
    fillDictionary("EnterProxy", "Введіть налаштування проксі", "Введите настройки прокси", "Enter proxy settigns");
    fillDictionary("ProxyHost", "Проксі адреса", "Прокси адрес", "Proxy host");
    fillDictionary("ProxyPort", "Проксі порт", "Прокси порт", "Proxy port");
    fillDictionary("ProxyUser", "Проксі користувач", "Прокси пользователь", "Proxy user");
    fillDictionary("ProxyPass", "Проксі пароль", "Прокси пароль", "Proxy password");
    fillDictionary("Close", "Закрити", "Закрыть", "Close");
    fillDictionary("LoginSett", "Налаштування входу", "Настройки входа", "Login settings");
    fillDictionary("NetworkSett", "Налаштування підключення", "Настройки сети", "Network settings");
    fillDictionary("LangSett", "Налаштування мови", "Настройки языка", "Language settings");
    fillDictionary("EnterNewUser", "Заповніть дані нового гравця", "Введите данные нового игрока", "Enter new gamer data");
    fillDictionary("Login", "Логін", "Логин", "Login");
    fillDictionary("Password", "Пароль", "Пароль", "Password");
    fillDictionary("Email", "Email", "Email", "Email");
    fillDictionary("Im13", "Мені як мінімум 13", "Мне как минимум 13", "I am a least 13 years old");
    fillDictionary("Register", "Зареєструвати", "Зарегистрировать", "Register");
    fillDictionary("LoginIn", "Зайти", "Войти", "Login in");
    fillDictionary("LangUA", "Українська", "Українська", "Українська");
    fillDictionary("LangEN", "English", "English", "English");
    fillDictionary("LangRU", "Русский", "Русский", "Русский");
    fillDictionary("Hint", "Підказка", "Подсказка", "Hint");
    fillDictionary("Description", "Опис", "Описание", "Description");
    fillDictionary("Accept", "Прийняти", "Принять", "Accept");

    // game dictionary
    fillGameDictionary();
}

void QGameDictionary::fillDictionary(const QString& Code, const QString& ua, const QString& ru, const QString& en) {
    dictionary_ua[Code] = ua;
    dictionary_ru[Code] = ru;
    dictionary_en[Code] = en;
}

QString QGameDictionary::getStringByCode(const QString &Code) {
    QString result;
    if (lang.compare("ua") == 0) {
        result = dictionary_ua[Code];
    } else if (lang.compare("ru") == 0) {
        result = dictionary_ru[Code];
    } else {
        result = dictionary_en[Code];
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
    if ((Lang.compare("ua") == 0 && Lang.compare("ru") == 0 && Lang.compare("en") == 0) || (Lang.compare(lang) == 0)) {
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
