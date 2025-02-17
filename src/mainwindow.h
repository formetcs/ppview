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

/*!
 * \file mainwindow.h
 *
 * \brief Declaration of the MainWindow class.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class SmtVerifierDialog;
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


/*!
 * \brief The main window of the application.
 *
 * It contains menus, tool bars and a QGraphicsView displaying the track layout as central widget (with data obtained from GraphicsScene).
 * It has also the following dock windows:
 * - a QTreeView displaying the document structure (with data from DocumentTreeModel)
 * - a QTreeView displaying the list of PlanPro objects (with data from ObjectListModel)
 * - the ObjectInfoWidget displaying the properties of the selected PlanPro object
 * - the FilterWidget to select which object types should be displayed in the track layout view
 * - a QListWidget displaying the bookmark list
 * - a QTreeWidget displaying search results, verification counterexamples and dependencies
 *
 * Selections between the different views are synchronized with the SelectionManager.
 *
 * The loaded PlanPro file is stored in the PlanProXmlDocument. It also acts as data source for the GraphicsScene, DocumentTreeModel and ObjectListModel.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    /*!
     * \brief Enum describing the active view mode.
     */
    enum ViewMode
    {
        ViewModeStateStart = 0, /*!< \brief The application is showing the start state of the planning. */
        ViewModeStateEnd = 1, /*!< \brief The application is showing the end state of the planning. */
        ViewModeStateComparison = 2 /*!< \brief The application is showing a combined view of start and end state, using black-red-yellow drawing. */
    };


    /*!
     * \brief Creates a new MainWindow.
     *
     * \param fileName name of the PlanPro file that should be loaded at program start, or an empty string if no file should be loaded
     * \param parent the parent widget
     */
    MainWindow(const QString& fileName, QWidget* parent = nullptr);

    /*!
     * \brief Destructor.
     */
    ~MainWindow();

protected:
    /*!
     * \brief This method is called before the window is closed.
     *
     * It calls okToContinue() to check if there are any unsaved changes, and writes the settings before the application terminates.
     *
     * \param ev the close event (has to be accepted to close or ignored to keep it open)
     */
    void closeEvent(QCloseEvent* ev);

    /*!
     * \brief This method is called when a drag operation is in progress and the mouse enters the window.
     *
     * This is used to open a file by dragging the file icon into the main window.
     * The event has to be accepted if it contains a file URL.
     *
     * \param ev the drag event
     */
    void dragEnterEvent(QDragEnterEvent* ev);

    /*!
     * \brief This method is called when a drag operation is finally dropped on the window.
     *
     * This is used to open a file by dragging the file icon into the main window.
     * It extracts the file URL from the event data and calls openNamedFile() to load the file.
     *
     * \param ev the drop event
     */
    void dropEvent(QDropEvent* ev);

