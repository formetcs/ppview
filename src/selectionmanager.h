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
