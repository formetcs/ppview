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

#include "domitem.h"
#include "urobjekt.h"

UrObjekt::UrObjekt(DomItem* item)
{
    domItem = item;
}

bool UrObjekt::isUrObjekt() const
{
    if(!domItem)
    {
        return false;
    }
    return (domItem->getFirstItemAtPath("Identitaet/Wert") != NULL);
}

DomItem* UrObjekt::getDomItem()
{
    return domItem;
}

const DomItem* UrObjekt::getDomItem() const
{
    return domItem;
}

QString UrObjekt::getName() const
{
    if(!domItem)
    {
        return QString();
    }
    return domItem->getName();
}

QString UrObjekt::getIdentitaet() const
{
    return getIdentitaet(domItem);
}

QString UrObjekt::getIdentitaet(const DomItem *item)
{
    if(!item)
    {
        return QString();
    }
    return item->getFirstValueAtPath("Identitaet/Wert");
}
