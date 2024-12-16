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

DomItem::DomItem(const QString& name, const QString& value, DomItem* parent)
{
    this->name = name;
    this->value = value;
    parentItem = parent;
}

DomItem::DomItem(const QString& name, DomItem* parent)
{
    this->name = name;
    this->value = QString();
    parentItem = parent;
}

DomItem::~DomItem()
{
    for(int i = 0; i < childItems.count(); ++i)
        delete childItems.at(i);
}

QString DomItem::getName() const
{
    return name;
}

QString DomItem::getValue() const
{
    return value;
}

void DomItem::setValue(const QString& v)
{
    value = v;
}

DomItem* DomItem::getChild(int i)
{
    if(i < 0 || i > childItems.count())
    {
        return NULL;
    }
    return childItems.at(i);
}

const DomItem* DomItem::getChild(int i) const
{
    if(i < 0 || i > childItems.count())
    {
        return NULL;
    }
    return childItems.at(i);
}

DomItem* DomItem::getFirstChildItem(const QString& c)
{
    for(int i = 0; i < childItems.count(); ++i)
    {
        DomItem* temp = childItems.at(i);
        if(temp->getName() == c)
        {
            return temp;
        }
    }
    return NULL;
}

const DomItem* DomItem::getFirstChildItem(const QString& c) const
{
    for(int i = 0; i < childItems.count(); ++i)
    {
        DomItem* temp = childItems.at(i);
        if(temp->getName() == c)
        {
            return temp;
        }
    }
    return NULL;
}

QList<DomItem*> DomItem::getChildItems(const QString& c)
{
    QList<DomItem*> returnlist;
    for(int i = 0; i < childItems.count(); ++i)
    {
        DomItem* temp = childItems.at(i);
        if(temp->getName() == c)
        {
            returnlist.append(temp);
        }
    }
    return returnlist;
}

QList<const DomItem *> DomItem::getChildItems(const QString& c) const
{
    QList<const DomItem*> returnlist;
    for(int i = 0; i < childItems.count(); ++i)
    {
        DomItem* temp = childItems.at(i);
        if(temp->getName() == c)
        {
            returnlist.append(temp);
        }
    }
    return returnlist;
}

DomItem* DomItem::getFirstItemAtPath(const QString& p)
{
    if(p.isEmpty())
    {
        return this;
    }

    int pos = p.indexOf("/");
    QString first = p;
    QString last = QString();
    if(pos >= 0)
    {
        first = p.first(pos);
        last = p.last(p.length() - pos - 1);
    }

    for(int i = 0; i < childItems.count(); ++i)
    {
        DomItem* temp = childItems.at(i);
        if(temp->getName() == first)
        {
            return temp->getFirstItemAtPath(last);
        }
    }
    return NULL;
}

const DomItem* DomItem::getFirstItemAtPath(const QString& p) const
{
    if(p.isEmpty())
    {
        return this;
    }

    int pos = p.indexOf("/");
    QString first = p;
    QString last = QString();
    if(pos >= 0)
    {
        first = p.first(pos);
        last = p.last(p.length() - pos - 1);
    }

    for(int i = 0; i < childItems.count(); ++i)
    {
        DomItem* temp = childItems.at(i);
        if(temp->getName() == first)
        {
            return temp->getFirstItemAtPath(last);
        }
    }
    return NULL;
}

QString DomItem::getFirstValueAtPath(const QString& p) const
{
    const DomItem* d = getFirstItemAtPath(p);
    if(d)
    {
        return d->getValue();
    }
    return QString();
}

void DomItem::addChild(DomItem* c)
{
    childItems.append(c);
}

int DomItem::childCount() const
{
    return childItems.count();
}

int DomItem::getOwnPosition() const
{
    if(!parentItem)
    {
        return 0;
    }
    for(int i = 0; i < parentItem->childCount(); ++i)
    {
        if(parentItem->getChild(i) == this)
        {
            return i;
        }
    }
    return 0;
}

QString DomItem::getAttribute(const QString& name) const
{
    return attributes.value(name);
}

void DomItem::addAttribute(const QString& name, const QString& value)
{
    attributes.insert(name, value);
}

QHash<QString, QString> DomItem::getAttributeMap() const
{
    return attributes;
}

DomItem* DomItem::parent()
{
    return parentItem;
}

const DomItem* DomItem::parent() const
{
    return parentItem;
}
