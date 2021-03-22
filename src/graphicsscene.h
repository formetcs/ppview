#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QObject>
#include <QList>

#include "filterwidget.h"

class QGraphicsScene;
class QGraphicsItem;


const int GRAPHICSITEM_TYPE = 1;
const int GRAPHICSITEM_ID = 2;
const int GRAPHICSITEM_INFOTEXT = 3;


class GraphicsScene : public QObject
{
    Q_OBJECT
public:
    explicit GraphicsScene(QObject *parent = NULL);
    ~GraphicsScene();

    QGraphicsScene* getGraphicsScene();
    QGraphicsItem* getItemById(QString id);

signals:
    void sendObjectInfo(QString);

public slots:
    void changeFilterSettings(QString key, bool state);
    void changeFilterSettings(QList<FilterState> statelist);
    void unselectAllItems();

private slots:
    void handleItemSelection();

private:
    QGraphicsScene* graphicsScene;
    //QList<QGraphicsItem*> graphicsItemList;
};

#endif // GRAPHICSSCENE_H
