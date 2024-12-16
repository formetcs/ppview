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

#include "planprodocument.h"
#include "planprograph.h"
#include "punktobjekt.h"
#include "temporarypunktobjekt.h"

PlanProGraph::PlanProGraph(PlanProDocument* doc)
{
    document = doc;
}

QList<NextTopKanteResult> PlanProGraph::getNextTopKante(DomItem* topKante, bool forward)
{
    QList<NextTopKanteResult> returnval;
    if(!document || !topKante)
    {
        return returnval;
    }

    DomItem* lstZustandItem = topKante->parent()->parent();
    QString lstZustandItemName = lstZustandItem->getName();
    PlanProDocument::PlanningState currentItemState = PlanProDocument::PlanningStateEnd;
    if(lstZustandItemName == "LST_Zustand_Start")
    {
        currentItemState = PlanProDocument::PlanningStateStart;
    }

    QString srcIdTopKnotenA = topKante->getFirstValueAtPath("ID_TOP_Knoten_A/Wert");
    QString srcIdTopKnotenB = topKante->getFirstValueAtPath("ID_TOP_Knoten_B/Wert");
    QString srcTopAnschlussA = topKante->getFirstValueAtPath("TOP_Kante_Allg/TOP_Anschluss_A/Wert");
    QString srcTopAnschlussB = topKante->getFirstValueAtPath("TOP_Kante_Allg/TOP_Anschluss_B/Wert");

    QList<DomItem*> objectlist = document->getObjectList(currentItemState);
    for(int i = 0; i < objectlist.count(); ++i)
    {
        DomItem* item = objectlist.at(i);
        QString name = item->getName();
        if (name == "TOP_Kante" && item != topKante)
        {
            QString dstIdTopKnotenA = item->getFirstValueAtPath("ID_TOP_Knoten_A/Wert");
            QString dstIdTopKnotenB = item->getFirstValueAtPath("ID_TOP_Knoten_B/Wert");
            QString dstTopAnschlussA = item->getFirstValueAtPath("TOP_Kante_Allg/TOP_Anschluss_A/Wert");
            QString dstTopAnschlussB = item->getFirstValueAtPath("TOP_Kante_Allg/TOP_Anschluss_B/Wert");

            if (forward && (srcIdTopKnotenB == dstIdTopKnotenA) && srcTopAnschlussB == "Verbindung"
                    && dstTopAnschlussA == "Verbindung")
            {
                NextTopKanteResult res(item, true);
                returnval.append(res);
            }
            else if (forward && (srcIdTopKnotenB == dstIdTopKnotenA) && srcTopAnschlussB == "Links"
                       && dstTopAnschlussA == "Spitze")
            {
                NextTopKanteResult res(item, true);
                returnval.append(res);
            }
            else if (forward && (srcIdTopKnotenB == dstIdTopKnotenA) && srcTopAnschlussB == "Rechts"
                       && dstTopAnschlussA == "Spitze")
            {
                NextTopKanteResult res(item, true);
                returnval.append(res);
            }
            else if (forward && (srcIdTopKnotenB == dstIdTopKnotenA) && srcTopAnschlussB == "Spitze"
                       && dstTopAnschlussA == "Links")
            {
                NextTopKanteResult res(item, true);
                returnval.append(res);
            }
            else if (forward && (srcIdTopKnotenB == dstIdTopKnotenA) && srcTopAnschlussB == "Spitze"
                       && dstTopAnschlussA == "Rechts")
            {
                NextTopKanteResult res(item, true);
                returnval.append(res);
            }

            else if (forward && (srcIdTopKnotenB == dstIdTopKnotenB) && srcTopAnschlussB == "Verbindung"
                     && dstTopAnschlussB == "Verbindung")
            {
                NextTopKanteResult res(item, false);
                returnval.append(res);
            }
            else if (forward && (srcIdTopKnotenB == dstIdTopKnotenB) && srcTopAnschlussB == "Links"
                       && dstTopAnschlussB == "Spitze")
            {
                NextTopKanteResult res(item, false);
                returnval.append(res);
            }
            else if (forward && (srcIdTopKnotenB == dstIdTopKnotenB) && srcTopAnschlussB == "Rechts"
                       && dstTopAnschlussB == "Spitze")
            {
                NextTopKanteResult res(item, false);
                returnval.append(res);
            }
            else if (forward && (srcIdTopKnotenB == dstIdTopKnotenB) && srcTopAnschlussB == "Spitze"
                       && dstTopAnschlussB == "Links")
            {
                NextTopKanteResult res(item, false);
                returnval.append(res);
            }
            else if (forward && (srcIdTopKnotenB == dstIdTopKnotenB) && srcTopAnschlussB == "Spitze"
                       && dstTopAnschlussB == "Rechts")
            {
                NextTopKanteResult res(item, false);
                returnval.append(res);
            }

            else if (!forward && (srcIdTopKnotenA == dstIdTopKnotenB) && srcTopAnschlussA == "Verbindung"
                     && dstTopAnschlussB == "Verbindung")
            {
                NextTopKanteResult res(item, false);
                returnval.append(res);
            }
            else if (!forward && (srcIdTopKnotenA == dstIdTopKnotenB) && srcTopAnschlussA == "Links"
                       && dstTopAnschlussB == "Spitze")
            {
                NextTopKanteResult res(item, false);
                returnval.append(res);
            }
            else if (!forward && (srcIdTopKnotenA == dstIdTopKnotenB) && srcTopAnschlussA == "Rechts"
                       && dstTopAnschlussB == "Spitze")
            {
                NextTopKanteResult res(item, false);
                returnval.append(res);
            }
            else if (!forward && (srcIdTopKnotenA == dstIdTopKnotenB) && srcTopAnschlussA == "Spitze"
                       && dstTopAnschlussB == "Links")
            {
                NextTopKanteResult res(item, false);
                returnval.append(res);
            }
            else if (!forward && (srcIdTopKnotenA == dstIdTopKnotenB) && srcTopAnschlussA == "Spitze"
                       && dstTopAnschlussB == "Rechts")
            {
                NextTopKanteResult res(item, false);
                returnval.append(res);
            }

            else if (!forward && (srcIdTopKnotenA == dstIdTopKnotenA) && srcTopAnschlussA == "Verbindung"
                     && dstTopAnschlussA == "Verbindung")
            {
                NextTopKanteResult res(item, true);
                returnval.append(res);
            }
            else if (!forward && (srcIdTopKnotenA == dstIdTopKnotenA) && srcTopAnschlussA == "Links"
                       && dstTopAnschlussA == "Spitze")
            {
                NextTopKanteResult res(item, true);
                returnval.append(res);
            }
            else if (!forward && (srcIdTopKnotenA == dstIdTopKnotenA) && srcTopAnschlussA == "Rechts"
                       && dstTopAnschlussA == "Spitze")
            {
                NextTopKanteResult res(item, true);
                returnval.append(res);
            }
            else if (!forward && (srcIdTopKnotenA == dstIdTopKnotenA) && srcTopAnschlussA == "Spitze"
                       && dstTopAnschlussA == "Links")
            {
                NextTopKanteResult res(item, true);
                returnval.append(res);
            }
            else if (!forward && (srcIdTopKnotenA == dstIdTopKnotenA) && srcTopAnschlussA == "Spitze"
                       && dstTopAnschlussA == "Rechts")
            {
                NextTopKanteResult res(item, true);
                returnval.append(res);
            }
        }
    }
    return returnval;
}

