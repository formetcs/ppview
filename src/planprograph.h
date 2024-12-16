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
