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

#ifndef TEMPORARYPUNKTOBJEKT_H
#define TEMPORARYPUNKTOBJEKT_H

#include "punktobjekt.h"

class TemporaryPunktObjekt : public PunktObjekt
{
public:
    TemporaryPunktObjekt(QString id, QString idtk, int abst, QString wirkr);
    bool isPunktObjekt() const;
    QString getIdentitaet() const;
    int getPunktObjektTopKanteCount() const;
    QString getIdTopKante(int i) const;
    int getAbstand(int i) const;
    QString getWirkrichtung(int i) const;
    QString getSeitlicheLage(int i) const;
    int getSeitlicherAbstand(int i) const;

private:
    QString identitaet;
    QString idTopKante;
    int abstand;
    QString wirkrichtung;
};

#endif // TEMPORARYPUNKTOBJEKT_H