private slots:
    /*!
     * \brief Target slot for the Open File action.
     *
     * Checks with okToContinue() that there are no unsaved changes,
     * shows a file dialog and loads the file with openNamedFile().
     */
    void openFile();

    /*!
     * \brief Target slot for the Save File action.
     *
     * Saves the file with saveNamedFile().
     *
     * \return true if the file was saved successfully, otherwise false.
     */
    bool saveFile();

    /*!
     * \brief Target slot for the Save As action.
     *
     * Shows a file dialog and saves the file with saveNamedFile().
     *
     * \return true if the file was saved successfully, otherwise false.
     */
    bool saveAs();

    /*!
     * \brief Target slot for the Close File action.
     *
     * Checks with okToContinue() that there are no unsaved changes
     * and closes the window if this is the case.
     */
    void closeFile();

    /*!
     * \brief Target slot for the Save As Picture action.
     *
     * Shows a file dialog and saves the visible area of the track layout.
     */
    void saveAsPicture();

    /*!
     * \brief Target slot for the Save As PDF action.
     *
     * Shows a file dialog and saves the visible area of the track layout.
     */
    void saveAsPdf();

    /*!
     * \brief Target slot for the Print action.
     *
     * Shows a print dialog and prints the visible area of the track layout.
     */
    void printFile();

    /*!
     * \brief Target slot for the scale and rotate spinboxes.
     *
     * Creates a transform matrix with the values and applies it to the graphics view.
     */
    void transformGraphicsView();

    /*!
     * \brief Target slot for the Zoom In action.
     *
     * Also updates the scale spinbox.
     */
    void zoomIn();

    /*!
     * \brief Target slot for the Zoom Out action.
     *
     * Also updates the scale spinbox.
     */
    void zoomOut();

    /*!
     * \brief Target slot for the Rotate Left action.
     *
     * Also updates the rotate spinbox.
     */
    void rotateLeft();

    /*!
     * \brief Target slot for the Rotate Right action.
     *
     * Also updates the rotate spinbox.
     */
    void rotateRight();

    /*!
     * \brief Target slot for the Rotate Right action.
     *
     * Resets scale and rotate values to the default.
     */
    void resetView();

    /*!
     * \brief Target slot for the Help Contents action.
     *
     * \todo The online help is not implemented yet.
     */
    void showHelp();

    /*!
     * \brief Target slot for the Show Document Info action.
     *
     * Shows a window with informations about the loaded file and the contained objects.
     */
    void showDocumentInfo();

    /*!
     * \brief Target slot for the Edit Remark action.
     *
     * Shows a text input dialog where a descriptive remark can be entered.
     * The remark will be stored in the header of the PlanPro file.
     */
    void editRemark();

    /*!
     * \brief Target slot for the About action.
     *
     * Shows version and copyright information.
     */
    void about();

    /*!
     * \brief Target slot for the Go To Object action.
     *
     * Shows a dialog where a UUID can be entered.
     * If such an object exists, it will be selected in all windows where it is visible.
     */
    void goToObject();

    /*!
     * \brief Target slot for the Search action.
     *
     * Shows a search dialog where a pattern and several options can be entered.
     * All objects that match the pattern will be shown in the search results window.
     */
    void handleObjectSearch();

    /*!
     * \brief Target slot for the Center Object action.
     *
     * Centers the selected object in the graphics view if it has been scrolled out of the visible area.
     */
    void centerObject();

    /*!
     * \brief Target slot for the view mode actions.
     *
     * Updates the object list and graphics view depending on the selected mode.
     */
    void switchViewMode();

    /*!
     * \brief Target slot for the category combo box.
     *
     * Updates the object list so that it shows only the selected category.
     */
    void changeCategory();

    /*!
     * \brief Target slot for the Extract File action.
     *
     * If the selected object contains binary Base64-encoded data, a file dialog is shown where the user can
     * change the storage location, and the file is written to disk.
     * If the object does not contain any data, an error message is shown.
     */
    void extractFile();

    /*!
     * \brief Target slot for the Measure Distance action.
     *
     * If two Objects of a Punkt_Objekt subtype are selected, the distance between them is calculated,
     * based on the graph topology.
     * If only one object or no Punkt_Objekt type is selected, an error message is shown.
     */
    void measureDistance();

    /*!
     * \brief Target slot for the SMT Verification action.
     *
     * Shows the SmtVerifierDialog.
     */
    void verifyPlan();

    /*!
     * \brief Target slot for the Add To Bookmarks action.
     *
     * Adds the currently selected object(s) to the bookmark list.
     */
    void addToBookmarks();

    /*!
     * \brief Target slot for the Remove From Bookmarks action.
     *
     * Removes the objects that are currently selected in the bookmark list from this list.
     */
    void removeFromBookmarks();

    /*!
     * \brief Target slot for the Find Referencing Objects action.
     *
     * Calls createReferenceListRec() recursively for the selected object and shows the results
     * in the search results window.
     */
    void findReferencingObjects();

    /*!
     * \brief Target slot for the signal SmtVerifierDialog::counterexamplesFound().
     *
     * Retrieves the counterexamples with SmtVerifierDialog::getCounterexampleList()
     * and shows them in the search results window.
     */
    void showSmtCounterexamples();

    /*!
     * \brief Target slot for the %Preferences action.
     *
     * Shows the PreferencesDialog.
     */
    void showPreferences();

    /*!
     * \brief Target slot for the Show Readme action.
     *
     * Shows a TextFileDialog displaying the Readme file.
     */
    void showReadme();

    /*!
     * \brief Target slot for the Show License action.
     *
     * Shows a TextFileDialog displaying the License file.
     */
    void showLicense();

    /*!
     * \brief Target slot for the Show Show Third Party Licenses action.
     *
     * Shows a TextFileDialog displaying the Third Party Licenses file.
     */
    void show3rdPartyLicenses();

