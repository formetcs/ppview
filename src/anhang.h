#ifndef ANHANG_H
#define ANHANG_H

#include "urobjekt.h"

#include <QByteArray>

class Anhang : public UrObjekt
{
public:
    Anhang(DomItem* item = nullptr);
    bool isAnhang();
    QString getAnhangArt();
    QString getDateiname();
    QString getDateityp();
    QByteArray getDaten();
};

#endif // ANHANG_H
