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


/*!
 * \brief Class to create a standalone PunktObjekt without the need to connect it with a DomItem.
 *
 * A TemporaryPunktObjekt provides the same interface as a PunktObjekt, but in opposite to a real PunktObjekt, it has no connections to a real PlanPro object tree.
 * It can be used to create objects which are not part of the PlanPro model, either as temporary objects, or to add it to the PlanPro tree later.
 * A common usage is the creation of intermediate objects for the recursive distance calculations on the PlanPro graph.
 *
 * Use always the reimplemented accessor methods from this class and no methods inherited unchanged from PunktObjekt, because they depend on the non-existing DomItem and provide wrong values!
 */
class TemporaryPunktObjekt : public PunktObjekt
{
public:
    /*!
     * \brief Constructor.
     *
     * The created object has an identity and only one Punkt_Objekt_Top_Kante group with the id of the referenced TOP_Kante,
     * distance and effective direction. It has no Punkt_Objekt_Strecke group and no additional attributes.
     *
     * \param id the identity of the object as UUID string
     * \param idtk the identity of the referenced TOP_Kante as UUID string
     * \param abst the distance from the beginning of the TOP_Kante in millimeters
     * \param wirkr the effective direction
     */
    TemporaryPunktObjekt(QString id, QString idtk, int abst, QString wirkr);

    /*!
     * \brief Checks if this is a valid PunktObjekt subtype. Is always true.
     *
     * \return always true
     */
    bool isPunktObjekt() const;

    /*!
     * \brief Get the Identity of the object.
     *
     * \return the identity as UUID string.
     */
    QString getIdentitaet() const;

    /*!
     * \brief Get the number of Punkt_Objekt_TOP_Kante entries. Is always 1.
     *
     * \return always 1
     */
    int getPunktObjektTopKanteCount() const;

    /*!
     * \brief Get the identity of the referenced TOP_Kante.
     *
     * \param i the position of the entry (must be 0)
     * \return the identity of the referenced TOP_Kante, or an empty string if i is not 0
     */
    QString getIdTopKante(int i) const;

    /*!
     * \brief Get the distance from the beginning of the TOP_Kante in millimeters.
     *
     * \param i the position of the entry (must be 0)
     * \return the distance from the beginning of the TOP_Kante in millimeters, or 0 if i is not 0
     */
    int getAbstand(int i) const;

    /*!
     * \brief Get the effective direction.
     *
     * \param i the position of the entry (must be 0)
     * \return the effective direction, or an empty string if i is not 0
     */
    QString getWirkrichtung(int i) const;

    /*!
     * \brief Get the lateral position.
     *
     * This property is not used and returns always an empty string.
     *
     * \param i the position of the entry (must be 0)
     * \return always an empty string
     */
    QString getSeitlicheLage(int i) const;

    /*!
     * \brief Get the lateral distance.
     *
     * This property is not used and returns always 0.
     *
     * \param i the position of the entry (must be 0)
     * \return always 0
     */
    int getSeitlicherAbstand(int i) const;

private:
    QString identitaet; /*!< \brief The identity of the object as UUID string. */
    QString idTopKante; /*!< \brief The identity of the referenced TOP_Kante as UUID string. */
    int abstand; /*!< \brief The distance from the beginning of the TOP_Kante in millimeters. */
    QString wirkrichtung; /*!< \brief The effective direction. */
};

#endif // TEMPORARYPUNKTOBJEKT_H
