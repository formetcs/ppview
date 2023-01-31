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
    this->value = "";
    parentItem = parent;
}

DomItem::~DomItem()
{
    for(int i = 0; i < childItems.count(); i++)
        delete childItems[i];
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
    return childItems[i];
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
    for(int i = 0; i < parentItem->childCount(); i++)
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

QHash<QString, QString> DomItem::getAttributeMap()
{
    return attributes;
}

DomItem* DomItem::parent()
{
    return parentItem;
}