private:
    /*!
     * \brief Create the actions and connect them to their slots.
     */
    void createActions();

    /*!
     * \brief Enable or disable actions, depending on if a valid file is loaded or not.
     */
    void enableActions();

    /*!
     * \brief Create the menus and add the actions to them.
     */
    void createMenus();

    /*!
     * \brief Create the toolbars and add the actions to them.
     */
    void createToolBars();

    /*!
     * \brief Create the dock windows.
     */
    void createDockWindows();

    /*!
     * \brief Check if an operation with potential data loss can be continued.
     *
     * This method is invoked before a new file is loaded or a file is closed and checks if there are unsaved changes.
     * If this is the case, a dialog box is shown asking the user if the changes should be saved.
     *
     * The method returns true if
     * - there are no unsaved changes
     * - there are unsaved changes and the user clicks Yes to save them
     * - there are unsaved changes and the user clicks No to confirm that the changes should be discarded
     *
     * The method returns false if the user clicks Cancel. In this case the current operation has to be aborted.
     *
     * \return true if the current operation can be continued, false if it must be aborted
     */
    bool okToContinue();

    /*!
     * \brief Open a PlanPro file.
     *
     * \param filename the file name
     */
    void openNamedFile(const QString& filename);

    /*!
     * \brief Save a PlanPro file.
     *
     * \param filename the file name
     * \return true if the file could be saved successfully, otherwise false
     */
    bool saveNamedFile(const QString& filename);

    /*!
     * \brief Reads the window state from the Preferences object.
     */
    void readSettings();

    /*!
     * \brief Updates the Preferences object with the current window state and writes the settings to disk.
     */
    void writeSettings();

    /*!
     * \brief Create the entries for the list of dependent objects recursively.
     *
     * This method searches for items that have an UUID reference to the current item.
     * All these found items are added to the list as child items below the current item.
     * The search is continued recursively for each found object until no further references are found
     * or the maximum recursion depth (as specified in the Preferences) is reached.
     *
     * \param item the current DomItem
     * \param parent the tree widget item representing the current dom item
     * \param depth the current recursion depth
     */
    void createReferenceListRec(DomItem* item, QTreeWidgetItem* parent, int depth);

    QMenu* fileMenu; /*!< \brief The File menu. */
    QMenu* editMenu; /*!< \brief The Edit menu. */
    QMenu* viewMenu; /*!< \brief The View menu. */
    QMenu* viewDockSubmenu; /*!< \brief The View Dock Windows submenu. */
    QMenu* viewToolbarSubmenu; /*!< \brief The View Toolbars submenu. */
    QMenu* objectMenu; /*!< \brief The Object menu. */
    QMenu* toolsMenu; /*!< \brief The Tools menu. */
    QMenu* bookmarkMenu; /*!< \brief The Bookmark menu. */
    QMenu* helpMenu; /*!< \brief The Help menu. */
    QToolBar* fileToolBar; /*!< \brief The File toolbar. */
    QToolBar* editToolBar; /*!< \brief The Edit toolbar. */
    QToolBar* viewToolBar; /*!< \brief The View toolbar. */
    QToolBar* objectToolBar; /*!< \brief The Object toolbar. */
    QToolBar* toolsToolBar; /*!< \brief The Tools toolbar. */
    QToolBar* bookmarkToolBar; /*!< \brief The Bookmar toolbar. */
    QAction* openFileAct; /*!< \brief The Open File action. */
    QAction* saveFileAct; /*!< \brief The Save File action. */
    QAction* saveAsAct; /*!< \brief The Save As action. */
    QAction* closeFileAct; /*!< \brief The Close File action. */
    QAction* saveAsPictureAct; /*!< \brief The Save As Picture action. */
    QAction* saveAsPdfAct; /*!< \brief The Save As PDF action. */
    QAction* printFileAct; /*!< \brief The Print action. */
    QAction* docInfoAct; /*!< \brief The Document Info action. */
    QAction* exitAct; /*!< \brief The Exit action. */
    QAction* searchAct; /*!< \brief The Search action. */
    QAction* editRemarkAct; /*!< \brief The Edit Remark action. */
    QAction* centerAct; /*!< \brief The Center Object action. */
    QAction* zoomInAct; /*!< \brief The Zoom In action. */
    QAction* zoomOutAct; /*!< \brief The Zoom Out action. */
    QAction* rotateLeftAct; /*!< \brief The Rotate Left action. */
    QAction* rotateRightAct; /*!< \brief The Rotate Right action. */
    QAction* resetViewAct; /*!< \brief The Reset View action. */
    QActionGroup* viewModeActGroup; /*!< \brief Action group for the view mode actions (to ensure that only one is active). */
    QAction* startViewModeAct; /*!< \brief The Show Start View Mode action. */
    QAction* endViewModeAct; /*!< \brief The Show End View Mode action. */
    QAction* comparisonViewModeAct; /*!< \brief The Show Start/End Comparison View Mode action. */
    QAction* selectAllFiltersAct; /*!< \brief The Select All Filter Settings action. */
    QAction* deselectAllFiltersAct; /*!< \brief The Deselect All Filter Settings action. */
    QAction* extractFileAct; /*!< \brief The Extract File action. */
    QAction* goToObjectAct; /*!< \brief The Go To Object action. */
    QAction* measureDistanceAct; /*!< \brief The Measure Distance action. */
    QAction* findReferencingObjectsAct; /*!< \brief The Find Referencing Objects action. */
    QAction* smtVerificationAct; /*!< \brief The SMT Verification action. */
    QAction* addBookmarkAct; /*!< \brief The Add To Bookmarks action. */
    QAction* removeBookmarkAct; /*!< \brief The Remove From Bookmarks. */
    QAction* clearBookmarkListAct; /*!< \brief The Clear Bookmarks action. */
    QAction* preferencesAct; /*!< \brief The %Preferences action. */
    QAction* helpContentsAct; /*!< \brief The Help Contents action. */
    QAction* showReadmeAct; /*!< \brief The Show Readme File action. */
    QAction* showLicenseAct; /*!< \brief The Show License File action. */
    QAction* show3rdPartyLicensesAct; /*!< \brief The Show Third Party Licenses action. */
    QAction* aboutAct; /*!< \brief The About action. */
    QSpinBox* scaleSpinBox; /*!< \brief The scale spin box in the toolbar. */
    QSpinBox* rotateSpinBox; /*!< \brief The rotate spin box in the toolbar. */
    
    DocumentTreeModel* doctreemodel; /*!< \brief The document tree model providing the content for the document tree view. */
    ObjectListModel* objectlistmodel; /*!< \brief The object list model providing the content for the object list view. */
    GraphicsScene* scene; /*!< \brief The graphics scene providing the content for the graphics view. */
    QGraphicsView* view; /*!< \brief The graphics view showing the track layout. */
    SelectionManager* selectionManager; /*!< \brief The selection manager for synchronizing selections between different views. */
    ObjectInfoWidget* objectInfo; /*!< \brief The object properties widget. */
    FilterWidget* filterWidget; /*!< \brief The filter settings widget. */
    QTreeView* documentTreeView; /*!< \brief The document tree view. */
    QTreeView* objectListView; /*!< \brief The object list view. */
    QListWidget* bookmarkList; /*!< \brief The bookmark list. */
    QTreeWidget* searchResultList; /*!< \brief The search result list. */
    QComboBox* categoryComboBox; /*!< \brief The category combo box for the object list view. */

    SmtVerifierDialog* smtVerifierDialog; /*!< \brief The modeless SMT verifier dialog. */

    PlanProXmlDocument* document; /*!< \brief The currently loaded PlanPro document. */
};

#endif // MAINWINDOW_H
