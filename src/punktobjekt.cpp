#include "punktobjekt.h"
#include "domitem.h"

PunktObjektStrecke::PunktObjektStrecke(DomItem* item)
{
    domItem = item;
}

QString PunktObjektStrecke::getIdStrecke() const
{
    if(!domItem)
    {
        return QString();
    }
    return domItem->getFirstValueAtPath("ID_Strecke/Wert");
}

QString PunktObjektStrecke::getStreckeKm() const
{
    if(!domItem)
    {
        return QString();
    }
    return domItem->getFirstValueAtPath("Strecke_Km/Wert");
}




PunktObjektTopKante::PunktObjektTopKante(DomItem* item)
{
    domItem = item;
}

QString PunktObjektTopKante::getIdTopKante() const
{
    if(!domItem)
    {
        return QString();
    }
    return domItem->getFirstValueAtPath("ID_TOP_Kante/Wert");
}

int PunktObjektTopKante::getAbstand() const
{
    if(!domItem)
    {
        return 0;
    }
    double val = domItem->getFirstValueAtPath("Abstand/Wert").toDouble();
    return (int) (val * 1000.0 + 0.5);
}

QString PunktObjektTopKante::getWirkrichtung() const
{
    if(!domItem)
    {
        return QString();
    }
    return domItem->getFirstValueAtPath("Wirkrichtung/Wert");
}

QString PunktObjektTopKante::getSeitlicheLage() const
{
    if(!domItem)
    {
        return QString();
    }
    return domItem->getFirstValueAtPath("Seitliche_Lage/Wert");
}

int PunktObjektTopKante::getSeitlicherAbstand() const
{
    if(!domItem)
    {
        return 0;
    }
    double val = domItem->getFirstValueAtPath("Seitlicher_Abstand/Wert").toDouble();
    return (int) (val * 1000.0 + 0.5);
}




PunktObjekt::PunktObjekt(DomItem* item) : BasisObjekt(item)
{
}

bool PunktObjekt::isPunktObjekt() const
{
    if(!domItem)
    {
        return false;
    }
    return (isBasisObjekt() && (domItem->getFirstItemAtPath("Punkt_Objekt_TOP_Kante") != NULL));
}

int PunktObjekt::getPunktObjektStreckeCount() const
{
    if(!domItem)
    {
        return 0;
    }
    return domItem->getChildItems("Punkt_Objekt_Strecke").count();
}

PunktObjektStrecke PunktObjekt::getPunktObjektStrecke(int i) const
{
    if(!domItem || i < 0 || i >= getPunktObjektStreckeCount())
    {
        return PunktObjektStrecke();
    }
    QList<DomItem*> childlist = domItem->getChildItems("Punkt_Objekt_Strecke");
    return PunktObjektStrecke(childlist.at(i));
}

QString PunktObjekt::getIdStrecke(int i) const
{
    if(!domItem || i < 0 || i >= getPunktObjektStreckeCount())
    {
        return QString();
    }
    QList<DomItem*> childlist = domItem->getChildItems("Punkt_Objekt_Strecke");
    return childlist.at(i)->getFirstValueAtPath("ID_Strecke/Wert");
}

QString PunktObjekt::getStreckeKm(int i) const
{
    if(!domItem || i < 0 || i >= getPunktObjektStreckeCount())
    {
        return QString();
    }
    QList<DomItem*> childlist = domItem->getChildItems("Punkt_Objekt_Strecke");
    return childlist.at(i)->getFirstValueAtPath("Strecke_Km/Wert");
}

int PunktObjekt::getPunktObjektTopKanteCount() const
{
    if(!domItem)
    {
        return 0;
    }
    return domItem->getChildItems("Punkt_Objekt_Strecke").count();
}

PunktObjektTopKante PunktObjekt::getPunktObjektTopKante(int i) const
{
    if(!domItem || i < 0 || i >= getPunktObjektTopKanteCount())
    {
        return PunktObjektTopKante();
    }
    QList<DomItem*> childlist = domItem->getChildItems("Punkt_Objekt_TOP_Kante");
    return PunktObjektTopKante(childlist.at(i));
}

QString PunktObjekt::getIdTopKante(int i) const
{
    if(!domItem || i < 0 || i >= getPunktObjektTopKanteCount())
    {
        return QString();
    }
    QList<DomItem*> childlist = domItem->getChildItems("Punkt_Objekt_TOP_Kante");
    return childlist.at(i)->getFirstValueAtPath("ID_TOP_Kante/Wert");
}

int PunktObjekt::getAbstand(int i) const
{
    if(!domItem || i < 0 || i >= getPunktObjektTopKanteCount())
    {
        return 0;
    }
    QList<DomItem*> childlist = domItem->getChildItems("Punkt_Objekt_TOP_Kante");
    double val = childlist.at(i)->getFirstValueAtPath("Abstand/Wert").toDouble();
    return (int) (val * 1000.0 + 0.5);
}

QString PunktObjekt::getWirkrichtung(int i) const
{
    if(!domItem || i < 0 || i >= getPunktObjektTopKanteCount())
    {
        return QString();
    }
    QList<DomItem*> childlist = domItem->getChildItems("Punkt_Objekt_TOP_Kante");
    return childlist.at(i)->getFirstValueAtPath("Wirkrichtung/Wert");
}

QString PunktObjekt::getSeitlicheLage(int i) const
{
    if(!domItem || i < 0 || i >= getPunktObjektTopKanteCount())
    {
        return QString();
    }
    QList<DomItem*> childlist = domItem->getChildItems("Punkt_Objekt_TOP_Kante");
    return childlist.at(i)->getFirstValueAtPath("Seitliche_Lage/Wert");
}

int PunktObjekt::getSeitlicherAbstand(int i) const
{
    if(!domItem || i < 0 || i >= getPunktObjektTopKanteCount())
    {
        return 0;
    }
    QList<DomItem*> childlist = domItem->getChildItems("Punkt_Objekt_TOP_Kante");
    double val = childlist.at(i)->getFirstValueAtPath("Seitlicher_Abstand/Wert").toDouble();
    return (int) (val * 1000.0 + 0.5);
}
