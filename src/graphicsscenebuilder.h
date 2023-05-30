#ifndef GRAPHICSSCENEBUILDER_H
#define GRAPHICSSCENEBUILDER_H

#include <QLineF>
#include <QPointF>


class QPen;
class DomItem;
class PlanProDocument;
class GraphicsScene;
class QGraphicsItem;


class GraphicsSceneBuilder
{
public:
    GraphicsSceneBuilder(PlanProDocument* doc, GraphicsScene* scene);
    bool createGraphicsScene();

private:
    QPointF calculatePunktObjekt(const DomItem* o);
    QList<QLineF> calculateBereichObjekt(const DomItem* o);

    PlanProDocument* document;
    GraphicsScene* graphicsScene;
};

#endif // GRAPHICSSCENEBUILDER_H
