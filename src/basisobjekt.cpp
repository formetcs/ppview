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
 * \file basisobjekt.cpp
 *
 * \brief Implementation of the BasisObjekt class.
 */

#include "basisobjekt.h"
#include "domitem.h"

BasisObjekt::BasisObjekt(DomItem* item) : UrObjekt(item)
{
}

bool BasisObjekt::isBasisObjekt() const
{
    if(!domItem)
    {
        return false;
    }
    return (isUrObjekt() && (domItem->getFirstItemAtPath("Basis_Objekt_Allg") != NULL));
}

int BasisObjekt::getIdBearbeitungsvermerkCount() const
{
    if(!domItem)
    {
        return 0;
    }
    return domItem->getChildItems("ID_Bearbeitungsvermerk").count();
}

QString BasisObjekt::getIdBearbeitungsvermerk(int i) const
{
    if(!domItem || i < 0 || i >= getIdBearbeitungsvermerkCount())
    {
        return QString();
    }
    QList<DomItem*> childlist = domItem->getChildItems("ID_Bearbeitungsvermerk");
    return childlist.at(i)->getFirstValueAtPath("Wert");
}

QString BasisObjekt::getDbGdiReferenz() const
{
    if(!domItem)
    {
        return QString();
    }
    return domItem->getFirstValueAtPath("Objektreferenzen/DB_GDI_Referenz/Wert");
}

QString BasisObjekt::getTechnischerPlatz() const
{
    if(!domItem)
    {
        return QString();
    }
    return domItem->getFirstValueAtPath("Objektreferenzen/Technischer_Platz/Wert");
}

QString BasisObjekt::getObjektzustandBesonders() const
{
    if(!domItem)
    {
        return QString();
    }
    return domItem->getFirstValueAtPath("Basis_Objekt_Allg/Objektzustand_Besonders/Wert");
}

bool BasisObjekt::getBestandsschutz() const
{
    if(!domItem)
    {
        return false;
    }
    return (domItem->getFirstValueAtPath("Basis_Objekt_Allg/Bestandsschutz/Wert") == "true");
}

QString BasisObjekt::getIdAnhangRegelwerkBesonders() const
{
    if(!domItem)
    {
        return QString();
    }
    return domItem->getFirstValueAtPath("Basis_Objekt_Allg/ID_Anhang_Regelwerk_Besonders/Wert");
}

QString BasisObjekt::getDatumRegelwerk() const
{
    if(!domItem)
    {
        return QString();
    }
    return domItem->getFirstValueAtPath("Basis_Objekt_Allg/Datum_Regelwerk/Wert");
}
