#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QtXml>
#include <QString>

class PlanProModel;
class GraphicsScene;
class QGraphicsItem;

class Parser : public QObject
{
    Q_OBJECT
public:
    static void createGraphicsScene(PlanProModel* model, GraphicsScene* scene);


signals:

public slots:

private slots:

private:
    static QGraphicsItem* placePunktObjekt(QDomElement o, PlanProModel* model, GraphicsScene* scene);
    static QGraphicsItem* placeBereichObjekt(QDomElement o, PlanProModel* model, GraphicsScene* scene, const QPen &pen);

    static int getMaxDomDepth(QDomNode o);
    static int getLeafCount(QDomNode o);
    static QString generateInfoText(QDomNode o);
    static QString generateInfoTextRec(QDomNode o, int currentDepth, int maxDepth);

    static QString getIdentitaet(QDomElement e);
    static QString getPunkt_Objekt_ID_TOP_Kante(QDomElement e);
    static double getPunkt_Objekt_Abstand(QDomElement e);
    static double getPunkt_Objekt_Seitlicher_Abstand(QDomElement e);
    static QString getPunkt_Objekt_Seitliche_Lage(QDomElement e);
    static QString getPunkt_Objekt_Wirkrichtung(QDomElement e);
    static QString getGEO_Kante_ID_GEO_Knoten_A(QDomElement e);
    static QString getGEO_Kante_ID_GEO_Knoten_B(QDomElement e);
    static QString getGEO_Kante_ID_GEO_Art(QDomElement e);
    static double getGEO_Kante_GEO_Laenge(QDomElement e);
    static QString getGEO_Punkt_ID_GEO_Knoten(QDomElement e);
    static double getGEO_Punkt_GK_X(QDomElement e);
    static double getGEO_Punkt_GK_Y(QDomElement e);
    static QString getTOP_Knoten_ID_GEO_Knoten(QDomElement e);
    static QString getTOP_Kante_ID_TOP_Knoten_A(QDomElement e);
    static QString getTOP_Kante_ID_TOP_Knoten_B(QDomElement e);
    static QString getTOP_Kante_TOP_Anschluss_A(QDomElement e);
    static QString getTOP_Kante_TOP_Anschluss_B(QDomElement e);
    static double getTOP_Kante_TOP_Laenge(QDomElement e);
};

#endif // PARSER_H
