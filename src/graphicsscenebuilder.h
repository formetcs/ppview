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

#ifndef GRAPHICSSCENEBUILDER_H
#define GRAPHICSSCENEBUILDER_H

#include <QLineF>
#include <QPointF>


class QPen;
class DomItem;
class PlanProDocument;
class GraphicsScene;
class QGraphicsItem;


/*!
 * \brief Class to build a GraphicsScene from a PlanProDocument.
 */
class GraphicsSceneBuilder
{
public:
    /*!
     * \brief Creates a new GraphicsSceneBuilder.
     *
     * This sets up the builder object only. Call createGraphicsScene()
     * afterwards to actually create the scene.
     *
     * \param doc the PlanProDocument
     * \param scene the graphics scene
     */
    GraphicsSceneBuilder(PlanProDocument* doc, GraphicsScene* scene);

    /*!
     * \brief Create the graphics scene.
     *
     * This has to be called whenever the coordinates of an object have changed.
     *
     * \return true if the scene was created successfully, otherwise false
     */
    bool createGraphicsScene();

private:
    /*!
     * \brief Calculate the coordinates where a Punkt_Objekt subtype has to be drawn.
     *
     * \param o the DomItem pointing to the root of a Punkt_Objekt subtype
     * \return the center point where the item has to be drawn
     */
    QPointF calculatePunktObjekt(const DomItem* o);

    /*!
     * \brief Calculate the coordinates where a Bereich_Objekt subtype has to be drawn.
     *
     * \param o the DomItem pointing to the root of a Bereich_Objekt subtype
     * \return a list of QLineF objects containing the coordinates of the line segments which have to be drawn
     */
    QList<QLineF> calculateBereichObjekt(const DomItem* o);

    PlanProDocument* document; /*!< \brief The PlanPro document. */
    GraphicsScene* graphicsScene; /*!< \brief The graphics scene. */
};

#endif // GRAPHICSSCENEBUILDER_H
