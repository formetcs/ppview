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
 * \file punktobjekt.h
 *
 * \brief Declaration of the PunktObjekt, PunktObjektStrecke and PunktObjektTopKante classes.
 */

#ifndef PUNKTOBJEKT_H
#define PUNKTOBJEKT_H

#include "basisobjekt.h"

#include <QString>


/*!
 * \brief Wrapper class around a DomItem to store a PlanPro Punkt_Objekt_Strecke substructure inside a PunktObjekt.
 *
 * This class encapsulates a DomItem pointing to the root of a Punkt_Objekt_Strecke substructure and provides direct accessor methods to all properties.
 */
class PunktObjektStrecke
{
public:
    /*!
     * \brief Constructor.
     *
     * \param item the DomItem to encapsulate
     */
    PunktObjektStrecke(DomItem* item = nullptr);

    /*!
     * \brief Get the value of the property ID_Strecke.
     *
     * \return the value of the property ID_Strecke, or an empty string if it does not exist
     */
    virtual QString getIdStrecke() const;

    /*!
     * \brief Get the value of the property Strecke_Km.
     *
     * \return the value of the property Strecke_Km, or an empty string if it does not exist
     */
    virtual QString getStreckeKm() const;

protected:
    DomItem* domItem; /*!< \brief The encapulated DOM item. */
};


/*!
 * \brief Wrapper class around a DomItem to store a PlanPro Punkt_Objekt_TOP_Kante substructure inside a PunktObjekt.
 *
 * This class encapsulates a DomItem pointing to the root of a Punkt_Objekt_TOP_Kante substructure and provides direct accessor methods to all properties.
 */
class PunktObjektTopKante
{
public:
    /*!
     * \brief Constructor.
     *
     * \param item the DomItem to encapsulate
     */
    PunktObjektTopKante(DomItem* item = nullptr);

    /*!
     * \brief Get the value of the property ID_TOP_Kante.
     *
     * \return the value of the property ID_TOP_Kante, or an empty string if it does not exist
     */
    virtual QString getIdTopKante() const;

    /*!
     * \brief Get the value of the property Abstand in millimeters.
     *
     * \return the value of the property Abstand in millimeters, or 0 if it does not exist
     */
    virtual int getAbstand() const;

    /*!
     * \brief Get the value of the property Wirkrichtung.
     *
     * \return the value of the property Wirkrichtung, or an empty string if it does not exist
     */
    virtual QString getWirkrichtung() const;

    /*!
     * \brief Get the value of the property Seitliche_Lage.
     *
     * \return the value of the property Seitliche_Lage, or an empty string if it does not exist
     */
    virtual QString getSeitlicheLage() const;

    /*!
     * \brief Get the value of the property Seitlicher_Abstand in millimeters.
     *
     * \return the value of the property Seitlicher_Abstand in millimeters, or 0 if it does not exist
     */
    virtual int getSeitlicherAbstand() const;

protected:
    DomItem* domItem; /*!< \brief The encapulated DOM item. */
};


/*!
 * \brief Wrapper class around a DomItem to store PlanPro Punkt_Objekt subtypes.
 *
 * This class encapsulates a DomItem pointing to the root of a Punkt_Objekt subtype and provides direct accessor methods to all properties.
 */
class PunktObjekt : public BasisObjekt
{
public:
    /*!
     * \brief Constructor.
     *
     * \param item the DomItem to encapsulate
     */
    PunktObjekt(DomItem* item = nullptr);

    /*!
     * \brief Checks if this is a valid PunktObjekt subtype.
     *
     * This is the case if this is also a BasisObjekt and the encapsulated DomItem points to the root of a Punkt_Objekt subtype,
     * which is determined by checking if a child item at the path Punkt_Objekt_TOP_Kante exists.
     *
     * \return true if this is a valid PunktObjekt subtype, otherwise false.
     */
    virtual bool isPunktObjekt() const;

    /*!
     * \brief Get the number of Punkt_Objekt_Strecke entries.
     *
     * \return the number of Punkt_Objekt_Strecke entries
     */
    virtual int getPunktObjektStreckeCount() const;

    /*!
     * \brief Get the Punkt_Objekt_Strecke substructure at a given position, encapsulated into a PunktObjektStrecke object.
     *
     * \param i the position of the entry
     * \return a PunktObjektStrecke object encapsulating the Punkt_Objekt_Strecke substructure, or an empty PunktObjektStrecke object if it does not exist
     */
    virtual PunktObjektStrecke getPunktObjektStrecke(int i) const;

    /*!
     * \brief Get the value of the property ID_Strecke at a given position.
     *
     * \param i the position of the entry
     * \return the value of the property ID_Strecke, or an empty string if it does not exist
     */
    virtual QString getIdStrecke(int i) const;

    /*!
     * \brief Get the value of the property Strecke_Km at a given position.
     *
     * \param i the position of the entry
     * \return the value of the property Strecke_Km, or an empty string if it does not exist
     */
    virtual QString getStreckeKm(int i) const;

    /*!
     * \brief Get the number of Punkt_Objekt_TOP_Kante entries.
     *
     * \return the number of Punkt_Objekt_TOP_Kante entries
     */
    virtual int getPunktObjektTopKanteCount() const;

    /*!
     * \brief Get the Punkt_Objekt_TOP_Kante substructure at a given position, encapsulated into a PunktObjektTopKante object.
     *
     * \param i the position of the entry
     * \return a PunktObjektTopKante object encapsulating the Punkt_Objekt_TOP_Kante substructure, or an empty PunktObjektTopKante object if it does not exist
     */
    virtual PunktObjektTopKante getPunktObjektTopKante(int i) const;

    /*!
     * \brief Get the value of the property ID_TOP_Kante at a given position.
     *
     * \param i the position of the entry
     * \return the value of the property ID_TOP_Kante, or an empty string if it does not exist
     */
    virtual QString getIdTopKante(int i) const;

    /*!
     * \brief Get the value of the property Abstand in millimeters at a given position.
     *
     * \param i the position of the entry
     * \return the value of the property Abstand in millimeters, or 0 if it does not exist
     */
    virtual int getAbstand(int i) const;

    /*!
     * \brief Get the value of the property Wirkrichtung at a given position.
     *
     * \param i the position of the entry
     * \return the value of the property Wirkrichtung, or an empty string if it does not exist
     */
    virtual QString getWirkrichtung(int i) const;

    /*!
     * \brief Get the value of the property Seitliche_Lage at a given position.
     *
     * \param i the position of the entry
     * \return the value of the property Seitliche_Lage, or an empty string if it does not exist
     */
    virtual QString getSeitlicheLage(int i) const;

    /*!
     * \brief Get the value of the property Seitlicher_Abstand in millimeters at a given position.
     *
     * \param i the position of the entry
     * \return the value of the property Seitlicher_Abstand in millimeters, or 0 if it does not exist
     */
    virtual int getSeitlicherAbstand(int i) const;

protected:

};

#endif // PUNKTOBJEKT_H
