#ifndef GRAPHICSSCENEBUILDER_H
#define GRAPHICSSCENEBUILDER_H

class QPen;
class DomItem;
class PlanProDocument;
class GraphicsScene;
class QGraphicsItem;


class GraphicsSceneBuilder
{
public:
    GraphicsSceneBuilder(PlanProDocument* doc, GraphicsScene* scene);
    void createGraphicsScene();

private:
    QGraphicsItem* placePunktObjekt(DomItem* o);
    QGraphicsItem* placeBereichObjekt(DomItem* o, const QPen &pen);

    PlanProDocument* document;
    GraphicsScene* graphicsScene;
};

#endif // GRAPHICSSCENEBUILDER_H
