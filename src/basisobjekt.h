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

class BasisObjekt : public UrObjekt
{
public:
    virtual bool isBasisObjekt() const;

    virtual int getIdBearbeitungsvermerkCount() const;
    virtual QString getIdBearbeitungsvermerk(int i) const;
    virtual QString getDbGdiReferenz() const;
    virtual QString getTechnischerPlatz() const;
    virtual QString getObjektzustandBesonders() const;
    virtual bool getBestandsschutz() const;
    virtual QString getIdAnhangRegelwerkBesonders() const;
    virtual QString getDatumRegelwerk() const;

protected:
    BasisObjekt(DomItem* item = nullptr);
};

#endif // BASISOBJEKT_H
