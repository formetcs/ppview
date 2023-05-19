#ifndef ANHANG_H
#define ANHANG_H

#include "urobjekt.h"

#include <QByteArray>

class Anhang : public UrObjekt
{
public:
    Anhang(DomItem* item = nullptr);
    bool isAnhang();

    virtual QString getAnhangArt();
    virtual QString getDateiname();
    virtual QString getDateityp();
    virtual QByteArray getDaten();
};

#endif // ANHANG_H
