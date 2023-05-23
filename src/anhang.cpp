#include "anhang.h"
#include "domitem.h"

Anhang::Anhang(DomItem* item) : UrObjekt(item)
{
}

bool Anhang::isAnhang() const
{
    if(!domItem)
    {
        return false;
    }
    return (isUrObjekt() && (domItem->getFirstItemAtPath("Anhang_Allg/Anhang_Art") != NULL));
}

QString Anhang::getAnhangArt() const
{
    if(!domItem)
    {
        return QString();
    }
    return domItem->getFirstValueAtPath("Anhang_Allg/Anhang_Art/Wert");
}

QString Anhang::getDateiname() const
{
    if(!domItem)
    {
        return QString();
    }
    return domItem->getFirstValueAtPath("Anhang_Allg/Dateiname/Wert");
}

QString Anhang::getDateityp() const
{
    if(!domItem)
    {
        return QString();
    }
    return domItem->getFirstValueAtPath("Anhang_Allg/Dateityp/Wert");
}

QByteArray Anhang::getDaten() const
{
    if(!domItem)
    {
        return QByteArray();
    }
    QString base64Data = domItem->getFirstValueAtPath("Anhang_Allg/Daten/Wert");
    QByteArray encodedData = base64Data.toLatin1();
    QByteArray finalData = QByteArray::fromBase64(encodedData);
    return finalData;
}
