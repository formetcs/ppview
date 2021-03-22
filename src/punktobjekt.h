#ifndef PUNKTOBJEKT_H
#define PUNKTOBJEKT_H

#include <QString>


class PunktObjekt
{
public:
    PunktObjekt();
    PunktObjekt(QString idtk, double abst, QString wirkr);
    PunktObjekt(QString guid, QString idtk, double abst, QString wirkr);

    void setIdentitaet(QString i);
    QString getIdentitaet();
    void setIdTopKante(QString i);
    QString getIdTopKante();
    void setAbstand(double a);
    double getAbstand();
    void setWirkrichtung(QString w);
    QString getWirkrichtung();

private:
    QString identitaet;
    QString idTopKante;
    double abstand;
    QString wirkrichtung;
};

#endif // PUNKTOBJEKT_H
