#ifndef UROBJEKT_H
#define UROBJEKT_H

#include <QString>

class DomItem;

class UrObjekt
{
public:
    bool isUrObjekt();
    DomItem* getDomItem() const;

    virtual QString getName() const;
    virtual QString getIdentitaet();
    static QString getIdentitaet(DomItem* item);

protected:
    UrObjekt(DomItem* item = nullptr);

    DomItem* domItem;
};

#endif // UROBJEKT_H
