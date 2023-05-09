#include "anhang.h"
#include "domitem.h"

Anhang::Anhang(DomItem* item) : UrObjekt(item)
{
}

bool Anhang::isAnhang()
{
    if(!domItem)
    {
        return false;
    }
    return (isUrObjekt() && (domItem->getFirstItemAtPath("Anhang_Allg/Anhang_Art") != NULL));
}

QString Anhang::getAnhangArt()
{
    if(!domItem)
    {
        return QString();
    }
    return domItem->getFirstValueAtPath("Anhang_Allg/Anhang_Art/Wert");
}

QString Anhang::getDateiname()
{
    if(!domItem)
    {
        return QString();
    }
    return domItem->getFirstValueAtPath("Anhang_Allg/Dateiname/Wert");
}

QString Anhang::getDateityp()
{
    if(!domItem)
    {
        return QString();
    }
    return domItem->getFirstValueAtPath("Anhang_Allg/Dateityp/Wert");
}

QByteArray Anhang::getDaten()
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
