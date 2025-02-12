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

#ifndef SELECTIONMANAGER_H
#define SELECTIONMANAGER_H

#include "domitem.h"
#include "mainwindow.h"

#include <QObject>

class QTreeWidgetItem;
class QTreeView;
class QListWidget;
class QTreeWidget;
class QItemSelection;
class GraphicsScene;
class PlanProDocument;


/*!
 * \brief Class to synchronize item selections between different views.
 */
class SelectionManager : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief Create a new SelectionManager.
     *
     * \param parent the parent object
     */
    explicit SelectionManager(QObject* parent = nullptr);

    /*!
     * \brief Set the PlanPro document.
     *
     * \param doc the PlanPro document
     */
    void setDocument(PlanProDocument* doc);

    /*!
     * \brief Set the graphics scene.
     *
     * \param scene the graphics scene
     */
    void setScene(GraphicsScene* scene);

    /*!
     * \brief Set the bookmark list widget.
     *
     * \param bmList the bookmark list widget
     */
    void setBookmarkListWidget(QListWidget* bmList);

    /*!
     * \brief Set the search result list widget.
     *
     * \param resList the search result list widget
     */
    void setSearchResultListWidget(QTreeWidget* resList);

    /*!
     * \brief Set the document tree view.
     *
     * \param docTreeView the document tree view
     */
    void setDocumentTreeView(QTreeView* docTreeView);

    /*!
     * \brief Set the object list view.
     *
     * \param objListView the object list view
     */
    void setObjectListView(QTreeView* objListView);

    /*!
     * \brief Get the selected items.
     *
     * \return the list of selected items
     */
    QList<DomItem*> getSelectedItems();


public slots:
    /*!
     * \brief Select the items with the given IDs.
     *
     * \param ids a string list containing the UUIDs of the items to be selected
     */
    void selectItems(const QStringList& ids);

    /*!
     * \brief Select a single item with a given ID.
     *
     * \param id the UUID string of the item to be selected
     */
    void selectItem(const QString& id);

    /*!
     * \brief Change the view mode.
     *
     * \param mode the new view mode
     */
    void changeViewMode(MainWindow::ViewMode mode);

    /*!
     * \brief Clear all selections.
     */
    void clearSelection();

signals:
    /*!
     * \brief Signal to indicate that the selection has changed.
     *
     * \param objectlist the list of the currently selected items
     */
    void selectionChanged(QList<DomItem*> objectlist);


private slots:
    /*!
     * \brief Process Selection changes from the graphics scene.
     *
     * This method is invoked when the selection is changed in the graphics scene
     * and propagates the changes to the other views.
     */
    void handleGraphicsSceneSelection();

    /*!
     * \brief Process Selection changes from the bookmark list.
     *
     * This method is invoked when the selection is changed in the bookmark list
     * and propagates the changes to the other views.
     */
    void handleBookmarkListSelection();

    /*!
     * \brief Process Selection changes from the result list.
     *
     * This method is invoked when the selection is changed in the result list
     * and propagates the changes to the other views.
     */
    void handleSearchResultListSelection();

    /*!
     * \brief Process Selection changes from the document tree view.
     *
     * This method is invoked when the selection is changed in the document tree view
     * and propagates the changes to the other views.
     *
     * \param selected list of newly selected items
     * \param deselected list of newly deselected items
     */
    void handleDocumentTreeSelection(const QItemSelection& selected, const QItemSelection& deselected);

    /*!
     * \brief Process Selection changes from the object list.
     *
     * This method is invoked when the selection is changed in the object list
     * and propagates the changes to the other views.
     *
     * \param selected list of newly selected items
     * \param deselected list of newly deselected items
     */
    void handleObjectListSelection(const QItemSelection& selected, const QItemSelection& deselected);


private:
    /*!
     * \brief Update the graphics scene after a selection change from another view.
     */
    void updateGraphicsSceneSelection();

    /*!
     * \brief Update the bookmark list after a selection change from another view.
     */
    void updateBookmarkListSelection();

    /*!
     * \brief Update the search result list after a selection change from another view.
     *
     * This method calls updateSearchResultListSelectionRec() recursively to update the selection of any child items.
     */
    void updateSearchResultListSelection();

    /*!
     * \brief Update the child items in the search result list recursively.
     *
     * \param item the current QTreeWidgetItem
     * \param id the UUID that should be set to selected
     */
    void updateSearchResultListSelectionRec(QTreeWidgetItem* item, QString id);

    /*!
     * \brief Update the document tree view after a selection change from another view.
     */
    void updateDocumentTreeSelection();

    /*!
     * \brief Update the object list after a selection change from another view.
     */
    void updateObjectListSelection();

    QList<DomItem*> selectedItemList; /*!< \brief The list of selected items. */
    PlanProDocument* document; /*!< \brief The PlanPro document. */
    GraphicsScene* graphicsScene; /*!< \brief The graphics scene. */
    QListWidget* bookmarkListWidget; /*!< \brief The bookmark list widget. */
    QTreeWidget* searchResultListWidget; /*!< \brief The result list widget. */
    QTreeView* documentTreeView; /*!< \brief The document tree view. */
    QTreeView* objectListView; /*!< \brief The object list view. */

    MainWindow::ViewMode viewMode; /*!< \brief The active view mode. */
    bool selectionChangeInProgress; /*!< \brief Flag to indicate that a selection change is ongoing (to avoid infinite loops). */

};

#endif // SELECTIONMANAGER_H
