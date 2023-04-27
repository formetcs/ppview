#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "objectlistmodel.h"

#include <QMainWindow>
#include <QItemSelection>

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
class PlanProModel;
class GraphicsScene;
class PlanProXmlDocument;
class GraphicsSceneBuilder;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString& dataFileName, QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* ev);
    void dragEnterEvent(QDragEnterEvent* ev);
    void dropEvent(QDropEvent* ev);

private slots:
    void openFile();
    void saveFile();
    void exportToPicture();
    void exportToPdf();
    void printFile();
    void transformGraphicsView(int);
    void showHelp();
    void showDocumentInfo();
    void about();
    void handleObjectSearchFromSearchWindow();
    void handleObjectSearchFromMenu();
    void centerObject();
    void switchPlanningState();
    void changeCategory();
    void extractFile();
    void measureDistance();
    void addToFavorites();
    void removeFromFavorites();
    void findReferencingObjects();
    void setLanguage();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createDockWindows();
    void openNamedFile(const QString& filename);
    void readSettings();
    void writeSettings();

    QMenu* fileMenu;
    QMenu* exportSubmenu;
    QMenu* editMenu;
    QMenu* viewMenu;
    QMenu* viewDockSubmenu;
    QMenu* objectMenu;
    QMenu* favoriteMenu;
    QMenu* extrasMenu;
    QMenu* helpMenu;
    QToolBar* toolBar;
    QAction* openFileAct;
    QAction* saveFileAct;
    QAction* exportToPictureAct;
    QAction* exportToPdfAct;
    QAction* printFileAct;
    QAction* exitAct;
    QAction* searchAct;
    QAction* centerAct;
    QActionGroup* planningStateActGroup;
    QAction* startStateAct;
    QAction* endStateAct;
    QAction* combinedStateAct;
    QAction* selectAllFiltersAct;
    QAction* deselectAllFiltersAct;
    QAction* extractFileAct;
    QAction* measureDistanceAct;
    QAction* findReferencingObjectsAct;
    QAction* addFavoriteAct;
    QAction* removeFavoriteAct;
    QAction* clearFavoriteListAct;
    QAction* setLanguageAct;
    QAction* helpContentsAct;
    QAction* aboutAct;
    QSpinBox* scaleSpinBox;
    QSpinBox* rotateSpinBox;

    PlanProModel* model;
    ObjectListModel* objectlistmodel;
    GraphicsScene* scene;
    QGraphicsView* view;
    SelectionManager* selectionManager;
    ObjectInfoWidget* objectInfo;
    FilterWidget* filterWidget;
    QTreeView* objectTreeView;
    QTreeView* objectListView;
    QListWidget* favoriteList;
    QListWidget* referenceList;
    QLineEdit* searchEdit;
    QPushButton* searchButton;
    QComboBox* categoryComboBox;

    PlanProXmlDocument* document;
};

#endif // MAINWINDOW_H
