#include "preferences.h"

Preferences* Preferences::preferences = 0;

Preferences::Preferences()
{
    readSettings();
}

Preferences* Preferences::getInstance()
{
    if(preferences == 0)
    {
        preferences = new Preferences();
    }
    return preferences;
}

void Preferences::readSettings()
{
    QSettings settings;
    size = settings.value("geometry/size", QSize(500, 450)).toSize();
    pos = settings.value("geometry/pos", QPoint(200, 200)).toPoint();
    windowState = settings.value("geometry/windowState").toByteArray();
    language = settings.value("preferences/language", "en").toString();

    settings.beginGroup("filterSettings");
    QStringList keys = settings.childKeys();
    for(int i = 0; i < keys.count(); i++)
    {
        QString filter = keys.at(i);
        bool enabled = settings.value(filter, true).toBool();
        filterSettings.insert(filter, enabled);
    }
    settings.endGroup();
}

void Preferences::writeSettings()
{
    QSettings settings;
    settings.setValue("geometry/size", size);
    settings.setValue("geometry/pos", pos);
    settings.setValue("geometry/windowState", windowState);
    settings.setValue("preferences/language", language);
    QHash<QString, bool>::const_iterator i = filterSettings.constBegin();
    while(i != filterSettings.constEnd())
    {
        settings.setValue("filterSettings/" + i.key(), i.value());
        i++;
    }
}

bool Preferences::getFilterSetting(QString filter)
{
    if(!filterSettings.contains(filter))
    {
        return true;
    }
    return filterSettings.value(filter);
}

void Preferences::setFilterSetting(QString filter, bool enabled)
{
    filterSettings.insert(filter, enabled);
}


QString Preferences::getLanguage() {return language;}
void Preferences::setLanguage(QString lang) {language = lang;}
QPoint Preferences::getPos() {return pos;}
void Preferences::setPos(QPoint p) {pos = p;}
QSize Preferences::getSize() {return size;}
void Preferences::setSize(QSize s) {size = s;}
QByteArray Preferences::getWindowState() {return windowState;}
void Preferences::setWindowState(QByteArray ws) {windowState = ws;}
