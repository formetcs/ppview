#include "graphicsscene.h"
#include "selectionmanager.h"

#include <QListWidget>
#include <QTreeView>

SelectionManager::SelectionManager(QObject* parent)
    : QObject{parent}
{
    document = NULL;
    graphicsScene = NULL;
    favoriteListWidget = NULL;
    referenceListWidget = NULL;
    documentTreeView = NULL;
    objectListView = NULL;
}

void SelectionManager::setDocument(PlanProDocument* doc)
{
    document = doc;
}

void SelectionManager::setScene(GraphicsScene* scene)
{
    if(graphicsScene)
    {
        disconnect(graphicsScene, SIGNAL(selectionChanged()), this, SLOT(handleGraphicsSceneSelection()));
    }
    graphicsScene = scene;
    if(graphicsScene)
    {
        connect(graphicsScene, SIGNAL(selectionChanged()), this, SLOT(handleGraphicsSceneSelection()));
    }
}

void SelectionManager::setFavoriteListWidget(QListWidget* favList)
{
    if(favoriteListWidget)
    {
        disconnect(favoriteListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(handleFavoriteListSelection()));
    }
    favoriteListWidget = favList;
    if(favoriteListWidget)
    {
        connect(favoriteListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(handleFavoriteListSelection()));
    }
}

void SelectionManager::setReferenceListWidget(QListWidget* refList)
{
    if(referenceListWidget)
    {
        disconnect(referenceListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(handleReferenceListSelection()));
    }
    referenceListWidget = refList;
    if(referenceListWidget)
    {
        connect(referenceListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(handleReferenceListSelection()));
    }
}

void SelectionManager::setDocumentTreeView(QTreeView* docTreeView)
{
    if(documentTreeView)
    {
        disconnect(documentTreeView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(handleDocumentTreeSelection(QItemSelection,QItemSelection)));
    }
    documentTreeView = docTreeView;
    if(documentTreeView)
    {
        connect(documentTreeView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(handleDocumentTreeSelection(QItemSelection,QItemSelection)));
    }
}

void SelectionManager::setObjectListView(QTreeView* objListView)
{
    if(objectListView)
    {
        disconnect(objectListView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(handleObjectListSelection(QItemSelection,QItemSelection)));
    }
    objectListView = objListView;
    if(objectListView)
    {
        connect(objectListView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(handleObjectListSelection(QItemSelection,QItemSelection)));
    }
}

QList<DomItem*> SelectionManager::getSelectedItems()
{
    return selectedItemList;
}

void SelectionManager::selectItems(const QStringList& ids)
{
    qDebug("SelectionManager::selectItems");

}

void SelectionManager::selectItem(const QString& id)
{
    qDebug("SelectionManager::selectItem");

//    if(selectionSource != SelectionSourceNotSelected)
//    {
//        return;
//    }
//    if(selectionSource == SelectionSourceNotSelected)
//    {
//        selectionSource = SelectionSourceExternal;
//    }

//    QModelIndex index = model->getModelIndexById(id);
//    if(!index.isValid())
//    {
//        QMessageBox::critical(0, tr("Search Error"),
//                        tr("Object with GUID\n%1\ncould not be found")
//                        .arg(id));

//        selectionSource = SelectionSourceNotSelected;
//        return;
//    }
//    QItemSelectionModel* selectionModel = objectTreeView->selectionModel();
//    selectionModel->select(index, QItemSelectionModel::ClearAndSelect);
//    objectTreeView->scrollTo(index, QAbstractItemView::EnsureVisible);

//    QList<QGraphicsItem*> graphicsItemList = scene->getGraphicsScene()->items();
//    for(int i = 0; i < graphicsItemList.count(); i++)
//    {
//        QGraphicsItem* item = graphicsItemList[i];
//        if((item->data(GRAPHICSITEM_ID)).toString() == id)
//        {
//            item->setSelected(true);
//            view->ensureVisible(item);
//        }
//        else
//        {
//            item->setSelected(false);
//        }
//    }

//    for(int i = 0; i < favoriteList->count(); i++)
//    {
//        QListWidgetItem* item = favoriteList->item(i);
//        if(item->text().contains(id))
//        {
//            item->setSelected(true);
//            favoriteList->scrollToItem(item);
//        }
//        else
//        {
//            item->setSelected(false);
//        }
//    }

//    for(int i = 0; i < referenceList->count(); i++)
//    {
//        QListWidgetItem* item = referenceList->item(i);
//        if(item->text().contains(id))
//        {
//            item->setSelected(true);
//            referenceList->scrollToItem(item);
//        }
//        else
//        {
//            item->setSelected(false);
//        }
//    }

//    if(selectionSource == SelectionSourceExternal)
//    {
//        selectionSource = SelectionSourceNotSelected;
//    }
}

