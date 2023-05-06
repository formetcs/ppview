#include "graphicsscene.h"
#include "objectlistmodel.h"
#include "planprodocument.h"
#include "planpromodel.h"
#include "selectionmanager.h"

#include <QGraphicsItem>
#include <QGraphicsView>
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
    viewMode = MainWindow::ViewModeStateEnd;
    selectionChangeInProgress = false;
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

void SelectionManager::changeViewMode(MainWindow::ViewMode mode)
{
    viewMode = mode;
}

void SelectionManager::selectItems(const QStringList& ids)
{
    if(selectionChangeInProgress)
    {
        return;
    }
    selectionChangeInProgress = true;

    selectedItemList.clear();
    PlanProDocument::PlanningState state = (viewMode == MainWindow::ViewModeStateStart) ? PlanProDocument::PlanningStateStart : PlanProDocument::PlanningStateEnd;
    for(int i = 0; i < ids.count(); ++i)
    {
        QString currentId = ids.at(i);
        DomItem* domItem = document->getObjectById(currentId, state);
        if(!domItem && viewMode == MainWindow::ViewModeStateComparison)
        {
            domItem = document->getObjectById(currentId, PlanProDocument::PlanningStateStart);
        }
        if(domItem)
        {
            selectedItemList.append(domItem);
        }
    }

    updateGraphicsSceneSelection();
    updateFavoriteListSelection();
    updateReferenceListSelection();
    updateDocumentTreeSelection();
    updateObjectListSelection();

    emit selectionChanged(selectedItemList);
    selectionChangeInProgress = false;
}

void SelectionManager::selectItem(const QString& id)
{
    QStringList list(id);
    selectItems(list);
}

void SelectionManager::handleGraphicsSceneSelection()
{
    if(selectionChangeInProgress)
    {
        return;
    }
    selectionChangeInProgress = true;

    selectedItemList.clear();
    PlanProDocument::PlanningState state = (viewMode == MainWindow::ViewModeStateStart) ? PlanProDocument::PlanningStateStart : PlanProDocument::PlanningStateEnd;
    QList<QGraphicsItem*> itemlist = graphicsScene->selectedItems();
    for(int i = 0; i < itemlist.count(); ++i)
    {
        QGraphicsItem* item = itemlist.at(i);
        QString currentId = item->data(GRAPHICSITEM_ID).toString();
        DomItem* domItem = document->getObjectById(currentId, state);
        if(!domItem && viewMode == MainWindow::ViewModeStateComparison)
        {
            domItem = document->getObjectById(currentId, PlanProDocument::PlanningStateStart);
        }
        if(domItem)
        {
            selectedItemList.append(domItem);
        }
    }

    updateFavoriteListSelection();
    updateReferenceListSelection();
    updateDocumentTreeSelection();
    updateObjectListSelection();

    emit selectionChanged(selectedItemList);
    selectionChangeInProgress = false;
}

void SelectionManager::handleFavoriteListSelection()
{
    if(selectionChangeInProgress)
    {
        return;
    }
    selectionChangeInProgress = true;

    selectedItemList.clear();
    PlanProDocument::PlanningState state = (viewMode == MainWindow::ViewModeStateStart) ? PlanProDocument::PlanningStateStart : PlanProDocument::PlanningStateEnd;
    QList<QListWidgetItem*> selectedList = favoriteListWidget->selectedItems();
    for(int i = 0; i < selectedList.count(); ++i)
    {
        QString itemtext = selectedList.at(i)->text();
        int charindex = itemtext.indexOf("[");
        QString currentId = itemtext.mid(charindex + 1, 36);
        DomItem* domItem = document->getObjectById(currentId, state);
        if(!domItem && viewMode == MainWindow::ViewModeStateComparison)
        {
            domItem = document->getObjectById(currentId, PlanProDocument::PlanningStateStart);
        }
        if(domItem)
        {
            selectedItemList.append(domItem);
        }
    }

    updateGraphicsSceneSelection();
    updateReferenceListSelection();
    updateDocumentTreeSelection();
    updateObjectListSelection();

    emit selectionChanged(selectedItemList);
    selectionChangeInProgress = false;
}

void SelectionManager::handleReferenceListSelection()
{
    if(selectionChangeInProgress)
    {
        return;
    }
    selectionChangeInProgress = true;

    selectedItemList.clear();
    PlanProDocument::PlanningState state = (viewMode == MainWindow::ViewModeStateStart) ? PlanProDocument::PlanningStateStart : PlanProDocument::PlanningStateEnd;
    QList<QListWidgetItem*> selectedList = referenceListWidget->selectedItems();
    for(int i = 0; i < selectedList.count(); ++i)
    {
        QString itemtext = selectedList.at(i)->text();
        int charindex = itemtext.indexOf("[");
        QString currentId = itemtext.mid(charindex + 1, 36);
        DomItem* domItem = document->getObjectById(currentId, state);
        if(!domItem && viewMode == MainWindow::ViewModeStateComparison)
        {
            domItem = document->getObjectById(currentId, PlanProDocument::PlanningStateStart);
        }
        if(domItem)
        {
            selectedItemList.append(domItem);
        }
    }

    updateGraphicsSceneSelection();
    updateFavoriteListSelection();
    updateDocumentTreeSelection();
    updateObjectListSelection();

    emit selectionChanged(selectedItemList);
    selectionChangeInProgress = false;
}

