#ifndef UROBJEKT_H
#define UROBJEKT_H

#include <QString>

class DomItem;

class UrObjekt
{
public:
    UrObjekt(DomItem* item = nullptr);
    bool isUrObjekt();
    QString getIdentitaet();

protected:
    DomItem* domItem;
};

#endif // UROBJEKT_H
