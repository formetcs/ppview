#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QtCore>


class Preferences
{
public:
    static Preferences* getInstance();

    void readSettings();
    void writeSettings();

    QString getLanguage();
    void setLanguage(QString lang);
    QPoint getPos();
    void setPos(QPoint p);
    QSize getSize();
    void setSize(QSize s);
    QByteArray getWindowState();
    void setWindowState(QByteArray ws);
    bool getFilterSetting(QString filter);
    void setFilterSetting(QString filter, bool enabled);

protected:
    Preferences();

private:
    static Preferences* preferences;

    QString language;
    QPoint pos;
    QSize size;
    QByteArray windowState;
    QHash<QString, bool> filterSettings;
};

#endif // PREFERENCES_H
