#ifndef SELECTIONMANAGER_H
#define SELECTIONMANAGER_H

#include "domitem.h"

#include <QObject>

class QTreeView;
class QListWidget;
class QItemSelection;
class GraphicsScene;
class PlanProDocument;
class SelectionManager : public QObject
{
    Q_OBJECT
public:

    enum SelectionSource
    {
        SelectionSourceNotSelected = 0,
        SelectionSourceExternal = 1,
        SelectionSourceGraphicsView = 2,
        SelectionSourceObjectList = 3,
        SelectionSourceFavoriteList = 4,
        SelectionSourceReferenceList = 5
    };

    explicit SelectionManager(QObject* parent = nullptr);
    void setDocument(PlanProDocument* doc);
    void setScene(GraphicsScene* scene);
    void setFavoriteListWidget(QListWidget* favList);
    void setReferenceListWidget(QListWidget* refList);
    void setDocumentTreeView(QTreeView* docTreeView);
    void setObjectListView(QTreeView* objListView);
    QList<DomItem*> getSelectedItems();


public slots:
    void selectItems(const QStringList& ids);
    void selectItem(const QString& id);

signals:
    void selectionChanged(QList<DomItem*> objectlist);


private slots:
    void handleGraphicsSceneSelection();
    void handleFavoriteListSelection();
    void handleReferenceListSelection();
    void handleDocumentTreeSelection(const QItemSelection& selected, const QItemSelection& deselected);
    void handleObjectListSelection(const QItemSelection& selected, const QItemSelection& deselected);


private:
    QList<DomItem*> selectedItemList;
    PlanProDocument* document;
    GraphicsScene* graphicsScene;
    QListWidget* favoriteListWidget;
    QListWidget* referenceListWidget;
    QTreeView* documentTreeView;
    QTreeView* objectListView;


    SelectionSource selectionSource;

};

#endif // SELECTIONMANAGER_H
