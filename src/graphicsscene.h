#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QObject>
#include <QList>
#include <QGraphicsScene>
#include <QHash>

#include "filterwidget.h"
#include "mainwindow.h"
//#include "planprodocument.h"

//class QGraphicsItem;


const int GRAPHICSITEM_TYPE = 1;
const int GRAPHICSITEM_ID = 2;
const int GRAPHICSITEM_VIEWMODE = 3;


class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphicsScene(QObject* parent = nullptr);
    ~GraphicsScene();

    QGraphicsItem* getItemById(const QString& id);

signals:

public slots:
    void changeFilterSettings(const QString& key, bool state);
    void changeFilterSettings(const QList<FilterState>& statelist);
    //void unselectAllItems();
    void changeViewMode(MainWindow::ViewMode mode);

private slots:

private:
    MainWindow::ViewMode viewMode;
    QHash<QString,bool> filterSettings;

};

#endif // GRAPHICSSCENE_H
