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

class SelectionManager : public QObject
{
    Q_OBJECT
public:
    explicit SelectionManager(QObject* parent = nullptr);
    void setDocument(PlanProDocument* doc);
    void setScene(GraphicsScene* scene);
    void setFavoriteListWidget(QListWidget* favList);
    void setReferenceListWidget(QTreeWidget* refList);
    void setDocumentTreeView(QTreeView* docTreeView);
    void setObjectListView(QTreeView* objListView);
    QList<DomItem*> getSelectedItems();


public slots:
    void selectItems(const QStringList& ids);
    void selectItem(const QString& id);
    void changeViewMode(MainWindow::ViewMode mode);
    void clearSelection();

signals:
    void selectionChanged(QList<DomItem*> objectlist);


private slots:
    void handleGraphicsSceneSelection();
    void handleFavoriteListSelection();
    void handleReferenceListSelection();
    void handleDocumentTreeSelection(const QItemSelection& selected, const QItemSelection& deselected);
    void handleObjectListSelection(const QItemSelection& selected, const QItemSelection& deselected);


private:
    void updateGraphicsSceneSelection();
    void updateFavoriteListSelection();
    void updateReferenceListSelection();
    void updateReferenceListSelectionRec(QTreeWidgetItem* item, QString id);
    void updateDocumentTreeSelection();
    void updateObjectListSelection();

    QList<DomItem*> selectedItemList;
    PlanProDocument* document;
    GraphicsScene* graphicsScene;
    QListWidget* favoriteListWidget;
    QTreeWidget* referenceListWidget;
    QTreeView* documentTreeView;
    QTreeView* objectListView;

    MainWindow::ViewMode viewMode;
    bool selectionChangeInProgress;

};

#endif // SELECTIONMANAGER_H
