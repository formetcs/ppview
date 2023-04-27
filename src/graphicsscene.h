#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QObject>
#include <QList>
#include <QGraphicsScene>

#include "filterwidget.h"
#include "planprodocument.h"

//class QGraphicsItem;


const int GRAPHICSITEM_TYPE = 1;
const int GRAPHICSITEM_ID = 2;
const int GRAPHICSITEM_STATE = 3;


class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphicsScene(QObject* parent = nullptr);
    ~GraphicsScene();

    QGraphicsItem* getItemById(const QString& id, PlanProDocument::PlanningState state);

signals:

public slots:
    void changeFilterSettings(const QString& key, bool state);
    void changeFilterSettings(const QList<FilterState>& statelist);
    void unselectAllItems();
    void changePlanningState(PlanProDocument::PlanningState state);

private slots:

private:
    PlanProDocument::PlanningState planningState;
    QHash<QString,bool> filterSettings;

};

#endif // GRAPHICSSCENE_H