void SelectionManager::handleGraphicsSceneSelection()
{
    qDebug("SelectionManager::handleGraphicsSceneSelection");

//    if(selectionSource != SelectionSourceNotSelected)
//    {
//        return;
//    }
//    if(selectionSource == SelectionSourceNotSelected)
//    {
//        selectionSource = SelectionSourceGraphicsView;
//    }

//    QList<QGraphicsItem*> itemlist = scene->getGraphicsScene()->selectedItems();
//    QItemSelectionModel* selectionModel = objectTreeView->selectionModel();
//    QItemSelection selection;
//    favoriteList->clearSelection();
//    referenceList->clearSelection();
//    for(int i = 0; i < itemlist.count(); i++)
//    {
//        QString id = itemlist[i]->data(GRAPHICSITEM_ID).toString();
//        QModelIndex index = model->getModelIndexById(id);
//        if(index.isValid())
//        {
//            selection.select(index, index);
//            objectTreeView->scrollTo(index, QAbstractItemView::EnsureVisible);
//        }
//        //std::cout << "handleGraphicsSceneSelection: " << qPrintable(id) << std::endl;

//        for(int i = 0; i < favoriteList->count(); i++)
//        {
//            QListWidgetItem* item = favoriteList->item(i);
//            if(item->text().contains(id))
//            {
//                item->setSelected(true);
//                favoriteList->scrollToItem(item);
//            }
//        }

//        for(int i = 0; i < referenceList->count(); i++)
//        {
//            QListWidgetItem* item = referenceList->item(i);
//            if(item->text().contains(id))
//            {
//                item->setSelected(true);
//                referenceList->scrollToItem(item);
//            }
//        }
//    }
//    selectionModel->select(selection, QItemSelectionModel::ClearAndSelect);

//    if(selectionSource == SelectionSourceGraphicsView)
//    {
//        selectionSource = SelectionSourceNotSelected;
//    }
}

void SelectionManager::handleFavoriteListSelection()
{
    qDebug("SelectionManager::handleFavoriteListSelection");

//    if(selectionSource != SelectionSourceNotSelected)
//    {
//        return;
//    }
//    if(selectionSource == SelectionSourceNotSelected)
//    {
//        selectionSource = SelectionSourceFavoriteList;
//    }

//    QItemSelectionModel* selectionModel = objectTreeView->selectionModel();
//    QItemSelection selection;
//    referenceList->clearSelection();
//    scene->getGraphicsScene()->clearSelection();

//    QList<QListWidgetItem*> selectedList = favoriteList->selectedItems();
//    for(int i = 0; i < selectedList.count(); i++)
//    {
//        QString itemtext = selectedList.at(i)->text();
//        int charindex = itemtext.indexOf("[");
//        QString id = itemtext.mid(charindex + 1, 36);

//        QModelIndex index = model->getModelIndexById(id);
//        if(index.isValid())
//        {
//            selection.select(index, index);
//            objectTreeView->scrollTo(index, QAbstractItemView::EnsureVisible);
//        }

//        QGraphicsItem* graphicsitem = scene->getItemById(id);
//        if(graphicsitem != NULL)
//        {
//            graphicsitem->setSelected(true);
//            view->ensureVisible(graphicsitem);
//        }

//        for(int j = 0; j < referenceList->count(); j++)
//        {
//            QListWidgetItem* item = referenceList->item(j);
//            if(item->text().contains(id))
//            {
//                item->setSelected(true);
//                referenceList->scrollToItem(item);
//            }
//        }
//    }
//    selectionModel->select(selection, QItemSelectionModel::ClearAndSelect);

//    if(selectionSource == SelectionSourceFavoriteList)
//    {
//        selectionSource = SelectionSourceNotSelected;
//    }
}

