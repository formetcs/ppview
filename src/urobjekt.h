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

#ifndef UROBJEKT_H
#define UROBJEKT_H

#include <QString>

class DomItem;


/*!
 * \brief Wrapper class around a DomItem to store PlanPro Ur_Objekt subtypes.
 *
 * This class encapsulates a DomItem pointing to the root of an Ur_Objekt subtype and provides direct accessor methods to all properties.
 * Because Ur_Objekt is an abstract type in PlanPro, this class cannot be instantiated directly.
 * Use one of the specific subtypes to create objects.
 */
class UrObjekt
{
public:
    /*!
     * \brief Checks if this is a valid UrObjekt subtype.
     *
     * This is the case if the encapsulated DomItem points to the root of an Ur_Objekt subtype,
     * which is determined by checking if a child item at the path Identitaet/Wert exists.
     *
     * \return true if this is a valid UrObjekt subtype, otherwise false.
     */
    virtual bool isUrObjekt() const;

    /*!
     * \brief Get the encapsulated DomItem.
     *
     * \return the DomItem
     */
    DomItem* getDomItem();

    /*!
     * \brief Get the encapsulated DomItem.
     *
     * \return the DomItem
     */
    const DomItem* getDomItem() const;

    /*!
     * \brief Get the name of the UrObjekt.
     *
     * Note that the real name of the subtype is returned, like Signal or Datenpunkt.
     *
     * \return the name of the UrObjekt, or an empty string if the encapsulated DomItem is NULL
     */
    virtual QString getName() const;

    /*!
     * \brief Get the identity of the object.
     *
     * \return the identity as UUID string, or an empty string if the encapsulated DomItem is NULL or no Ur_Objekt
     */
    virtual QString getIdentitaet() const;

    /*!
     * \brief Get the identity of an object provided as DomItem.
     *
     * \param item the DomItem which is the root of the object
     * \return the identity as UUID string, or an empty string if item is NULL or no Ur_Objekt
     */
    static QString getIdentitaet(const DomItem* item);

protected:
    /*!
     * \brief Constructor.
     *
     * \param item the DomItem to encapsulate
     */
    UrObjekt(DomItem* item = nullptr);

    DomItem* domItem; /*!< \brief The encapulated DOM item. */
};

#endif // UROBJEKT_H