int PlanProGraph::calculateDistance(DomItem* startpos, DomItem* endpos)
{
    if(!document || !startpos || !endpos)
    {
        return -10;
    }

    DomItem* lstZustandItem = startpos->parent()->parent();
    QString lstZustandItemName = lstZustandItem->getName();
    PlanProDocument::PlanningState currentItemState = PlanProDocument::PlanningStateEnd;
    if(lstZustandItemName == "LST_Zustand_Start")
    {
        currentItemState = PlanProDocument::PlanningStateStart;
    }

    PunktObjekt poStart(startpos);
    PunktObjekt poEnd(endpos);
    if(!poStart.isPunktObjekt() || !poEnd.isPunktObjekt())
    {
        return -10;
    }

    QString identitaet1 = poStart.getIdentitaet();
    for(int i = 0; i < poStart.getPunktObjektTopKanteCount(); ++i)
    {
        QString idTopKante1 = poStart.getIdTopKante(i);
        int abstand1 = poStart.getAbstand(i);
        QString wirkrichtung1 = poStart.getWirkrichtung(i);

        QString identitaet2 = poEnd.getIdentitaet();
        for(int j = 0; j < poEnd.getPunktObjektTopKanteCount(); ++j)
        {
            QString idTopKante2 = poEnd.getIdTopKante(j);
            int abstand2 = poEnd.getAbstand(j);
            QString wirkrichtung2 = poEnd.getWirkrichtung(j);

            TemporaryPunktObjekt tpo1(identitaet1, idTopKante1, abstand1, wirkrichtung1);
            TemporaryPunktObjekt tpo2(identitaet2, idTopKante2, abstand2, wirkrichtung2);

            int result = calculateDistance(tpo1, tpo2, true, currentItemState);
            if(result >= 0)
            {
                return result;
            }
            result = calculateDistance(tpo1, tpo2, false, currentItemState);
            if(result >= 0)
            {
                return result;
            }
        }
    }
    return -1;
}

