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

#ifndef PUNKTOBJEKT_H
#define PUNKTOBJEKT_H

#include "basisobjekt.h"

#include <QString>

class PunktObjektStrecke
{
public:
    PunktObjektStrecke(DomItem* item = nullptr);

    virtual QString getIdStrecke() const;
    virtual QString getStreckeKm() const;

protected:
    DomItem* domItem;
};

class PunktObjektTopKante
{
public:
    PunktObjektTopKante(DomItem* item = nullptr);

    virtual QString getIdTopKante() const;
    virtual int getAbstand() const;
    virtual QString getWirkrichtung() const;
    virtual QString getSeitlicheLage() const;
    virtual int getSeitlicherAbstand() const;

protected:
    DomItem* domItem;
};

class PunktObjekt : public BasisObjekt
{
public:
    PunktObjekt(DomItem* item = nullptr);
    virtual bool isPunktObjekt() const;

    virtual int getPunktObjektStreckeCount() const;
    virtual PunktObjektStrecke getPunktObjektStrecke(int i) const;
    virtual QString getIdStrecke(int i) const;
    virtual QString getStreckeKm(int i) const;
    virtual int getPunktObjektTopKanteCount() const;
    virtual PunktObjektTopKante getPunktObjektTopKante(int i) const;
    virtual QString getIdTopKante(int i) const;
    virtual int getAbstand(int i) const;
    virtual QString getWirkrichtung(int i) const;
    virtual QString getSeitlicheLage(int i) const;
    virtual int getSeitlicherAbstand(int i) const;

protected:

};

#endif // PUNKTOBJEKT_H
