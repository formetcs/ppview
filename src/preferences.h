#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QBrush>
#include <QPen>
#include <QtCore>


class Preferences
{
public:
    static Preferences* getInstance();

    void readSettings();
    void writeSettings();
    void setDefaultValues();

    QPoint getPos();
    void setPos(QPoint p);
    QSize getSize();
    void setSize(QSize s);
    QByteArray getWindowState();
    void setWindowState(QByteArray ws);

    QString getLanguage();
    void setLanguage(QString lang);
    int getZoomStep();
    void setZoomStep(int step);
    int getMinZoom();
    void setMinZoom(int min);
    int getRotateStep();
    void setRotateStep(int step);
    int getMaxRecursionDepth();
    void setMaxRecursionDepth(int depth);

    bool getFilterSetting(QString filter);
    void setFilterSetting(QString filter, bool enabled);

    QPen getGraphicsViewPen(QString type);
    void setGraphicsViewPen(QString type, QPen pen);
    QBrush getGraphicsViewBrush(QString type);
    void setGraphicsViewBrush(QString type, QBrush brush);

    QBrush getObjectListFgStartBrush();
    void setObjectListFgStartBrush(QBrush brush);
    QBrush getObjectListBgStartBrush();
    void setObjectListBgStartBrush(QBrush brush);
    QBrush getObjectListFgEndBrush();
    void setObjectListFgEndBrush(QBrush brush);
    QBrush getObjectListBgEndBrush();
    void setObjectListBgEndBrush(QBrush brush);
    QBrush getObjectListFgBothBrush();
    void setObjectListFgBothBrush(QBrush brush);
    QBrush getObjectListBgBothBrush();
    void setObjectListBgBothBrush(QBrush brush);

    QPen getGraphicsViewStartPen();
    void setGraphicsViewStartPen(QPen pen);
    QBrush getGraphicsViewStartBrush();
    void setGraphicsViewStartBrush(QBrush brush);
    QPen getGraphicsViewEndPen();
    void setGraphicsViewEndPen(QPen pen);
    QBrush getGraphicsViewEndBrush();
    void setGraphicsViewEndBrush(QBrush brush);
    QPen getGraphicsViewBothPen();
    void setGraphicsViewBothPen(QPen pen);
    QBrush getGraphicsViewBothBrush();
    void setGraphicsViewBothBrush(QBrush brush);

protected:
    Preferences();

private:
    static Preferences* preferences;

    QPoint pos;
    QSize size;
    QByteArray windowState;

    QString language;
    int zoomStep;
    int minZoom;
    int rotateStep;
    int maxRecursionDepth;

    QHash<QString, bool> filterSettings;

    QHash<QString, QPen> graphicsViewPens;
    QHash<QString, QBrush> graphicsViewBrushes;

    QBrush objectListFgStartBrush;
    QBrush objectListBgStartBrush;
    QBrush objectListFgEndBrush;
    QBrush objectListBgEndBrush;
    QBrush objectListFgBothBrush;
    QBrush objectListBgBothBrush;

    QPen graphicsViewStartPen;
    QBrush graphicsViewStartBrush;
    QPen graphicsViewEndPen;
    QBrush graphicsViewEndBrush;
    QPen graphicsViewBothPen;
    QBrush graphicsViewBothBrush;


};

#endif // PREFERENCES_H
