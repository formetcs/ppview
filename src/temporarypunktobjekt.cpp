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

#include "temporarypunktobjekt.h"

TemporaryPunktObjekt::TemporaryPunktObjekt(QString id, QString idtk, int abst, QString wirkr) : PunktObjekt(nullptr)
{
    identitaet = id;
    idTopKante = idtk;
    abstand = abst;
    wirkrichtung = wirkr;
}

bool TemporaryPunktObjekt::isPunktObjekt() const
{
    return true;
}

QString TemporaryPunktObjekt::getIdentitaet() const
{
    return identitaet;
}

int TemporaryPunktObjekt::getPunktObjektTopKanteCount() const
{
    return 1;
}

QString TemporaryPunktObjekt::getIdTopKante(int i) const
{
    if(i != 0)
    {
        return QString();
    }
    return idTopKante;
}

int TemporaryPunktObjekt::getAbstand(int i) const
{
    if(i != 0)
    {
        return 0;
    }
    return abstand;
}

QString TemporaryPunktObjekt::getWirkrichtung(int i) const
{
    if(i != 0)
    {
        return QString();
    }
    return wirkrichtung;
}

QString TemporaryPunktObjekt::getSeitlicheLage(int i) const
{
    Q_UNUSED(i);
    return QString();
}

int TemporaryPunktObjekt::getSeitlicherAbstand(int i) const
{
    Q_UNUSED(i);
    return 0;
}
