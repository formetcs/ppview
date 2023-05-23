#ifndef ANHANG_H
#define ANHANG_H

#include "urobjekt.h"

#include <QByteArray>

class Anhang : public UrObjekt
{
public:
    Anhang(DomItem* item = nullptr);
    virtual bool isAnhang() const;

    virtual QString getAnhangArt() const;
    virtual QString getDateiname() const;
    virtual QString getDateityp() const;
    virtual QByteArray getDaten() const;
};

#endif // ANHANG_H
