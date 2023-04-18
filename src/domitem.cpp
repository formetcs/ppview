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

QString DomItem::getFirstValueAtPath(const QString& p)
{
    DomItem* d = getFirstItemAtPath(p);
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

QHash<QString, QString> DomItem::getAttributeMap()
{
    return attributes;
}

DomItem* DomItem::parent()
{
    return parentItem;
}
