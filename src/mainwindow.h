#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QItemSelection>

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

enum SelectionSource
{
    SelectionSourceNotSelected = 0,
    SelectionSourceExternal = 1,
    SelectionSourceGraphicsView = 2,
    SelectionSourceObjectList = 3,
    SelectionSourceFavoriteList = 4,
    SelectionSourceReferenceList = 5
};


class Parser;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString& dataFileName, QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* ev);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private slots:
    void openFile();
    void addFile();
    void saveFile();
    void exportToPicture();
    void exportToPdf();
    void printFile();
    void transformGraphicsView(int);
    void showHelp();
    void about();
    void setTelegramInfo(const QString& text);
    void handleObjectSearchFromSearchWindow();
    void handleObjectSearchFromMenu();
    void handleObjectSearch(QString id);
    void centerObject();
    void extractFile();
    void measureDistance();
    void handleGraphicsSceneSelection();
    void handleFavoriteListSelection();
    void handleReferenceListSelection();
    void handleObjectListSelection(const QItemSelection &selected, const QItemSelection &deselected);
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
    QAction* addFileAct;
    QAction* saveFileAct;
    QAction* exportToPictureAct;
    QAction* exportToPdfAct;
    QAction* printFileAct;
    QAction* exitAct;
    QAction* searchAct;
    QAction* centerAct;
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
    GraphicsScene* scene;
    QGraphicsView* view;
    QTextEdit* objectInfo;
    FilterWidget* filterWidget;
    QTreeView* objectTreeView;
    QListWidget* favoriteList;
    QListWidget* referenceList;
    QLineEdit* searchEdit;
    QPushButton* searchButton;

    QString fileName;
    QString language;

    SelectionSource selectionSource;

};

#endif // MAINWINDOW_H
