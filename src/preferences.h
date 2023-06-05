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
    int getZoomStep();
    void setZoomStep(int step);
    int getMinZoom();
    void setMinZoom(int min);
    int getRotateStep();
    void setRotateStep(int step);
    int getMaxRecursionDepth();
    void setMaxRecursionDepth(int depth);

protected:
    Preferences();

private:
    static Preferences* preferences;

    QString language;
    QPoint pos;
    QSize size;
    QByteArray windowState;
    QHash<QString, bool> filterSettings;
    int zoomStep;
    int minZoom;
    int rotateStep;
    int maxRecursionDepth;
};

#endif // PREFERENCES_H
