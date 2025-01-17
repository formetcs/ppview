/*
 * PlanPro Viewer
 *
 * Copyright (c) 2017-2025, The FormETCS Project. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "graphicsscene.h"
#include "objectlistmodel.h"
#include "planprodocument.h"
#include "documenttreemodel.h"
#include "selectionmanager.h"

#include <QGraphicsItem>
#include <QGraphicsView>
#include <QListWidget>
#include <QTreeWidget>
#include <QTreeView>

SelectionManager::SelectionManager(QObject* parent)
    : QObject{parent}
{
    document = NULL;
    graphicsScene = NULL;
    bookmarkListWidget = NULL;
    searchResultListWidget = NULL;
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

void SelectionManager::setBookmarkListWidget(QListWidget* bmList)
{
    if(bookmarkListWidget)
    {
        disconnect(bookmarkListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(handleBookmarkListSelection()));
    }
    bookmarkListWidget = bmList;
    if(bookmarkListWidget)
    {
        connect(bookmarkListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(handleBookmarkListSelection()));
    }
}

void SelectionManager::setSearchResultListWidget(QTreeWidget* resList)
{
    if(searchResultListWidget)
    {
        disconnect(searchResultListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(handleSearchResultListSelection()));
    }
    searchResultListWidget = resList;
    if(searchResultListWidget)
    {
        connect(searchResultListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(handleSearchResultListSelection()));
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
    clearSelection();
}

void SelectionManager::clearSelection()
{
    if(selectionChangeInProgress)
    {
        return;
    }
    selectionChangeInProgress = true;
    selectedItemList.clear();
    updateGraphicsSceneSelection();
    updateBookmarkListSelection();
    updateSearchResultListSelection();
    updateDocumentTreeSelection();
    updateObjectListSelection();

    emit selectionChanged(selectedItemList);
    selectionChangeInProgress = false;
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
    updateBookmarkListSelection();
    updateSearchResultListSelection();
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

    updateBookmarkListSelection();
    updateSearchResultListSelection();
    updateDocumentTreeSelection();
    updateObjectListSelection();

    emit selectionChanged(selectedItemList);
    selectionChangeInProgress = false;
}

void SelectionManager::handleBookmarkListSelection()
{
    if(selectionChangeInProgress)
    {
        return;
    }
    selectionChangeInProgress = true;

    selectedItemList.clear();
    PlanProDocument::PlanningState state = (viewMode == MainWindow::ViewModeStateStart) ? PlanProDocument::PlanningStateStart : PlanProDocument::PlanningStateEnd;
    QList<QListWidgetItem*> selectedList = bookmarkListWidget->selectedItems();
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
    updateSearchResultListSelection();
    updateDocumentTreeSelection();
    updateObjectListSelection();

    emit selectionChanged(selectedItemList);
    selectionChangeInProgress = false;
}

void SelectionManager::handleSearchResultListSelection()
{
    if(selectionChangeInProgress)
    {
        return;
    }
    selectionChangeInProgress = true;

    selectedItemList.clear();
    PlanProDocument::PlanningState state = (viewMode == MainWindow::ViewModeStateStart) ? PlanProDocument::PlanningStateStart : PlanProDocument::PlanningStateEnd;
    QList<QTreeWidgetItem*> selectedList = searchResultListWidget->selectedItems();
    for(int i = 0; i < selectedList.count(); ++i)
    {
        QString currentId = selectedList.at(i)->text(1);
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
    updateBookmarkListSelection();
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
    updateBookmarkListSelection();
    updateSearchResultListSelection();
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
    updateBookmarkListSelection();
    updateSearchResultListSelection();
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

void SelectionManager::updateBookmarkListSelection()
{
    bookmarkListWidget->clearSelection();
    for(int i = 0; i < selectedItemList.count(); ++i)
    {
        DomItem* currentItem = selectedItemList.at(i);
        QString currentId = currentItem->getFirstValueAtPath("Identitaet/Wert");
        for(int j = 0; j < bookmarkListWidget->count(); ++j)
        {
            QListWidgetItem* item = bookmarkListWidget->item(j);
            if(item->text().contains(currentId))
            {
                item->setSelected(true);
                bookmarkListWidget->scrollToItem(item);
            }
        }
    }
}

void SelectionManager::updateSearchResultListSelection()
{
    searchResultListWidget->clearSelection();
    for(int i = 0; i < selectedItemList.count(); ++i)
    {
        DomItem* currentItem = selectedItemList.at(i);
        QString currentId = currentItem->getFirstValueAtPath("Identitaet/Wert");
        for(int j = 0; j < searchResultListWidget->topLevelItemCount(); ++j)
        {
            QTreeWidgetItem* item = searchResultListWidget->topLevelItem(j);
            updateSearchResultListSelectionRec(item, currentId);
            if(item->text(1) == currentId)
            {
                item->setSelected(true);
                searchResultListWidget->scrollToItem(item);
            }
        }
    }
}

void SelectionManager::updateSearchResultListSelectionRec(QTreeWidgetItem* item, QString id)
{
    for(int i = 0; i < item->childCount(); ++i)
    {
        QTreeWidgetItem* currentItem = item->child(i);
        updateSearchResultListSelectionRec(currentItem, id);
        if(currentItem->text(1) == id)
        {
            currentItem->setSelected(true);
            searchResultListWidget->scrollToItem(currentItem);
        }
    }
}

void SelectionManager::updateDocumentTreeSelection()
{
    QItemSelectionModel* selectionModel = documentTreeView->selectionModel();
    DocumentTreeModel* model = static_cast<DocumentTreeModel*>(documentTreeView->model());
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
