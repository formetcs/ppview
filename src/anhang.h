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

#ifndef ANHANG_H
#define ANHANG_H

#include "urobjekt.h"

#include <QByteArray>

class Anhang : public UrObjekt
{
public:
    Anhang(DomItem* item = nullptr);
    virtual bool isAnhang() const;

    virtual QString getAnhangArt() const;
    virtual QString getDateiname() const;
    virtual QString getDateityp() const;
    virtual QByteArray getDaten() const;
};

#endif // ANHANG_H
