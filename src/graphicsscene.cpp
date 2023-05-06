#include <QtGui>
#include <QtWidgets>
#include "graphicsscene.h"

GraphicsScene::GraphicsScene(QObject* parent) : QGraphicsScene(parent)
{
    viewMode = MainWindow::ViewModeStateEnd;
}

GraphicsScene::~GraphicsScene()
{
}

void GraphicsScene::changeViewMode(MainWindow::ViewMode mode)
{
    viewMode = mode;
    QList<QGraphicsItem*> graphicsItemList = items();
    for(int i = 0; i < graphicsItemList.count(); ++i)
    {
        QGraphicsItem* item = graphicsItemList.at(i);
        QString itemType = item->data(GRAPHICSITEM_TYPE).toString();
        MainWindow::ViewMode itemviewMode = item->data(GRAPHICSITEM_VIEWMODE).value<MainWindow::ViewMode>();
        if(itemviewMode == viewMode)
        {
            item->setVisible(filterSettings.value(itemType,true));
        }
        else
        {
            item->setVisible(false);
        }
    }
}

void GraphicsScene::changeFilterSettings(const QString& key, bool state)
{
    QString mod_key(key);
    mod_key.remove(QChar('&'));
    filterSettings.insert(mod_key, state);
    QList<QGraphicsItem*> graphicsItemList = items();
    for(int i = 0; i < graphicsItemList.count(); ++i)
    {
        QGraphicsItem* item = graphicsItemList.at(i);
        QString itemType = item->data(GRAPHICSITEM_TYPE).toString();
        MainWindow::ViewMode itemviewMode = item->data(GRAPHICSITEM_VIEWMODE).value<MainWindow::ViewMode>();
        if(itemType == mod_key)
        {
            item->setVisible(state && (itemviewMode == viewMode));
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
        if((item->data(GRAPHICSITEM_ID)).toString() == id && item->data(GRAPHICSITEM_VIEWMODE) == viewMode)
        {
            return item;
        }
    }
    return NULL;
}

//void GraphicsScene::unselectAllItems()
//{
//    QList<QGraphicsItem*> graphicsItemList = items();
//    for(int i = 0; i < graphicsItemList.count(); ++i)
//    {
//        QGraphicsItem* item = graphicsItemList.at(i);
//        item->setSelected(false);
//    }
//}