void SelectionManager::handleDocumentTreeSelection(const QItemSelection& selected, const QItemSelection& deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);

    if(selectionChangeInProgress)
    {
        return;
    }
    selectionChangeInProgress = true;

    selectedItemList.clear();
    QItemSelectionModel* selectionModel = documentTreeView->selectionModel();
    QModelIndexList selectedList = selectionModel->selectedIndexes();
    for(int i = 0; i < selectedList.count(); ++i)
    {
        QModelIndex index = selectedList.at(i);
        DomItem* domItem = static_cast<DomItem*>(index.internalPointer());
        while(domItem)
        {
            DomItem* parentItem = domItem->parent();
            if(!parentItem)
            {
                break;
            }
            QString itemId = domItem->getFirstValueAtPath("Identitaet/Wert");
            QString parentName = parentItem->getName();
            if(!itemId.isEmpty() && parentName == "Container")
            {
                selectedItemList.append(domItem);
                break;
            }
            else
            {
                domItem = parentItem;
            }
        }
    }

    updateGraphicsSceneSelection();
    updateFavoriteListSelection();
    updateReferenceListSelection();
    updateObjectListSelection();

    emit selectionChanged(selectedItemList);
    selectionChangeInProgress = false;
}

void SelectionManager::handleObjectListSelection(const QItemSelection& selected, const QItemSelection& deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);

    if(selectionChangeInProgress)
    {
        return;
    }
    selectionChangeInProgress = true;

    selectedItemList.clear();
    QItemSelectionModel* selectionModel = objectListView->selectionModel();
    ObjectListModel* olModel = static_cast<ObjectListModel*>(objectListView->model());
    QModelIndexList selectedList = selectionModel->selectedIndexes();
    for(int i = 0; i < selectedList.count(); ++i)
    {
        QModelIndex index = selectedList.at(i);
        if(index.column() != 0)
        {
            continue;
        }
        DomItem* item = olModel->getItem(index);
        if(item)
        {
            selectedItemList.append(item);
        }
    }

    updateGraphicsSceneSelection();
    updateFavoriteListSelection();
    updateReferenceListSelection();
    updateDocumentTreeSelection();

    emit selectionChanged(selectedItemList);
    selectionChangeInProgress = false;
}

void SelectionManager::updateGraphicsSceneSelection()
{
    graphicsScene->clearSelection();
    QGraphicsItem* lastGraphicsItem = NULL;
    for(int i = 0; i < selectedItemList.count(); ++i)
    {
        DomItem* currentItem = selectedItemList.at(i);
        QString currentId = currentItem->getFirstValueAtPath("Identitaet/Wert");
        QGraphicsItem* graphicsItem = graphicsScene->getItemById(currentId);
        if(graphicsItem)
        {
            graphicsItem->setSelected(true);
            lastGraphicsItem = graphicsItem;
        }
    }
    if(lastGraphicsItem)
    {
        QList<QGraphicsView *> viewlist = graphicsScene->views();
        for(int i = 0; i < viewlist.count(); ++i)
        {
            QGraphicsView* view = viewlist.at(i);
            view->centerOn(lastGraphicsItem);
        }
    }
}

void SelectionManager::updateFavoriteListSelection()
{
    favoriteListWidget->clearSelection();
    for(int i = 0; i < selectedItemList.count(); ++i)
    {
        DomItem* currentItem = selectedItemList.at(i);
        QString currentId = currentItem->getFirstValueAtPath("Identitaet/Wert");
        for(int j = 0; j < favoriteListWidget->count(); ++j)
        {
            QListWidgetItem* item = favoriteListWidget->item(j);
            if(item->text().contains(currentId))
            {
                item->setSelected(true);
                favoriteListWidget->scrollToItem(item);
            }
        }
    }
}

void SelectionManager::updateReferenceListSelection()
{
    referenceListWidget->clearSelection();
    for(int i = 0; i < selectedItemList.count(); ++i)
    {
        DomItem* currentItem = selectedItemList.at(i);
        QString currentId = currentItem->getFirstValueAtPath("Identitaet/Wert");
        for(int j = 0; j < referenceListWidget->count(); ++j)
        {
            QListWidgetItem* item = referenceListWidget->item(j);
            if(item->text().contains(currentId))
            {
                item->setSelected(true);
                referenceListWidget->scrollToItem(item);
            }
        }
    }
}

void SelectionManager::updateDocumentTreeSelection()
{
    QItemSelectionModel* selectionModel = documentTreeView->selectionModel();
    PlanProModel* model = static_cast<PlanProModel*>(documentTreeView->model());
    QItemSelection selection;

    for(int i = 0; i < selectedItemList.count(); ++i)
    {
        DomItem* currentItem = selectedItemList.at(i);
        QModelIndex index = model->getModelIndex(currentItem);
        if(index.isValid())
        {
            selection.select(index, index);
            documentTreeView->scrollTo(index, QAbstractItemView::EnsureVisible);
        }
    }
    selectionModel->select(selection, QItemSelectionModel::ClearAndSelect);
}

void SelectionManager::updateObjectListSelection()
{
    QItemSelectionModel* selectionModel = objectListView->selectionModel();
    ObjectListModel* olModel = static_cast<ObjectListModel*>(objectListView->model());
    QItemSelection selection;

    for(int i = 0; i < selectedItemList.count(); ++i)
    {
        DomItem* currentItem = selectedItemList.at(i);
        QString currentId = currentItem->getFirstValueAtPath("Identitaet/Wert");
        QModelIndex index = olModel->getModelIndexById(currentId);
        if(index.isValid())
        {
            selection.select(index, index);
            objectListView->scrollTo(index, QAbstractItemView::EnsureVisible);
        }
    }
    selectionModel->select(selection, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
}
