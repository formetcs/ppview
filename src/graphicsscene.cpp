#include <QtGui>
#include <QtWidgets>
#include "graphicsscene.h"

GraphicsScene::GraphicsScene(QObject *parent) : QGraphicsScene(parent)
{
}

GraphicsScene::~GraphicsScene()
{
}

void GraphicsScene::changeFilterSettings(const QString& key, bool state)
{
    QString mod_key(key);
    mod_key.remove(QChar('&'));
    QList<QGraphicsItem*> graphicsItemList = items();
    for(int i = 0; i < graphicsItemList.count(); ++i)
    {
        QGraphicsItem* item = graphicsItemList.at(i);
        if((item->data(GRAPHICSITEM_TYPE)).toString() == mod_key)
        {
            item->setVisible(state);
        }
    }
}

void GraphicsScene::changeFilterSettings(const QList<FilterState>& statelist)
{
    for(int i = 0; i < statelist.count(); ++i)
    {
        FilterState fs = statelist.at(i);
        changeFilterSettings(fs.name, fs.state);
    }
}

QGraphicsItem* GraphicsScene::getItemById(const QString& id)
{
    QList<QGraphicsItem*> graphicsItemList = items();
    for(int i = 0; i < graphicsItemList.count(); ++i)
    {
        QGraphicsItem* item = graphicsItemList.at(i);
        if((item->data(GRAPHICSITEM_ID)).toString() == id)
        {
            return item;
        }
    }
    return NULL;
}

void GraphicsScene::unselectAllItems()
{
    QList<QGraphicsItem*> graphicsItemList = items();
    for(int i = 0; i < graphicsItemList.count(); ++i)
    {
        QGraphicsItem* item = graphicsItemList.at(i);
        item->setSelected(false);
    }
}
