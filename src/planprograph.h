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
 * \file planprograph.h
 *
 * \brief Declaration of the PlanProGraph class and the NextTopKanteResult structure.
 */

#ifndef PLANPROGRAPH_H
#define PLANPROGRAPH_H

#include "planprodocument.h"
#include <QList>

class PunktObjekt;

class DomItem;


/*!
 * \brief Structure used as return value for PlanProGraph::getNextTopKante().
 *
 * It holds the DomItem of the resulting edge as well as the following search direction.
 * This is important if the topological direction of the edge changes, e.g. a node B of the first edge is also connected to a node B of the following edge.
 */
struct NextTopKanteResult
{
    DomItem* topKante; /*!< \brief DomItem representing the following TOP_Kante object. */
    bool direction; /*!< \brief The search direction of the returned edge, to continue searching in the same direction. */

    /*!
     * \brief Constructor.
     *
     * \param tk DomItem representing the following TOP_Kante object
     * \param dir the search direction of the returned edge, to continue searching in the same direction
     */
    NextTopKanteResult(DomItem* tk, bool dir)
    {
        topKante = tk;
        direction = dir;
    }
};

/*!
 * \brief Class to perform calculations on the PlanPro graph.
 */
class PlanProGraph
{
public:
    /*!
     * \brief Constructor.
     *
     * \param doc the PlanPro document
     */
    PlanProGraph(PlanProDocument* doc);

    /*!
     * \brief Get a list of TOP_Kante objects directly connected to a given TOP_Kante in a specified search direction.
     *
     * \param topKante DomItem representing the TOP_Kante whose neighbors should be searched
     * \param forward true if the search direction should be the same as the topological direction (A -> B) of the starting edge
     * \return a list of NextTopKanteResult objects, containing all connected edges
     */
    QList<NextTopKanteResult> getNextTopKante(DomItem* topKante, bool forward);

    /*!
     * \brief Calculates the distance between two Punkt_Objekt objects.
     *
     * The search is performed both in forward and reverse direction (related to startpos)
     * and also multiple Punkt_Objekt_TOP_Kante attribute groups in both startpos and endpos are considered.
     *
     * \param startpos the starting Punkt_Objekt
     * \param endpos the ending Punkt_Objekt
     * \return the (positive) distance between the objects in millimeters, -1 if endpos is not reachable, or -10 if one of the arguments is no Punkt_Objekt
     */
    int calculateDistance(DomItem* startpos, DomItem* endpos);

    /*!
     * \brief Calculates the distance between two Punkt_Objekt objects, using a specified search direction.
     *
     * If the forward value is true, the search follows the effective direction of the starting Punkt_Objekt, otherwise it searches backwards
     * (The effective direction "beide" will be handled like effective direction "in").
     *
     * \param startpos the starting Punkt_Objekt
     * \param endpos the ending Punkt_Objekt
     * \param forward true if the search direction should be the same as the effective direction of the starting Punkt_Objekt
     * \param state the planning state (allowed values are PlanningState::PlanningStateStart and PlanningState::PlanningStateEnd)
     * \return the (positive) distance between the objects in millimeters, or -1 if endpos is not reachable
     */
    int calculateDistance(PunktObjekt& startpos, PunktObjekt& endpos, bool forward, PlanProDocument::PlanningState state = PlanProDocument::PlanningStateEnd);

private:
    PlanProDocument* document; /*!< \brief The PlanPro document. */
};

#endif // PLANPROGRAPH_H
