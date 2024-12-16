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