void SelectionManager::handleReferenceListSelection()
{
    qDebug("SelectionManager::handleReferenceListSelection");

//    if(selectionSource != SelectionSourceNotSelected)
//    {
//        return;
//    }
//    if(selectionSource == SelectionSourceNotSelected)
//    {
//        selectionSource = SelectionSourceReferenceList;
//    }

//    QItemSelectionModel* selectionModel = objectTreeView->selectionModel();
//    QItemSelection selection;
//    favoriteList->clearSelection();
//    scene->getGraphicsScene()->clearSelection();

//    QList<QListWidgetItem*> selectedList = referenceList->selectedItems();
//    for(int i = 0; i < selectedList.count(); i++)
//    {
//        QString itemtext = selectedList.at(i)->text();
//        int charindex = itemtext.indexOf("[");
//        QString id = itemtext.mid(charindex + 1, 36);

//        QModelIndex index = model->getModelIndexById(id);
//        if(index.isValid())
//        {
//            selection.select(index, index);
//            objectTreeView->scrollTo(index, QAbstractItemView::EnsureVisible);
//        }

//        QGraphicsItem* graphicsitem = scene->getItemById(id);
//        if(graphicsitem != NULL)
//        {
//            graphicsitem->setSelected(true);
//            view->ensureVisible(graphicsitem);
//        }

//        for(int j = 0; j < favoriteList->count(); j++)
//        {
//            QListWidgetItem* item = favoriteList->item(j);
//            if(item->text().contains(id))
//            {
//                item->setSelected(true);
//                favoriteList->scrollToItem(item);
//            }
//        }
//    }
//    selectionModel->select(selection, QItemSelectionModel::ClearAndSelect);

//    if(selectionSource == SelectionSourceReferenceList)
//    {
//        selectionSource = SelectionSourceNotSelected;
//    }
}

void SelectionManager::handleDocumentTreeSelection(const QItemSelection& selected, const QItemSelection& deselected)
{
    qDebug("SelectionManager::handleDocumentTreeSelection");

}

void SelectionManager::handleObjectListSelection(const QItemSelection& selected, const QItemSelection& deselected)
{
    qDebug("SelectionManager::handleObjectListSelection");

//    if(selectionSource != SelectionSourceNotSelected)
//    {
//        return;
//    }
//    if(selectionSource == SelectionSourceNotSelected)
//    {
//        selectionSource = SelectionSourceObjectList;
//    }

//    QModelIndex index;
//    QModelIndexList items = selected.indexes();

//    foreach (index, items)
//    {
//        QModelIndex parentIndex = model->parent(index);
//        while(parentIndex.isValid())
//        {
//            index = parentIndex;
//            parentIndex = model->parent(index);
//        }
//        DomItem* domitem = static_cast<DomItem*>(index.internalPointer());
//        QString id = domitem->node().firstChildElement("Identitaet").firstChildElement("Wert").text();
//        QGraphicsItem* graphicsitem = scene->getItemById(id);
//        if(graphicsitem != NULL)
//        {
//            graphicsitem->setSelected(true);
//            view->ensureVisible(graphicsitem);
//        }
//        //QString text = QString("Selected: (%1,%2)").arg(index.row()).arg(index.column());
//        //std::cout << qPrintable(text) << std::endl;

//        for(int i = 0; i < favoriteList->count(); i++)
//        {
//            QListWidgetItem* item = favoriteList->item(i);
//            if(item->text().contains(id))
//            {
//                item->setSelected(true);
//                favoriteList->scrollToItem(item);
//            }
//        }

//        for(int i = 0; i < referenceList->count(); i++)
//        {
//            QListWidgetItem* item = referenceList->item(i);
//            if(item->text().contains(id))
//            {
//                item->setSelected(true);
//                referenceList->scrollToItem(item);
//            }
//        }
//    }

//    items = deselected.indexes();

//    foreach (index, items)
//    {
//        QModelIndex parentIndex = model->parent(index);
//        while(parentIndex.isValid())
//        {
//            index = parentIndex;
//            parentIndex = model->parent(index);
//        }
//        DomItem* domitem = static_cast<DomItem*>(index.internalPointer());
//        QString id = domitem->node().firstChildElement("Identitaet").firstChildElement("Wert").text();
//        QGraphicsItem* graphicsitem = scene->getItemById(id);
//        if(graphicsitem != NULL)
//        {
//            graphicsitem->setSelected(false);
//        }

//        //QString text = QString("Deselected: (%1,%2)").arg(index.row()).arg(index.column());
//        //std::cout << qPrintable(text) << std::endl;

//        for(int i = 0; i < favoriteList->count(); i++)
//        {
//            QListWidgetItem* item = favoriteList->item(i);
//            if(item->text().contains(id))
//            {
//                item->setSelected(false);
//            }
//        }

//        for(int i = 0; i < referenceList->count(); i++)
//        {
//            QListWidgetItem* item = referenceList->item(i);
//            if(item->text().contains(id))
//            {
//                item->setSelected(false);
//            }
//        }
//    }

//    if(selectionSource == SelectionSourceObjectList)
//    {
//        selectionSource = SelectionSourceNotSelected;
//    }
}
