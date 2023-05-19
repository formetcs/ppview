#include "basisobjekt.h"
#include "domitem.h"

BasisObjekt::BasisObjekt(DomItem* item) : UrObjekt(item)
{
}

bool BasisObjekt::isBasisObjekt()
{
    if(!domItem)
    {
        return false;
    }
    return (isUrObjekt() && (domItem->getFirstItemAtPath("Basis_Objekt_Allg") != NULL));
}

int BasisObjekt::getIdBearbeitungsvermerkCount()
{
    if(!domItem)
    {
        return 0;
    }
    return domItem->getChildItems("ID_Bearbeitungsvermerk").count();
}

QString BasisObjekt::getIdBearbeitungsvermerk(int i)
{
    if(!domItem || i < 0 || i >= getIdBearbeitungsvermerkCount())
    {
        return QString();
    }
    QList<DomItem*> childlist = domItem->getChildItems("ID_Bearbeitungsvermerk");
    return childlist.at(i)->getFirstValueAtPath("Wert");
}

QString BasisObjekt::getDbGdiReferenz()
{
    if(!domItem)
    {
        return QString();
    }
    return domItem->getFirstValueAtPath("Objektreferenzen/DB_GDI_Referenz/Wert");
}

QString BasisObjekt::getTechnischerPlatz()
{
    if(!domItem)
    {
        return QString();
    }
    return domItem->getFirstValueAtPath("Objektreferenzen/Technischer_Platz/Wert");
}

QString BasisObjekt::getObjektzustandBesonders()
{
    if(!domItem)
    {
        return QString();
    }
    return domItem->getFirstValueAtPath("Basis_Objekt_Allg/Objektzustand_Besonders/Wert");
}

bool BasisObjekt::getBestandsschutz()
{
    if(!domItem)
    {
        return false;
    }
    return (domItem->getFirstValueAtPath("Basis_Objekt_Allg/Bestandsschutz/Wert") == "true");
}

QString BasisObjekt::getIdAnhangRegelwerkBesonders()
{
    if(!domItem)
    {
        return QString();
    }
    return domItem->getFirstValueAtPath("Basis_Objekt_Allg/ID_Anhang_Regelwerk_Besonders/Wert");
}

QString BasisObjekt::getDatumRegelwerk()
{
    if(!domItem)
    {
        return QString();
    }
    return domItem->getFirstValueAtPath("Basis_Objekt_Allg/Datum_Regelwerk/Wert");
}
