#ifndef PLANPROGRAPH_H
#define PLANPROGRAPH_H

#include "planprodocument.h"
#include <QList>

class PunktObjekt;

class DomItem;

struct NextTopKanteResult
{
    DomItem* topKante;
    bool direction;
    NextTopKanteResult(DomItem* tk, bool dir)
    {
        topKante = tk;
        direction = dir;
    }
};

class PlanProGraph
{
public:
    PlanProGraph(PlanProDocument* doc);
    QList<NextTopKanteResult> getNextTopKante(DomItem* topKante, bool forward);
    int calculateDistance(DomItem* startpos, DomItem* endpos);
    int calculateDistance(PunktObjekt& startpos, PunktObjekt& endpos, bool forward, PlanProDocument::PlanningState state = PlanProDocument::PlanningStateEnd);

private:
    PlanProDocument* document;
};

#endif // PLANPROGRAPH_H