int PlanProGraph::calculateDistance(PunktObjekt& startpos, PunktObjekt& endpos, bool forward, PlanProDocument::PlanningState state)
{
    int returnval = -1;
    QString startGuid = startpos.getIdTopKante(0);
    int startAbstand = startpos.getAbstand(0);
    QString startWirkrichtung = startpos.getWirkrichtung(0);
    QString endGuid = endpos.getIdTopKante(0);
    int endAbstand = endpos.getAbstand(0);
    //QString endWirkrichtung = endpos.getWirkrichtung(0);
    if (startGuid == endGuid && startWirkrichtung != "gegen" && forward) // both positions are on the same edge
    {
        returnval = endAbstand - startAbstand;
    }
    else if (startGuid == endGuid && startWirkrichtung != "gegen" && !forward) // both positions are on the same edge
    {
        returnval = startAbstand - endAbstand;
    }
    else if (startGuid == endGuid && startWirkrichtung == "gegen" && !forward) // both positions are on the same edge
    {
        returnval = endAbstand - startAbstand;
    }
    else if (startGuid == endGuid && startWirkrichtung == "gegen" && forward) // both positions are on the same edge
    {
        returnval = startAbstand - endAbstand;
    }
    else
    {
        DomItem* startEdge = document->getObjectById(startGuid, state);
        if (startEdge)
        {
            int currentDistance = -1;
            int remainingDistance = -1;
            bool direction = true;
            if (startWirkrichtung != "gegen" && forward)
            {
                double tempEdgeLength = startEdge->getFirstValueAtPath("TOP_Kante_Allg/TOP_Laenge/Wert").toDouble();
                int edgeLength = (int) (tempEdgeLength * 1000.0 + 0.5);
                currentDistance = edgeLength - startAbstand;
                direction = true;
            }
            else if (startWirkrichtung != "gegen" && !forward)
            {
                currentDistance = startAbstand;
                direction = false;
            }
            else if (startWirkrichtung == "gegen" && forward)
            {
                currentDistance = startAbstand;
                direction = false;
            }
            else if (startWirkrichtung == "gegen" && !forward)
            {
                double tempEdgeLength = startEdge->getFirstValueAtPath("TOP_Kante_Allg/TOP_Laenge/Wert").toDouble();
                int edgeLength = (int) (tempEdgeLength * 1000.0 + 0.5);
                currentDistance = edgeLength - startAbstand;
                direction = true;
            }
            QList<NextTopKanteResult> edgelist = getNextTopKante(startEdge, direction);
            for (int i = 0; i < edgelist.size(); ++i)
            {
                NextTopKanteResult edgeresult = edgelist.at(i);
                DomItem* tka = edgeresult.topKante;
                QString tka_id = tka->getFirstValueAtPath("Identitaet/Wert");
                bool newdir = edgeresult.direction;
                int newAbstand = 0;
                if (newdir)
                {
                    newAbstand = 0;
                }
                else
                {
                    double tka_len = tka->getFirstValueAtPath("TOP_Kante_Allg/TOP_Laenge/Wert").toDouble();
                    newAbstand = (int) (tka_len * 1000.0 + 0.5);
                }
                TemporaryPunktObjekt newStart(QString(), tka_id, newAbstand, "in");
                int tempdist = calculateDistance(newStart, endpos, newdir, state);
                if ((remainingDistance < 0 && tempdist >= 0)
                        || (remainingDistance >= 0 && tempdist >= 0 && tempdist < remainingDistance))
                {
                    remainingDistance = tempdist;
                }
            }
            if (remainingDistance >= 0) {
                returnval = currentDistance + remainingDistance;
            }
        }
    }
    return returnval;
}
