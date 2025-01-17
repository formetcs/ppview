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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class DomItem;
class QTreeWidgetItem;
class QTreeWidget;
class QActionGroup;
class QComboBox;
class SelectionManager;
class ObjectInfoWidget;
class QAction;
class QSpinBox;
class QMenu;
class QString;
class QGraphicsView;
class QTextEdit;
class QLineEdit;
class QPushButton;
class QTreeView;
class QListView;
class QListWidget;
class FilterWidget;
class DocumentTreeModel;
class GraphicsScene;
class PlanProXmlDocument;
class GraphicsSceneBuilder;
class ObjectListModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    enum ViewMode
    {
        ViewModeStateStart = 0,
        ViewModeStateEnd = 1,
        ViewModeStateComparison = 2
    };


    MainWindow(const QString& fileName, QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* ev);
    void dragEnterEvent(QDragEnterEvent* ev);
    void dropEvent(QDropEvent* ev);

private slots:
    void openFile();
    bool saveFile();
    bool saveAs();
    void closeFile();
    void saveAsPicture();
    void saveAsPdf();
    void printFile();
    void transformGraphicsView();
    void zoomIn();
    void zoomOut();
    void rotateLeft();
    void rotateRight();
    void resetView();
    void showHelp();
    void showDocumentInfo();
    void editRemark();
    void about();
    void goToObject();
    void handleObjectSearch();
    void centerObject();
    void switchViewMode();
    void changeCategory();
    void extractFile();
    void measureDistance();
    void addToBookmarks();
    void removeFromBookmarks();
    void findReferencingObjects();
    void showPreferences();
    void showReadme();
    void showLicense();
    void show3rdPartyLicenses();

private:
    void createActions();
    void enableActions();
    void createMenus();
    void createToolBars();
    void createDockWindows();
    bool okToContinue();
    void openNamedFile(const QString& filename);
    bool saveNamedFile(const QString& filename);
    void readSettings();
    void writeSettings();
    void createReferenceListRec(DomItem* item, QTreeWidgetItem* parent, int depth);

    QMenu* fileMenu;
    QMenu* editMenu;
    QMenu* viewMenu;
    QMenu* viewDockSubmenu;
    QMenu* viewToolbarSubmenu;
    QMenu* objectMenu;
    QMenu* bookmarkMenu;
    QMenu* helpMenu;
    QToolBar* fileToolBar;
    QToolBar* editToolBar;
    QToolBar* viewToolBar;
    QToolBar* objectToolBar;
    QToolBar* bookmarkToolBar;
    QAction* openFileAct;
    QAction* saveFileAct;
    QAction* saveAsAct;
    QAction* closeFileAct;
    QAction* saveAsPictureAct;
    QAction* saveAsPdfAct;
    QAction* printFileAct;
    QAction* docInfoAct;
    QAction* exitAct;
    QAction* searchAct;
    QAction* editRemarkAct;
    QAction* centerAct;
    QAction* zoomInAct;
    QAction* zoomOutAct;
    QAction* rotateLeftAct;
    QAction* rotateRightAct;
    QAction* resetViewAct;
    QActionGroup* viewModeActGroup;
    QAction* startViewModeAct;
    QAction* endViewModeAct;
    QAction* comparisonViewModeAct;
    QAction* selectAllFiltersAct;
    QAction* deselectAllFiltersAct;
    QAction* extractFileAct;
    QAction* goToObjectAct;
    QAction* measureDistanceAct;
    QAction* findReferencingObjectsAct;
    QAction* addBookmarkAct;
    QAction* removeBookmarkAct;
    QAction* clearBookmarkListAct;
    QAction* preferencesAct;
    QAction* helpContentsAct;
    QAction* showReadmeAct;
    QAction* showLicenseAct;
    QAction* show3rdPartyLicensesAct;
    QAction* aboutAct;
    QSpinBox* scaleSpinBox;
    QSpinBox* rotateSpinBox;
    
    DocumentTreeModel* doctreemodel;
    ObjectListModel* objectlistmodel;
    GraphicsScene* scene;
    QGraphicsView* view;
    SelectionManager* selectionManager;
    ObjectInfoWidget* objectInfo;
    FilterWidget* filterWidget;
    QTreeView* documentTreeView;
    QTreeView* objectListView;
    QListWidget* bookmarkList;
    QTreeWidget* searchResultList;
    QComboBox* categoryComboBox;

    PlanProXmlDocument* document;
};

#endif // MAINWINDOW_H
