#include "domitem.h"
#include "urobjekt.h"

UrObjekt::UrObjekt(DomItem* item)
{
    domItem = item;
}

bool UrObjekt::isUrObjekt()
{
    if(!domItem)
    {
        return false;
    }
    return (domItem->getFirstItemAtPath("Identitaet/Wert") != NULL);
}

QString UrObjekt::getIdentitaet()
{
    if(!domItem)
    {
        return QString();
    }
    return domItem->getFirstValueAtPath("Identitaet/Wert");
}
