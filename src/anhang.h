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
 * \file anhang.h
 *
 * \brief Declaration of the Anhang class.
 */

#ifndef ANHANG_H
#define ANHANG_H

#include "urobjekt.h"

#include <QByteArray>


/*!
 * \brief Wrapper class around a DomItem to store PlanPro %Anhang types.
 *
 * This class encapsulates a DomItem pointing to the root of an %Anhang type and provides direct accessor methods to all properties.
 */
class Anhang : public UrObjekt
{
public:
    /*!
     * \brief Constructor.
     *
     * \param item the DomItem to encapsulate
     */
    Anhang(DomItem* item = nullptr);

    /*!
     * \brief Checks if this is a valid Anhang type.
     *
     * This is the case if this is also an UrObjekt and the encapsulated DomItem points to the root of an %Anhang type,
     * which is determined by checking if a child item at the path Anhang_Allg/Anhang_Art exists.
     *
     * \return true if this is a valid Anhang type, otherwise false.
     */
    virtual bool isAnhang() const;

    /*!
     * \brief Get the value of the property Anhang_Art.
     *
     * \return the value of the property Anhang_Art, or an empty string if it does not exist
     */
    virtual QString getAnhangArt() const;

    /*!
     * \brief Get the value of the property Dateiname.
     *
     * \return the value of the property Dateiname, or an empty string if it does not exist
     */
    virtual QString getDateiname() const;

    /*!
     * \brief Get the value of the property Dateityp.
     *
     * \return the value of the property Dateityp, or an empty string if it does not exist
     */
    virtual QString getDateityp() const;

    /*!
     * \brief Get the value of the property Daten, decoded from the Base64 format.
     *
     * \return the decoded value of the property Daten, or an empty byte array if it does not exist
     */
    virtual QByteArray getDaten() const;
};

#endif // ANHANG_H
