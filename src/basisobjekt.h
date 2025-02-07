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

#ifndef BASISOBJEKT_H
#define BASISOBJEKT_H

#include "urobjekt.h"


/*!
 * \brief Wrapper class around a DomItem to store PlanPro Basis_Objekt subtypes.
 *
 * This class encapsulates a DomItem pointing to the root of a Basis_Objekt subtype and provides direct accessor methods to all properties.
 * Because Basis_Objekt is an abstract type in PlanPro, this class cannot be instantiated directly.
 * Use one of the specific subtypes to create objects.
 */
class BasisObjekt : public UrObjekt
{
public:
    /*!
     * \brief Checks if this is a valid BasisObjekt subtype.
     *
     * This is the case if this is also an UrObjekt and the encapsulated DomItem points to the root of a Basis_Objekt subtype,
     * which is determined by checking if a child item at the path Basis_Objekt_Allg exists.
     *
     * \return true if this is a valid BasisObjekt subtype, otherwise false.
     */
    virtual bool isBasisObjekt() const;

    /*!
     * \brief Get the number of ID_Bearbeitungsvermerk entries.
     *
     * \return the number of ID_Bearbeitungsvermerk entries
     */
    virtual int getIdBearbeitungsvermerkCount() const;

    /*!
     * \brief Get the value of the property ID_Bearbeitungsvermerk at a given position.
     *
     * \param i the position of the entry
     * \return the value of the property ID_Bearbeitungsvermerk, or an empty string if it does not exist
     */
    virtual QString getIdBearbeitungsvermerk(int i) const;

    /*!
     * \brief Get the value of the property DB_GDI_Referenz.
     *
     * \return the value of the property DB_GDI_Referenz, or an empty string if it does not exist
     */
    virtual QString getDbGdiReferenz() const;

    /*!
     * \brief Get the value of the property Technischer_Platz.
     *
     * \return the value of the property Technischer_Platz, or an empty string if it does not exist
     */
    virtual QString getTechnischerPlatz() const;

    /*!
     * \brief Get the value of the property Objektzustand_Besonders.
     *
     * \return the value of the property Objektzustand_Besonders, or an empty string if it does not exist
     */
    virtual QString getObjektzustandBesonders() const;

    /*!
     * \brief Get the value of the property Bestandsschutz.
     *
     * \return the value of the property Bestandsschutz, or false if it does not exist
     */
    virtual bool getBestandsschutz() const;

    /*!
     * \brief Get the value of the property ID_Anhang_Regelwerk_Besonders.
     *
     * \return the value of the property ID_Anhang_Regelwerk_Besonders, or an empty string if it does not exist
     */
    virtual QString getIdAnhangRegelwerkBesonders() const;

    /*!
     * \brief Get the value of the property Datum_Regelwerk.
     *
     * \return the value of the property Datum_Regelwerk, or an empty string if it does not exist
     */
    virtual QString getDatumRegelwerk() const;

protected:
    /*!
     * \brief Constructor.
     *
     * \param item the DomItem to encapsulate
     */
    BasisObjekt(DomItem* item = nullptr);
};

#endif // BASISOBJEKT_H
