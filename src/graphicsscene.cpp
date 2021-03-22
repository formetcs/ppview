#include <QtGui>
#include <QtWidgets>
#include "graphicsscene.h"

GraphicsScene::GraphicsScene(QObject *parent) : QObject(parent)
{
    graphicsScene = new QGraphicsScene();
    connect(graphicsScene, SIGNAL(selectionChanged()), this, SLOT(handleItemSelection()));
}

GraphicsScene::~GraphicsScene()
{
    delete graphicsScene;
}

QGraphicsScene* GraphicsScene::getGraphicsScene()
{
    return graphicsScene;
}

void GraphicsScene::changeFilterSettings(QString key, bool state)
{
    QList<QGraphicsItem*> graphicsItemList = graphicsScene->items();
    for(int i = 0; i < graphicsItemList.count(); i++)
    {
        QGraphicsItem* item = graphicsItemList[i];
        if((item->data(GRAPHICSITEM_TYPE)).toString() == key.remove(QChar('&')))
        {
            item->setVisible(state);
        }
    }
}

void GraphicsScene::changeFilterSettings(QList<FilterState> statelist)
{
    for(int i = 0; i < statelist.count(); i++)
    {
        FilterState fs = statelist.at(i);
        changeFilterSettings(fs.name, fs.state);
    }
}

QGraphicsItem* GraphicsScene::getItemById(QString id)
{
    QList<QGraphicsItem*> graphicsItemList = graphicsScene->items();
    for(int i = 0; i < graphicsItemList.count(); i++)
    {
        QGraphicsItem* item = graphicsItemList[i];
        if((item->data(GRAPHICSITEM_ID)).toString() == id)
        {
            return item;
        }
    }
    return NULL;
}

void GraphicsScene::unselectAllItems()
{
    QList<QGraphicsItem*> graphicsItemList = graphicsScene->items();
    for(int i = 0; i < graphicsItemList.count(); i++)
    {
        QGraphicsItem* item = graphicsItemList[i];
        item->setSelected(false);
    }
}

void GraphicsScene::handleItemSelection()
{
    QList<QGraphicsItem*> itemlist = graphicsScene->selectedItems();
    QString resultstring = QString();
    for(int i = 0; i < itemlist.count(); i++)
    {
        resultstring.append(itemlist[i]->data(GRAPHICSITEM_INFOTEXT).toString());
        resultstring.append("<br>");
    }

    emit sendObjectInfo(resultstring);
}
