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

#ifndef DOMITEM_H
#define DOMITEM_H

#include <QString>
#include <QHash>
#include <QList>


class DomItem
{
public:
    DomItem(const QString& name, const QString& value, DomItem* parent = NULL);
    DomItem(const QString& name, DomItem* parent = NULL);
    ~DomItem();
    QString getName() const;
    QString getValue() const;
    void setValue(const QString& v);
    DomItem* getChild(int i);
    const DomItem* getChild(int i) const;
    DomItem* getFirstChildItem(const QString& c);
    const DomItem* getFirstChildItem(const QString& c) const;
    QList<DomItem*> getChildItems(const QString& c);
    QList<const DomItem*> getChildItems(const QString& c) const;
    DomItem* getFirstItemAtPath(const QString& p);
    const DomItem* getFirstItemAtPath(const QString& p) const;
    QString getFirstValueAtPath(const QString& p) const;
    void addChild(DomItem* c);
    int childCount() const;
    int getOwnPosition() const;
    QString getAttribute(const QString& name) const;
    void addAttribute(const QString& name, const QString& value);
    QHash<QString, QString> getAttributeMap() const;
    DomItem* parent();
    const DomItem* parent() const;

private:
    QString name;
    QString value;
    QList<DomItem*> childItems;
    QHash<QString, QString> attributes;
    DomItem* parentItem;
};

#endif // DOMITEM_H
