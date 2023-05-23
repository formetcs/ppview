#ifndef UROBJEKT_H
#define UROBJEKT_H

#include <QString>

class DomItem;

class UrObjekt
{
public:
    virtual bool isUrObjekt() const;
    DomItem* getDomItem();
    const DomItem* getDomItem() const;

    virtual QString getName() const;
    virtual QString getIdentitaet() const;
    static QString getIdentitaet(const DomItem* item);

protected:
    UrObjekt(DomItem* item = nullptr);

    DomItem* domItem;
};

#endif // UROBJEKT_H
