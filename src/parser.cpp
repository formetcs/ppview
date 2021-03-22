#include <QGraphicsScene>
#include <QGraphicsEllipseItem>

#include "parser.h"
#include "planpromodel.h"
#include "graphicsscene.h"

#include <iostream>


const double TRAVERSE_DELTA = 0.1; // to avoid problems with double precision

void Parser::createGraphicsScene(PlanProModel *model, GraphicsScene *scene)
{
    QDomElement containerElement = model->getContainerElement();
    QGraphicsScene* graphicsScene = scene->getGraphicsScene();

    graphicsScene->clear();

    QDomNode n = containerElement.firstChild();
    while (!n.isNull())
    {
        if (n.isElement())
        {
            QDomElement e = n.toElement();
            QString name = e.tagName();
            if(name == "GEO_Punkt")
            {
                QString id = getIdentitaet(e);
                QString tooltip = "GEO_Punkt\n";
                tooltip += id;
                double x = getGEO_Punkt_GK_X(e);
                double y = getGEO_Punkt_GK_Y(e);
                QGraphicsItem* item = (QGraphicsItem*) graphicsScene->addEllipse(x - 2, -y - 2, 4, 4, QPen(Qt::black), QBrush(Qt::white));
                item->setToolTip(tooltip);
                item->setData(GRAPHICSITEM_TYPE, "GEO_Punkt");
                item->setData(GRAPHICSITEM_ID, id);
                QString infotext = generateInfoText(e);
                item->setData(GRAPHICSITEM_INFOTEXT, infotext);
                item->setFlag(QGraphicsItem::ItemIsSelectable);
                item->setZValue(-3);
            }
            else if(name == "GEO_Knoten")
            {
                QString id = getIdentitaet(e);
                QString tooltip = "GEO_Knoten\n";
                tooltip += id;
                QDomNode n2 = containerElement.firstChild();
                while (!n2.isNull())
                {
                    if (n2.isElement())
                    {
                        QDomElement e2 = n2.toElement();
                        if(e2.tagName() == "GEO_Punkt")
                        {
                            if(getGEO_Punkt_ID_GEO_Knoten(e2) == id)
                            {
                                double px = getGEO_Punkt_GK_X(e2);
                                double py = getGEO_Punkt_GK_Y(e2);
                                QGraphicsItem* item = (QGraphicsItem*) graphicsScene->addEllipse(px - 2, -py - 2, 4, 4, QPen(Qt::black), QBrush(Qt::black));
                                item->setToolTip(tooltip);
                                item->setData(GRAPHICSITEM_TYPE, "GEO_Knoten");
                                item->setData(GRAPHICSITEM_ID, id);
                                QString infotext = generateInfoText(e);
                                item->setData(GRAPHICSITEM_INFOTEXT, infotext);
                                item->setFlag(QGraphicsItem::ItemIsSelectable);
                                item->setZValue(-2);
                                break;
                            }
                        }
                    }
                    n2 = n2.nextSibling();
                }
            }
            else if(name == "GEO_Kante")
            {
                QString id = getIdentitaet(e);
                QString tooltip = "GEO_Kante\n";
                tooltip += id;
                QString idknotena = getGEO_Kante_ID_GEO_Knoten_A(e);
                QString idknotenb = getGEO_Kante_ID_GEO_Knoten_B(e);
                QDomNode punktA = QDomNode();
                QDomNode punktB = QDomNode();
                QDomNode n2 = containerElement.firstChild();
                while (!n2.isNull() && (punktA.isNull() || punktB.isNull()))
                {
                    if (n2.isElement())
                    {
                        QDomElement e2 = n2.toElement();
                        if(e2.tagName() == "GEO_Punkt")
                        {
                            if(getGEO_Punkt_ID_GEO_Knoten(e2) == idknotena)
                            {
                                punktA = n2;
                            }
                            else if(getGEO_Punkt_ID_GEO_Knoten(e2) == idknotenb)
                            {
                                punktB = n2;
                            }
                        }
                    }
                    n2 = n2.nextSibling();
                }

                if(!punktA.isNull() && !punktB.isNull())
                {
                    double ax = getGEO_Punkt_GK_X(punktA.toElement());
                    double ay = getGEO_Punkt_GK_Y(punktA.toElement());
                    double bx = getGEO_Punkt_GK_X(punktB.toElement());
                    double by = getGEO_Punkt_GK_Y(punktB.toElement());

                    QGraphicsItem* item = (QGraphicsItem*) graphicsScene->addLine(ax, -ay, bx, -by);
                    item->setToolTip(tooltip);
                    item->setData(GRAPHICSITEM_TYPE, "GEO_Kante");
                    item->setData(GRAPHICSITEM_ID, id);
                    QString infotext = generateInfoText(e);
                    item->setData(GRAPHICSITEM_INFOTEXT, infotext);
                    item->setFlag(QGraphicsItem::ItemIsSelectable);
                    item->setZValue(-20);
                }
            }
            else if(name == "TOP_Knoten")
            {
                QString id = getIdentitaet(e);
                QString tooltip = "TOP_Knoten\n";
                tooltip += id;
                QString idknoten = getTOP_Knoten_ID_GEO_Knoten(e);
                QDomNode n2 = containerElement.firstChild();
                while (!n2.isNull())
                {
                    if (n2.isElement())
                    {
                        QDomElement e2 = n2.toElement();
                        if(e2.tagName() == "GEO_Punkt")
                        {
                            if(getGEO_Punkt_ID_GEO_Knoten(e2) == idknoten)
                            {
                                double px = getGEO_Punkt_GK_X(e2);
                                double py = getGEO_Punkt_GK_Y(e2);
                                QGraphicsItem* item = (QGraphicsItem*) graphicsScene->addEllipse(px - 2, -py - 2, 4, 4, QPen(Qt::red), QBrush(Qt::red));
                                item->setToolTip(tooltip);
                                item->setData(GRAPHICSITEM_TYPE, "TOP_Knoten");
                                item->setData(GRAPHICSITEM_ID, id);
                                QString infotext = generateInfoText(e);
                                item->setData(GRAPHICSITEM_INFOTEXT, infotext);
                                item->setFlag(QGraphicsItem::ItemIsSelectable);
                                item->setZValue(-1);
                                break;
                            }
                        }
                    }
                    n2 = n2.nextSibling();
                }
            }
            else if(name == "TOP_Kante")
            {
                QString id = getIdentitaet(e);
                QString tooltip = "TOP_Kante\n";
                tooltip += id;

                QList<QGraphicsItem*> graphicsitemlist;

                QDomNode n2 = containerElement.firstChild();
                while (!n2.isNull())
                {
                    if (n2.isElement())
                    {
                        QDomElement e2 = n2.toElement();
                        if(e2.tagName() == "GEO_Kante")
                        {
                            if(getGEO_Kante_ID_GEO_Art(e2) == id)
                            {
                                QString idknotena = getGEO_Kante_ID_GEO_Knoten_A(e2);
                                QString idknotenb = getGEO_Kante_ID_GEO_Knoten_B(e2);
                                QDomNode punktA = QDomNode();
                                QDomNode punktB = QDomNode();
                                QDomNode n3 = containerElement.firstChild();
                                while (!n3.isNull() && (punktA.isNull() || punktB.isNull()))
                                {
                                    if (n3.isElement())
                                    {
                                        QDomElement e3 = n3.toElement();
                                        if(e3.tagName() == "GEO_Punkt")
                                        {
                                            if(getGEO_Punkt_ID_GEO_Knoten(e3) == idknotena)
                                            {
                                                punktA = n3;
                                            }
                                            else if(getGEO_Punkt_ID_GEO_Knoten(e3) == idknotenb)
                                            {
                                                punktB = n3;
                                            }
                                        }
                                    }
                                    n3 = n3.nextSibling();
                                }

                                if(!punktA.isNull() && !punktB.isNull())
                                {
                                    double ax = getGEO_Punkt_GK_X(punktA.toElement());
                                    double ay = getGEO_Punkt_GK_Y(punktA.toElement());
                                    double bx = getGEO_Punkt_GK_X(punktB.toElement());
                                    double by = getGEO_Punkt_GK_Y(punktB.toElement());

                                    QGraphicsItem* item = (QGraphicsItem*) graphicsScene->addLine(ax, -ay, bx, -by, QPen(Qt::red));
                                    graphicsitemlist.append(item);
                                }

                            }
                        }
                    }
                    n2 = n2.nextSibling();
                }

                QGraphicsItem* groupitem = graphicsScene->createItemGroup(graphicsitemlist);
                groupitem->setToolTip(tooltip);
                groupitem->setData(GRAPHICSITEM_TYPE, "TOP_Kante");
                groupitem->setData(GRAPHICSITEM_ID, id);
                QString infotext = generateInfoText(e);
                groupitem->setData(GRAPHICSITEM_INFOTEXT, infotext);
                groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
                groupitem->setZValue(-10);


                /*QString id = getIdentitaet(e);
                QString idknotena = getTOP_Kante_ID_TOP_Knoten_A(e);
                QString idknotenb = getTOP_Kante_ID_TOP_Knoten_B(e);
                QDomElement topKnotenA = model->getObjectById(idknotena);
                QDomElement topKnotenB = model->getObjectById(idknotenb);
                QDomNode punktA = QDomNode();
                QDomNode punktB = QDomNode();
                QDomNode n2 = containerElement.firstChild();
                while (!n2.isNull() && (punktA.isNull() || punktB.isNull()))
                {
                    if (n2.isElement())
                    {
                        QDomElement e2 = n2.toElement();
                        if(e2.tagName() == "GEO_Punkt")
                        {
                            if(getGEO_Punkt_ID_GEO_Knoten(e2) == getTOP_Knoten_ID_GEO_Knoten(topKnotenA))
                            {
                                punktA = n2;
                            }
                            else if(getGEO_Punkt_ID_GEO_Knoten(e2) == getTOP_Knoten_ID_GEO_Knoten(topKnotenB))
                            {
                                punktB = n2;
                            }
                        }
                    }
                    n2 = n2.nextSibling();
                }

                if(!punktA.isNull() && !punktB.isNull())
                {
                    double ax = getGEO_Punkt_GK_X(punktA.toElement());
                    double ay = getGEO_Punkt_GK_Y(punktA.toElement());
                    double bx = getGEO_Punkt_GK_X(punktB.toElement());
                    double by = getGEO_Punkt_GK_Y(punktB.toElement());

                    QGraphicsItem* item = (QGraphicsItem*) graphicsScene->addLine(ax - xOffset, - (ay - yOffset), bx - xOffset, - (by - yOffset), QPen(Qt::red));
                    item->setData(1, "TOP_Kante");
                    item->setData(2, id);
                    QString infotext = generateInfoText(e);
                    item->setData(3, infotext);
                    item->setFlag(QGraphicsItem::ItemIsSelectable);
                }*/
            }
            else if(name == "Datenpunkt")
            {
                QString id = getIdentitaet(e);
                QDomElement datenpunktAllgemeinElement = e.firstChildElement("Datenpunkt_Allg");
                QDomElement datenpunktBeschreibungElement = datenpunktAllgemeinElement.firstChildElement("Datenpunkt_Beschreibung");
                QDomElement datenpunktBeschreibungWertElement = datenpunktBeschreibungElement.firstChildElement("Wert");
                QString datenpunktbeschreibungtext = datenpunktBeschreibungWertElement.text();
                QString km = e.firstChildElement("Punkt_Objekt_Strecke").firstChildElement("Strecke_Km").firstChildElement("Wert").text();
                QString tooltip = "Datenpunkt (Km: " + km + ")\n";
                tooltip += id + "\n";
                tooltip += datenpunktbeschreibungtext;

                QGraphicsEllipseItem* retval = (QGraphicsEllipseItem*) placePunktObjekt(e, model, scene);
                if(retval == NULL)
                {
                    std::cout << "Datenpunkt not placeable" << std::endl;
                }
                else
                {
                    retval->setToolTip(tooltip);
                    retval->setPen(QPen(Qt::black));
                    retval->setBrush(QBrush(Qt::yellow));
                    retval->setData(GRAPHICSITEM_TYPE, "Datenpunkt");
                    retval->setData(GRAPHICSITEM_ID, id);
                    QString infotext = generateInfoText(e);
                    retval->setData(GRAPHICSITEM_INFOTEXT, infotext);
                    retval->setFlag(QGraphicsItem::ItemIsSelectable);
                }
            }
            else if(name == "ZUB_Bereichsgrenze")
            {
                QString id = getIdentitaet(e);
                QString km = e.firstChildElement("Punkt_Objekt_Strecke").firstChildElement("Strecke_Km").firstChildElement("Wert").text();
                QString tooltip = "ZUB_Bereichsgrenze (Km: " + km + ")\n";
                tooltip += id;

                QGraphicsEllipseItem* retval = (QGraphicsEllipseItem*) placePunktObjekt(e, model, scene);
                if(retval == NULL)
                {
                    std::cout << "ZUB_Bereichsgrenze not placeable" << std::endl;
                }
                else
                {
                    retval->setToolTip(tooltip);
                    retval->setPen(QPen(Qt::red));
                    retval->setBrush(QBrush(Qt::yellow));
                    retval->setData(GRAPHICSITEM_TYPE, "ZUB_Bereichsgrenze");
                    retval->setData(GRAPHICSITEM_ID, id);
                    QString infotext = generateInfoText(e);
                    retval->setData(GRAPHICSITEM_INFOTEXT, infotext);
                    retval->setFlag(QGraphicsItem::ItemIsSelectable);
                }
            }
            else if(name == "Bahnsteig_Zugang")
            {
                QString id = getIdentitaet(e);
                QString km = e.firstChildElement("Punkt_Objekt_Strecke").firstChildElement("Strecke_Km").firstChildElement("Wert").text();
                QString tooltip = "Bahnsteig_Zugang (Km: " + km + ")\n";
                tooltip += id;

                QGraphicsEllipseItem* retval = (QGraphicsEllipseItem*) placePunktObjekt(e, model, scene);
                if(retval == NULL)
                {
                    std::cout << "Bahnsteig_Zugang not placeable" << std::endl;
                }
                else
                {
                    retval->setToolTip(tooltip);
                    retval->setPen(QPen(Qt::cyan));
                    retval->setBrush(QBrush(Qt::cyan));
                    retval->setData(GRAPHICSITEM_TYPE, "Bahnsteig_Zugang");
                    retval->setData(GRAPHICSITEM_ID, id);
                    QString infotext = generateInfoText(e);
                    retval->setData(GRAPHICSITEM_INFOTEXT, infotext);
                    retval->setFlag(QGraphicsItem::ItemIsSelectable);
                }
            }
            else if(name == "BUE_Anlage")
            {
                QString id = getIdentitaet(e);
                QDomElement bezeichnungElement = e.firstChildElement("Bezeichnung");
                QDomElement bezeichnungLageplanKurzElement = bezeichnungElement.firstChildElement("Bezeichnung_Lageplan_Kurz");
                QDomElement bezeichnungLageplanKurzWertElement = bezeichnungLageplanKurzElement.firstChildElement("Wert");
                QString bezeichnunglageplankurztext = bezeichnungLageplanKurzWertElement.text();
                QString km = e.firstChildElement("Punkt_Objekt_Strecke").firstChildElement("Strecke_Km").firstChildElement("Wert").text();
                QString tooltip = "BUE_Anlage (Km: " + km + ")\n";
                tooltip += id + "\n";
                tooltip += bezeichnunglageplankurztext;

                QGraphicsEllipseItem* retval = (QGraphicsEllipseItem*) placePunktObjekt(e, model, scene);
                if(retval == NULL)
                {
                    std::cout << "BUE_Anlage not placeable" << std::endl;
                }
                else
                {
                    retval->setToolTip(tooltip);
                    retval->setPen(QPen(Qt::blue));
                    retval->setBrush(QBrush(Qt::blue));
                    retval->setData(GRAPHICSITEM_TYPE, "BUE_Anlage");
                    retval->setData(GRAPHICSITEM_ID, id);
                    QString infotext = generateInfoText(e);
                    retval->setData(GRAPHICSITEM_INFOTEXT, infotext);
                    retval->setFlag(QGraphicsItem::ItemIsSelectable);
                }
            }
            else if(name == "Sonstiger_Punkt")
            {
                QString id = getIdentitaet(e);
                QString km = e.firstChildElement("Punkt_Objekt_Strecke").firstChildElement("Strecke_Km").firstChildElement("Wert").text();
                QString tooltip = "Sonstiger_Punkt (Km: " + km + ")\n";
                tooltip += id;

                QGraphicsEllipseItem* retval = (QGraphicsEllipseItem*) placePunktObjekt(e, model, scene);
                if(retval == NULL)
                {
                    std::cout << "Sonstiger_Punkt not placeable" << std::endl;
                }
                else
                {
                    retval->setToolTip(tooltip);
                    retval->setPen(QPen(Qt::magenta));
                    retval->setBrush(QBrush(Qt::magenta));
                    retval->setData(GRAPHICSITEM_TYPE, "Sonstiger_Punkt");
                    retval->setData(GRAPHICSITEM_ID, id);
                    QString infotext = generateInfoText(e);
                    retval->setData(GRAPHICSITEM_INFOTEXT, infotext);
                    retval->setFlag(QGraphicsItem::ItemIsSelectable);
                }
            }
            else if(name == "Hoehenpunkt")
            {
                QString id = getIdentitaet(e);
                QString km = e.firstChildElement("Punkt_Objekt_Strecke").firstChildElement("Strecke_Km").firstChildElement("Wert").text();
                QString tooltip = "Hoehenpunkt (Km: " + km + ")\n";
                tooltip += id;

                QGraphicsEllipseItem* retval = (QGraphicsEllipseItem*) placePunktObjekt(e, model, scene);
                if(retval == NULL)
                {
                    std::cout << "Hoehenpunkt not placeable" << std::endl;
                }
                else
                {
                    retval->setToolTip(tooltip);
                    retval->setPen(QPen(Qt::black));
                    retval->setBrush(QBrush(Qt::red));
                    retval->setData(GRAPHICSITEM_TYPE, "Hoehenpunkt");
                    retval->setData(GRAPHICSITEM_ID, id);
                    QString infotext = generateInfoText(e);
                    retval->setData(GRAPHICSITEM_INFOTEXT, infotext);
                    retval->setFlag(QGraphicsItem::ItemIsSelectable);
                }
            }
            else if(name == "Technischer_Punkt")
            {
                QString id = getIdentitaet(e);
                QString km = e.firstChildElement("Punkt_Objekt_Strecke").firstChildElement("Strecke_Km").firstChildElement("Wert").text();
                QString tooltip = "Technischer_Punkt (Km: " + km + ")\n";
                tooltip += id;

                QGraphicsEllipseItem* retval = (QGraphicsEllipseItem*) placePunktObjekt(e, model, scene);
                if(retval == NULL)
                {
                    std::cout << "Technischer_Punkt not placeable" << std::endl;
                }
                else
                {
                    retval->setToolTip(tooltip);
                    retval->setPen(QPen(Qt::black));
                    retval->setBrush(QBrush(Qt::red));
                    retval->setData(GRAPHICSITEM_TYPE, "Technischer_Punkt");
                    retval->setData(GRAPHICSITEM_ID, id);
                    QString infotext = generateInfoText(e);
                    retval->setData(GRAPHICSITEM_INFOTEXT, infotext);
                    retval->setFlag(QGraphicsItem::ItemIsSelectable);
                }
            }
            else if(name == "Ueberhoehung")
            {
                QString id = getIdentitaet(e);
                QString km = e.firstChildElement("Punkt_Objekt_Strecke").firstChildElement("Strecke_Km").firstChildElement("Wert").text();
                QString tooltip = "Ueberhoehung (Km: " + km + ")\n";
                tooltip += id;

                QGraphicsEllipseItem* retval = (QGraphicsEllipseItem*) placePunktObjekt(e, model, scene);
                if(retval == NULL)
                {
                    std::cout << "Ueberhoehung not placeable" << std::endl;
                }
                else
                {
                    retval->setToolTip(tooltip);
                    retval->setPen(QPen(Qt::black));
                    retval->setBrush(QBrush(Qt::red));
                    retval->setData(GRAPHICSITEM_TYPE, "Ueberhoehung");
                    retval->setData(GRAPHICSITEM_ID, id);
                    QString infotext = generateInfoText(e);
                    retval->setData(GRAPHICSITEM_INFOTEXT, infotext);
                    retval->setFlag(QGraphicsItem::ItemIsSelectable);
                }
            }
            else if(name == "Gleis_Abschluss")
            {
                QString id = getIdentitaet(e);
                QString km = e.firstChildElement("Punkt_Objekt_Strecke").firstChildElement("Strecke_Km").firstChildElement("Wert").text();
                QString tooltip = "Gleis_Abschluss (Km: " + km + ")\n";
                tooltip += id;

                QGraphicsEllipseItem* retval = (QGraphicsEllipseItem*) placePunktObjekt(e, model, scene);
                if(retval == NULL)
                {
                    std::cout << "Gleis_Abschluss not placeable" << std::endl;
                }
                else
                {
                    retval->setToolTip(tooltip);
                    retval->setPen(QPen(Qt::black));
                    retval->setBrush(QBrush("olive"));
                    retval->setData(GRAPHICSITEM_TYPE, "Gleis_Abschluss");
                    retval->setData(GRAPHICSITEM_ID, id);
                    QString infotext = generateInfoText(e);
                    retval->setData(GRAPHICSITEM_INFOTEXT, infotext);
                    retval->setFlag(QGraphicsItem::ItemIsSelectable);
                }
            }
            else if(name == "NB_Zone_Grenze")
            {
                QString id = getIdentitaet(e);
                QString km = e.firstChildElement("Punkt_Objekt_Strecke").firstChildElement("Strecke_Km").firstChildElement("Wert").text();
                QString tooltip = "NB_Zone_Grenze (Km: " + km + ")\n";
                tooltip += id;

                QGraphicsEllipseItem* retval = (QGraphicsEllipseItem*) placePunktObjekt(e, model, scene);
                if(retval == NULL)
                {
                    std::cout << "NB_Zone_Grenze not placeable" << std::endl;
                }
                else
                {
                    retval->setToolTip(tooltip);
                    retval->setPen(QPen(Qt::black));
                    retval->setBrush(QBrush("lightsteelblue"));
                    retval->setData(GRAPHICSITEM_TYPE, "NB_Zone_Grenze");
                    retval->setData(GRAPHICSITEM_ID, id);
                    QString infotext = generateInfoText(e);
                    retval->setData(GRAPHICSITEM_INFOTEXT, infotext);
                    retval->setFlag(QGraphicsItem::ItemIsSelectable);
                }
            }
            else if(name == "FMA_Element")
            {
                QString id = getIdentitaet(e);
                QString km = e.firstChildElement("Punkt_Objekt_Strecke").firstChildElement("Strecke_Km").firstChildElement("Wert").text();
                QString tooltip = "FMA_Element (Km: " + km + ")\n";
                tooltip += id;

                QGraphicsEllipseItem* retval = (QGraphicsEllipseItem*) placePunktObjekt(e, model, scene);
                if(retval == NULL)
                {
                    std::cout << "FMA_Element not placeable" << std::endl;
                }
                else
                {
                    retval->setToolTip(tooltip);
                    retval->setPen(QPen("brown"));
                    retval->setBrush(QBrush("brown"));
                    retval->setData(GRAPHICSITEM_TYPE, "FMA_Element");
                    retval->setData(GRAPHICSITEM_ID, id);
                    QString infotext = generateInfoText(e);
                    retval->setData(GRAPHICSITEM_INFOTEXT, infotext);
                    retval->setFlag(QGraphicsItem::ItemIsSelectable);
                }
            }
            else if(name == "FMA_Komponente")
            {
                QString id = getIdentitaet(e);
                QDomElement bezeichnungElement = e.firstChildElement("Bezeichnung");
                QDomElement bezeichnungLageplanKurzElement = bezeichnungElement.firstChildElement("Bezeichnung_Lageplan_Kurz");
                QDomElement bezeichnungLageplanKurzWertElement = bezeichnungLageplanKurzElement.firstChildElement("Wert");
                QString bezeichnunglageplankurztext = bezeichnungLageplanKurzWertElement.text();
                QString km = e.firstChildElement("Punkt_Objekt_Strecke").firstChildElement("Strecke_Km").firstChildElement("Wert").text();
                QString tooltip = "FMA_Komponente (Km: " + km + ")\n";
                tooltip += id + "\n";
                tooltip += bezeichnunglageplankurztext;

                QGraphicsEllipseItem* retval = (QGraphicsEllipseItem*) placePunktObjekt(e, model, scene);
                if(retval == NULL)
                {
                    std::cout << "FMA_Komponente not placeable" << std::endl;
                }
                else
                {
                    retval->setToolTip(tooltip);
                    retval->setPen(QPen("brown"));
                    retval->setBrush(QBrush("brown"));
                    retval->setData(GRAPHICSITEM_TYPE, "FMA_Komponente");
                    retval->setData(GRAPHICSITEM_ID, id);
                    QString infotext = generateInfoText(e);
                    retval->setData(GRAPHICSITEM_INFOTEXT, infotext);
                    retval->setFlag(QGraphicsItem::ItemIsSelectable);
                }
            }
            else if(name == "Zugeinwirkung")
            {
                QString id = getIdentitaet(e);
                QDomElement bezeichnungElement = e.firstChildElement("Bezeichnung");
                QDomElement bezeichnungLageplanKurzElement = bezeichnungElement.firstChildElement("Bezeichnung_Lageplan_Kurz");
                QDomElement bezeichnungLageplanKurzWertElement = bezeichnungLageplanKurzElement.firstChildElement("Wert");
                QString bezeichnunglageplankurztext = bezeichnungLageplanKurzWertElement.text();
                QString km = e.firstChildElement("Punkt_Objekt_Strecke").firstChildElement("Strecke_Km").firstChildElement("Wert").text();
                QString tooltip = "Zugeinwirkung (Km: " + km + ")\n";
                tooltip += id + "\n";
                tooltip += bezeichnunglageplankurztext;

                QGraphicsEllipseItem* retval = (QGraphicsEllipseItem*) placePunktObjekt(e, model, scene);
                if(retval == NULL)
                {
                    std::cout << "Zugeinwirkung not placeable" << std::endl;
                }
                else
                {
                    retval->setToolTip(tooltip);
                    retval->setPen(QPen("brown"));
                    retval->setBrush(QBrush("brown"));
                    retval->setData(GRAPHICSITEM_TYPE, "Zugeinwirkung");
                    retval->setData(GRAPHICSITEM_ID, id);
                    QString infotext = generateInfoText(e);
                    retval->setData(GRAPHICSITEM_INFOTEXT, infotext);
                    retval->setFlag(QGraphicsItem::ItemIsSelectable);
                }
            }
            else if(name == "PZB_Element")
            {
                QString id = getIdentitaet(e);
                QString km = e.firstChildElement("Punkt_Objekt_Strecke").firstChildElement("Strecke_Km").firstChildElement("Wert").text();
                QString tooltip = "PZB_Element (Km: " + km + ")\n";
                tooltip += id;

                QGraphicsEllipseItem* retval = (QGraphicsEllipseItem*) placePunktObjekt(e, model, scene);
                if(retval == NULL)
                {
                    std::cout << "PZB_Element not placeable" << std::endl;
                }
                else
                {
                    retval->setToolTip(tooltip);
                    retval->setPen(QPen(Qt::gray));
                    retval->setBrush(QBrush(Qt::gray));
                    retval->setData(GRAPHICSITEM_TYPE, "PZB_Element");
                    retval->setData(GRAPHICSITEM_ID, id);
                    QString infotext = generateInfoText(e);
                    retval->setData(GRAPHICSITEM_INFOTEXT, infotext);
                    retval->setFlag(QGraphicsItem::ItemIsSelectable);
                }
            }
            else if(name == "Signal")
            {
                QString id = getIdentitaet(e);
                QDomElement bezeichnungElement = e.firstChildElement("Bezeichnung");
                QDomElement bezeichnungLageplanKurzElement = bezeichnungElement.firstChildElement("Bezeichnung_Lageplan_Kurz");
                QDomElement bezeichnungLageplanKurzWertElement = bezeichnungLageplanKurzElement.firstChildElement("Wert");
                QString bezeichnunglageplankurztext = bezeichnungLageplanKurzWertElement.text();
                QString km = e.firstChildElement("Punkt_Objekt_Strecke").firstChildElement("Strecke_Km").firstChildElement("Wert").text();
                QString tooltip = "Signal (Km: " + km + ")\n";
                tooltip += id + "\n";
                tooltip += bezeichnunglageplankurztext;


                QGraphicsEllipseItem* retval = (QGraphicsEllipseItem*) placePunktObjekt(e, model, scene);
                if(retval == NULL)
                {
                    std::cout << "Signal not placeable" << std::endl;
                }
                else
                {
                    retval->setToolTip(tooltip);
                    retval->setPen(QPen(Qt::green));
                    retval->setBrush(QBrush(Qt::green));
                    retval->setData(GRAPHICSITEM_TYPE, "Signal");
                    retval->setData(GRAPHICSITEM_ID, id);
                    QString infotext = generateInfoText(e);
                    retval->setData(GRAPHICSITEM_INFOTEXT, infotext);
                    retval->setFlag(QGraphicsItem::ItemIsSelectable);
                }
            }
            else if(name == "Signal_Befestigung")
            {
                QString id = getIdentitaet(e);
                QString km = e.firstChildElement("Punkt_Objekt_Strecke").firstChildElement("Strecke_Km").firstChildElement("Wert").text();
                QString tooltip = "Signal_Befestigung (Km: " + km + ")\n";
                tooltip += id;

                QGraphicsEllipseItem* retval = (QGraphicsEllipseItem*) placePunktObjekt(e, model, scene);
                if(retval == NULL)
                {
                    std::cout << "Signal_Befestigung not placeable" << std::endl;
                }
                else
                {
                    retval->setToolTip(tooltip);
                    retval->setPen(QPen(Qt::black));
                    retval->setBrush(QBrush("greenyellow"));
                    retval->setData(GRAPHICSITEM_TYPE, "Signal_Befestigung");
                    retval->setData(GRAPHICSITEM_ID, id);
                    QString infotext = generateInfoText(e);
                    retval->setData(GRAPHICSITEM_INFOTEXT, infotext);
                    retval->setFlag(QGraphicsItem::ItemIsSelectable);
                }
            }
            else if(name == "W_Kr_Gsp_Komponente")
            {
                QString id = getIdentitaet(e);
                QString km = e.firstChildElement("Punkt_Objekt_Strecke").firstChildElement("Strecke_Km").firstChildElement("Wert").text();
                QString tooltip = "W_Kr_Gsp_Komponente (Km: " + km + ")\n";
                tooltip += id;

                QGraphicsEllipseItem* retval = (QGraphicsEllipseItem*) placePunktObjekt(e, model, scene);
                if(retval == NULL)
                {
                    std::cout << "W_Kr_Gsp_Komponente not placeable" << std::endl;
                }
                else
                {
                    retval->setToolTip(tooltip);
                    retval->setPen(QPen("coral"));
                    retval->setBrush(QBrush("coral"));
                    retval->setData(GRAPHICSITEM_TYPE, "W_Kr_Gsp_Komponente");
                    retval->setData(GRAPHICSITEM_ID, id);
                    QString infotext = generateInfoText(e);
                    retval->setData(GRAPHICSITEM_INFOTEXT, infotext);
                    retval->setFlag(QGraphicsItem::ItemIsSelectable);
                }
            }
            else if(name == "ZUB_Streckeneigenschaft")
            {
                QString id = getIdentitaet(e);
                QString tooltip = "ZUB_Streckeneigenschaft\n";
                tooltip += id;

                QGraphicsItem* retval = placeBereichObjekt(e, model, scene, QPen(Qt::yellow));
                if(retval == NULL)
                {
                    std::cout << "ZUB_Streckeneigenschaft not placeable" << std::endl;
                }
                else
                {
                    retval->setToolTip(tooltip);
                    retval->setData(GRAPHICSITEM_TYPE, "ZUB_Streckeneigenschaft");
                    retval->setData(GRAPHICSITEM_ID, id);
                    QString infotext = generateInfoText(e);
                    retval->setData(GRAPHICSITEM_INFOTEXT, infotext);
                    retval->setFlag(QGraphicsItem::ItemIsSelectable);
                    retval->setZValue(-5);
                }
            }
            else if(name == "Stell_Bereich")
            {
                QString id = getIdentitaet(e);
                QString tooltip = "Stell_Bereich\n";
                tooltip += id;

                QGraphicsItem* retval = placeBereichObjekt(e, model, scene, QPen("orange"));
                if(retval == NULL)
                {
                    std::cout << "Stell_Bereich not placeable" << std::endl;
                }
                else
                {
                    retval->setToolTip(tooltip);
                    retval->setData(GRAPHICSITEM_TYPE, "Stell_Bereich");
                    retval->setData(GRAPHICSITEM_ID, id);
                    QString infotext = generateInfoText(e);
                    retval->setData(GRAPHICSITEM_INFOTEXT, infotext);
                    retval->setFlag(QGraphicsItem::ItemIsSelectable);
                    retval->setZValue(-5);
                }
            }
            else if(name == "Bahnsteig_Kante")
            {
                QString id = getIdentitaet(e);
                QString tooltip = "Bahnsteig_Kante\n";
                tooltip += id;

                QGraphicsItem* retval = placeBereichObjekt(e, model, scene, QPen(Qt::cyan));
                if(retval == NULL)
                {
                    std::cout << "Bahnsteig_Kante not placeable" << std::endl;
                }
                else
                {
                    retval->setToolTip(tooltip);
                    retval->setData(GRAPHICSITEM_TYPE, "Bahnsteig_Kante");
                    retval->setData(GRAPHICSITEM_ID, id);
                    QString infotext = generateInfoText(e);
                    retval->setData(GRAPHICSITEM_INFOTEXT, infotext);
                    retval->setFlag(QGraphicsItem::ItemIsSelectable);
                    retval->setZValue(-5);
                }
            }
            else if(name == "BUE_Gleisbezogener_Gefahrraum")
            {
                QString id = getIdentitaet(e);
                QString tooltip = "BUE_Gleisbezogener_Gefahrraum\n";
                tooltip += id;

                QGraphicsItem* retval = placeBereichObjekt(e, model, scene, QPen(Qt::blue));
                if(retval == NULL)
                {
                    std::cout << "BUE_Gleisbezogener_Gefahrraum not placeable" << std::endl;
                }
                else
                {
                    retval->setToolTip(tooltip);
                    retval->setData(GRAPHICSITEM_TYPE, "BUE_Gleisbezogener_Gefahrraum");
                    retval->setData(GRAPHICSITEM_ID, id);
                    QString infotext = generateInfoText(e);
                    retval->setData(GRAPHICSITEM_INFOTEXT, infotext);
                    retval->setFlag(QGraphicsItem::ItemIsSelectable);
                    retval->setZValue(-5);
                }
            }
            else if(name == "Fstr_Fahrweg")
            {
                QString id = getIdentitaet(e);
                QString tooltip = "Fstr_Fahrweg\n";
                tooltip += id;

                QGraphicsItem* retval = placeBereichObjekt(e, model, scene, QPen(Qt::magenta));
                if(retval == NULL)
                {
                    std::cout << "Fstr_Fahrweg not placeable" << std::endl;
                }
                else
                {
                    retval->setToolTip(tooltip);
                    retval->setData(GRAPHICSITEM_TYPE, "Fstr_Fahrweg");
                    retval->setData(GRAPHICSITEM_ID, id);
                    QString infotext = generateInfoText(e);
                    retval->setData(GRAPHICSITEM_INFOTEXT, infotext);
                    retval->setFlag(QGraphicsItem::ItemIsSelectable);
                    retval->setZValue(-5);
                }
            }
            else if(name == "Geschwindigkeitsprofil")
            {
                QString id = getIdentitaet(e);
                QString tooltip = "Geschwindigkeitsprofil\n";
                tooltip += id;

                QGraphicsItem* retval = placeBereichObjekt(e, model, scene, QPen(Qt::red));
                if(retval == NULL)
                {
                    std::cout << "Geschwindigkeitsprofil not placeable" << std::endl;
                }
                else
                {
                    retval->setToolTip(tooltip);
                    retval->setData(GRAPHICSITEM_TYPE, "Geschwindigkeitsprofil");
                    retval->setData(GRAPHICSITEM_ID, id);
                    QString infotext = generateInfoText(e);
                    retval->setData(GRAPHICSITEM_INFOTEXT, infotext);
                    retval->setFlag(QGraphicsItem::ItemIsSelectable);
                    retval->setZValue(-5);
                }
            }
            else if(name == "Strecke")
            {
                QString id = getIdentitaet(e);
                QString tooltip = "Strecke\n";
                tooltip += id;

                QGraphicsItem* retval = placeBereichObjekt(e, model, scene, QPen(Qt::red));
                if(retval == NULL)
                {
                    std::cout << "Strecke not placeable" << std::endl;
                }
                else
                {
                    retval->setToolTip(tooltip);
                    retval->setData(GRAPHICSITEM_TYPE, "Strecke");
                    retval->setData(GRAPHICSITEM_ID, id);
                    QString infotext = generateInfoText(e);
                    retval->setData(GRAPHICSITEM_INFOTEXT, infotext);
                    retval->setFlag(QGraphicsItem::ItemIsSelectable);
                    retval->setZValue(-5);
                }
            }
            else if(name == "Technischer_Bereich")
            {
                QString id = getIdentitaet(e);
                QString tooltip = "Technischer_Bereich\n";
                tooltip += id;

                QGraphicsItem* retval = placeBereichObjekt(e, model, scene, QPen(Qt::red));
                if(retval == NULL)
                {
                    std::cout << "Technischer_Bereich not placeable" << std::endl;
                }
                else
                {
                    retval->setToolTip(tooltip);
                    retval->setData(GRAPHICSITEM_TYPE, "Technischer_Bereich");
                    retval->setData(GRAPHICSITEM_ID, id);
                    QString infotext = generateInfoText(e);
                    retval->setData(GRAPHICSITEM_INFOTEXT, infotext);
                    retval->setFlag(QGraphicsItem::ItemIsSelectable);
                    retval->setZValue(-5);
                }
            }
            else if(name == "Gleis_Abschnitt")
            {
                QString id = getIdentitaet(e);
                QString tooltip = "Gleis_Abschnitt\n";
                tooltip += id;

                QGraphicsItem* retval = placeBereichObjekt(e, model, scene, QPen("olive"));
                if(retval == NULL)
                {
                    std::cout << "Gleis_Abschnitt not placeable" << std::endl;
                }
                else
                {
                    retval->setToolTip(tooltip);
                    retval->setData(GRAPHICSITEM_TYPE, "Gleis_Abschnitt");
                    retval->setData(GRAPHICSITEM_ID, id);
                    QString infotext = generateInfoText(e);
                    retval->setData(GRAPHICSITEM_INFOTEXT, infotext);
                    retval->setFlag(QGraphicsItem::ItemIsSelectable);
                    retval->setZValue(-5);
                }
            }
            else if(name == "Gleis_Art")
            {
                QString id = getIdentitaet(e);
                QString tooltip = "Gleis_Art\n";
                tooltip += id;

                QGraphicsItem* retval = placeBereichObjekt(e, model, scene, QPen("olive"));
                if(retval == NULL)
                {
                    std::cout << "Gleis_Art not placeable" << std::endl;
                }
                else
                {
                    retval->setToolTip(tooltip);
                    retval->setData(GRAPHICSITEM_TYPE, "Gleis_Art");
                    retval->setData(GRAPHICSITEM_ID, id);
                    QString infotext = generateInfoText(e);
                    retval->setData(GRAPHICSITEM_INFOTEXT, infotext);
                    retval->setFlag(QGraphicsItem::ItemIsSelectable);
                    retval->setZValue(-5);
                }
            }
            else if(name == "Gleis_Baubereich")
            {
                QString id = getIdentitaet(e);
                QString tooltip = "Gleis_Baubereich\n";
                tooltip += id;

                QGraphicsItem* retval = placeBereichObjekt(e, model, scene, QPen("olive"));
                if(retval == NULL)
                {
                    std::cout << "Gleis_Baubereich not placeable" << std::endl;
                }
                else
                {
                    retval->setToolTip(tooltip);
                    retval->setData(GRAPHICSITEM_TYPE, "Gleis_Baubereich");
                    retval->setData(GRAPHICSITEM_ID, id);
                    QString infotext = generateInfoText(e);
                    retval->setData(GRAPHICSITEM_INFOTEXT, infotext);
                    retval->setFlag(QGraphicsItem::ItemIsSelectable);
                    retval->setZValue(-5);
                }
            }
            else if(name == "Gleis_Bezeichnung")
            {
                QString id = getIdentitaet(e);
                QString tooltip = "Gleis_Bezeichnung\n";
                tooltip += id;

                QGraphicsItem* retval = placeBereichObjekt(e, model, scene, QPen("olive"));
                if(retval == NULL)
                {
                    std::cout << "Gleis_Bezeichnung not placeable" << std::endl;
                }
                else
                {
                    retval->setToolTip(tooltip);
                    retval->setData(GRAPHICSITEM_TYPE, "Gleis_Bezeichnung");
                    retval->setData(GRAPHICSITEM_ID, id);
                    QString infotext = generateInfoText(e);
                    retval->setData(GRAPHICSITEM_INFOTEXT, infotext);
                    retval->setFlag(QGraphicsItem::ItemIsSelectable);
                    retval->setZValue(-5);
                }
            }
            else if(name == "Gleis_Fahrbahn")
            {
                QString id = getIdentitaet(e);
                QString tooltip = "Gleis_Fahrbahn\n";
                tooltip += id;

                QGraphicsItem* retval = placeBereichObjekt(e, model, scene, QPen("olive"));
                if(retval == NULL)
                {
                    std::cout << "Gleis_Fahrbahn not placeable" << std::endl;
                }
                else
                {
                    retval->setToolTip(tooltip);
                    retval->setData(GRAPHICSITEM_TYPE, "Gleis_Fahrbahn");
                    retval->setData(GRAPHICSITEM_ID, id);
                    QString infotext = generateInfoText(e);
                    retval->setData(GRAPHICSITEM_INFOTEXT, infotext);
                    retval->setFlag(QGraphicsItem::ItemIsSelectable);
                    retval->setZValue(-5);
                }
            }
            else if(name == "Gleis_Lichtraum")
            {
                QString id = getIdentitaet(e);
                QString tooltip = "Gleis_Lichtraum\n";
                tooltip += id;

                QGraphicsItem* retval = placeBereichObjekt(e, model, scene, QPen("olive"));
                if(retval == NULL)
                {
                    std::cout << "Gleis_Lichtraum not placeable" << std::endl;
                }
                else
                {
                    retval->setToolTip(tooltip);
                    retval->setData(GRAPHICSITEM_TYPE, "Gleis_Lichtraum");
                    retval->setData(GRAPHICSITEM_ID, id);
                    QString infotext = generateInfoText(e);
                    retval->setData(GRAPHICSITEM_INFOTEXT, infotext);
                    retval->setFlag(QGraphicsItem::ItemIsSelectable);
                    retval->setZValue(-5);
                }
            }
            else if(name == "Gleis_Schaltgruppe")
            {
                QString id = getIdentitaet(e);
                QString tooltip = "Gleis_Schaltgruppe\n";
                tooltip += id;

                QGraphicsItem* retval = placeBereichObjekt(e, model, scene, QPen("olive"));
                if(retval == NULL)
                {
                    std::cout << "Gleis_Schaltgruppe not placeable" << std::endl;
                }
                else
                {
                    retval->setToolTip(tooltip);
                    retval->setData(GRAPHICSITEM_TYPE, "Gleis_Schaltgruppe");
                    retval->setData(GRAPHICSITEM_ID, id);
                    QString infotext = generateInfoText(e);
                    retval->setData(GRAPHICSITEM_INFOTEXT, infotext);
                    retval->setFlag(QGraphicsItem::ItemIsSelectable);
                    retval->setZValue(-5);
                }
            }
        }
        n = n.nextSibling();
    }
}


int Parser::getMaxDomDepth(QDomNode o)
{
    if(!o.hasChildNodes())
    {
        return 0;
    }

    QDomNodeList childnodelist = o.childNodes();
    int maxdepth = 0;
    for(int i = 0; i < childnodelist.count(); i++)
    {
        int tempdepth = getMaxDomDepth(childnodelist.at(i));
        if(tempdepth > maxdepth)
            maxdepth = tempdepth;
    }

    return maxdepth + 1;
}

int Parser::getLeafCount(QDomNode o)
{
    if(!o.hasChildNodes())
    {
        return 1;
    }

    QDomNodeList childnodelist = o.childNodes();
    int tempcount = 0;
    for(int i = 0; i < childnodelist.count(); i++)
    {
        tempcount += getLeafCount(childnodelist.at(i));
    }

    return tempcount;
}

QString Parser::generateInfoText(QDomNode o)
{
    int maxdepth = getMaxDomDepth(o);
    QString returnval = QString("<table border='1' cellpadding='2'>\n");
    returnval += QString("<tr><th bgcolor='deepskyblue' colspan ='%1'>%2</th></tr>\n<tr>").arg(maxdepth - 1) .arg(o.nodeName());
    returnval += generateInfoTextRec(o, 1, maxdepth);
    returnval = returnval.left(returnval.size() - 4);
    returnval += QString("</table>\n");
    return returnval;
}

QString Parser::generateInfoTextRec(QDomNode o, int currentDepth, int maxDepth)
{
    QString returnval = QString();
    if(!o.hasChildNodes())
    {
        returnval += QString("<td colspan ='%1'></td></tr>\n<tr>") .arg(maxDepth - currentDepth);
        return returnval;
    }
    QDomElement wertelement = o.firstChildElement("Wert");
    if(!wertelement.isNull())
    {
        returnval += QString("<td>%1</td></tr>\n<tr>").arg(wertelement.text());
        return returnval;
    }
    QDomNodeList childnodelist = o.childNodes();
    for(int i = 0; i < childnodelist.count(); i++)
    {
        QDomNode childnode = childnodelist.at(i);
        int leafcount = getLeafCount(childnode);
        int colspan = 1;
        if(!childnode.firstChildElement("Wert").isNull())
        {
            colspan = maxDepth - currentDepth - 1;
        }
        returnval += QString("<td bgcolor='lightskyblue' rowspan ='%1' colspan ='%2'><b>%3</b></td>").arg(leafcount) .arg(colspan) .arg(childnode.nodeName());
        returnval += generateInfoTextRec(childnode, currentDepth + 1, maxDepth);
    }

    return returnval;
}

QGraphicsItem* Parser::placePunktObjekt(QDomElement o, PlanProModel *model, GraphicsScene *scene)
{
    QDomElement containerElement = model->getContainerElement();
    QGraphicsScene* graphicsScene = scene->getGraphicsScene();
    double remainingDistance = getPunkt_Objekt_Abstand(o);

    QDomElement topKante = model->getObjectById(getPunkt_Objekt_ID_TOP_Kante(o));
    QDomElement topKnotenA = model->getObjectById(getTOP_Kante_ID_TOP_Knoten_A(topKante));

    QDomNode currentGeoKante = QDomNode();
    bool traverseForward = true;

    // find the starting GEO_Kante
    QDomNode n = containerElement.firstChild();
    while (!n.isNull())
    {
        if (n.isElement())
        {
            QDomElement e = n.toElement();
            if(e.tagName() == "GEO_Kante")
            {
                if((getGEO_Kante_ID_GEO_Knoten_A(e) == getTOP_Knoten_ID_GEO_Knoten(topKnotenA)) && (getGEO_Kante_ID_GEO_Art(e) == getIdentitaet(topKante)))
                {
                    currentGeoKante = n;
                    traverseForward = true;
                    break;
                }
                else if((getGEO_Kante_ID_GEO_Knoten_B(e) == getTOP_Knoten_ID_GEO_Knoten(topKnotenA)) && (getGEO_Kante_ID_GEO_Art(e) == getIdentitaet(topKante)))
                {
                    currentGeoKante = n;
                    traverseForward = false;
                    break;
                }
            }
        }
        n = n.nextSibling();
    }

    if(currentGeoKante.isNull())
    {
        return NULL;
    }

    // traverse along the GEO_Kante chain until the remaining distance has reached zero (with delta value due to double precision)
    // (afterwards, currentGeoKante will point to the GEO_Kante where the Punkt_Objekt has to be placed)
    while(remainingDistance > getGEO_Kante_GEO_Laenge(currentGeoKante.toElement()) + TRAVERSE_DELTA)
    {
        remainingDistance -= getGEO_Kante_GEO_Laenge(currentGeoKante.toElement());
        QDomNode n = containerElement.firstChild();
        while (!n.isNull())
        {
            if (n.isElement())
            {
                QDomElement e = n.toElement();
                if(e.tagName() == "GEO_Kante" && n != currentGeoKante)
                {
                    if(traverseForward && (getGEO_Kante_ID_GEO_Knoten_A(e) == getGEO_Kante_ID_GEO_Knoten_B(currentGeoKante.toElement())) && (getGEO_Kante_ID_GEO_Art(e) == getIdentitaet(topKante)))
                    {
                        currentGeoKante = n;
                        traverseForward = true;
                        break;
                    }
                    else if(traverseForward && (getGEO_Kante_ID_GEO_Knoten_B(e) == getGEO_Kante_ID_GEO_Knoten_B(currentGeoKante.toElement())) && (getGEO_Kante_ID_GEO_Art(e) == getIdentitaet(topKante)))
                    {
                        currentGeoKante = n;
                        traverseForward = false;
                        break;
                    }
                    else if(!traverseForward && (getGEO_Kante_ID_GEO_Knoten_B(e) == getGEO_Kante_ID_GEO_Knoten_A(currentGeoKante.toElement())) && (getGEO_Kante_ID_GEO_Art(e) == getIdentitaet(topKante)))
                    {
                        currentGeoKante = n;
                        traverseForward = false;
                        break;
                    }
                    else if(!traverseForward && (getGEO_Kante_ID_GEO_Knoten_A(e) == getGEO_Kante_ID_GEO_Knoten_A(currentGeoKante.toElement())) && (getGEO_Kante_ID_GEO_Art(e) == getIdentitaet(topKante)))
                    {
                        currentGeoKante = n;
                        traverseForward = true;
                        break;
                    }
                }
            }
            n = n.nextSibling();
        }
    }

    // adjust the remaining distance if it is longer than the edge (due to double precision)
    if(remainingDistance > getGEO_Kante_GEO_Laenge(currentGeoKante.toElement()))
    {
        remainingDistance = getGEO_Kante_GEO_Laenge(currentGeoKante.toElement());
    }

    // Determine the GEO_Punkt objects corresponding to the target edge (where the Punkt_Objekt will be placed).
    // KnotenA is always the node where the remaining distance starts, so if the traversing direction is reverse,
    // the nodes will be swapped.
    QDomNode punktA = QDomNode();
    QDomNode punktB = QDomNode();
    QDomNode n2 = containerElement.firstChild();
    while (!n2.isNull() && (punktA.isNull() || punktB.isNull()))
    {
        if (n2.isElement())
        {
            QDomElement e = n2.toElement();
            if(e.tagName() == "GEO_Punkt")
            {
                if(getGEO_Punkt_ID_GEO_Knoten(e) == getGEO_Kante_ID_GEO_Knoten_A(currentGeoKante.toElement()) && traverseForward)
                {
                    punktA = n2;
                }
                else if(getGEO_Punkt_ID_GEO_Knoten(e) == getGEO_Kante_ID_GEO_Knoten_A(currentGeoKante.toElement()) && !traverseForward)
                {
                    punktB = n2;
                }
                else if(getGEO_Punkt_ID_GEO_Knoten(e) == getGEO_Kante_ID_GEO_Knoten_B(currentGeoKante.toElement()) && traverseForward)
                {
                    punktB = n2;
                }
                else if(getGEO_Punkt_ID_GEO_Knoten(e) == getGEO_Kante_ID_GEO_Knoten_B(currentGeoKante.toElement()) && !traverseForward)
                {
                    punktA = n2;
                }
            }
        }
        n2 = n2.nextSibling();
    }

    // Retrieve the coordinates of the GEO_Punkt objects and calculate the coordinates for the Punkt_Objekt
    if(!punktA.isNull() && !punktB.isNull())
    {
        double ax = getGEO_Punkt_GK_X(punktA.toElement());
        double ay = getGEO_Punkt_GK_Y(punktA.toElement());
        double bx = getGEO_Punkt_GK_X(punktB.toElement());
        double by = getGEO_Punkt_GK_Y(punktB.toElement());

        double lengthFactor = remainingDistance / getGEO_Kante_GEO_Laenge(currentGeoKante.toElement());
        double sideFactor = getPunkt_Objekt_Seitlicher_Abstand(o) / getGEO_Kante_GEO_Laenge(currentGeoKante.toElement());
        double xAdd = qAbs(bx - ax) * lengthFactor;
        double yAdd = qAbs(by - ay) * lengthFactor;
        double xSide = qAbs(bx - ax) * sideFactor;
        double ySide = qAbs(by - ay) * sideFactor;
        double px = 0;
        double py = 0;

        if(ax <= bx && ay <= by)
        {
            px = ax + xAdd + ySide;
            py = ay + yAdd - xSide;
        }
        else if(ax > bx && ay > by)
        {
            px = ax - xAdd - ySide;
            py = ay - yAdd + xSide;
        }
        else if(ax <= bx && ay > by)
        {
            px = ax + xAdd - ySide;
            py = ay - yAdd - xSide;
        }
        else if(ax > bx && ay <= by)
        {
            px = ax - xAdd + ySide;
            py = ay + yAdd + xSide;
        }

        QGraphicsItem* item = (QGraphicsItem*) graphicsScene->addEllipse(px - 2, -py - 2, 4, 4);
        return item;
    }

    return NULL;
}

QGraphicsItem* Parser::placeBereichObjekt(QDomElement o, PlanProModel *model, GraphicsScene *scene, const QPen& pen)
{
    QDomElement containerElement = model->getContainerElement();
    QGraphicsScene* graphicsScene = scene->getGraphicsScene();

    QList<QGraphicsItem*> graphicsitemlist;

    QDomElement teilbereichElement = o.firstChildElement("Bereich_Objekt_Teilbereich");
    while(!teilbereichElement.isNull())
    {
        QString idtopkante = teilbereichElement.firstChildElement("ID_TOP_Kante").firstChildElement("Wert").text();
        double begrenzungA = teilbereichElement.firstChildElement("Begrenzung_A").firstChildElement("Wert").text().toDouble();
        double begrenzungB = teilbereichElement.firstChildElement("Begrenzung_B").firstChildElement("Wert").text().toDouble();

        QDomElement topKante = model->getObjectById(idtopkante);
        QDomElement topKnotenA = model->getObjectById(getTOP_Kante_ID_TOP_Knoten_A(topKante));

        QDomNode currentGeoKante = QDomNode();
        bool traverseForward = true;

        // find the starting GEO_Kante
        QDomNode n = containerElement.firstChild();
        while (!n.isNull())
        {
            if (n.isElement())
            {
                QDomElement e = n.toElement();
                if(e.tagName() == "GEO_Kante")
                {
                    if((getGEO_Kante_ID_GEO_Knoten_A(e) == getTOP_Knoten_ID_GEO_Knoten(topKnotenA)) && (getGEO_Kante_ID_GEO_Art(e) == getIdentitaet(topKante)))
                    {
                        currentGeoKante = n;
                        traverseForward = true;
                        break;
                    }
                    else if((getGEO_Kante_ID_GEO_Knoten_B(e) == getTOP_Knoten_ID_GEO_Knoten(topKnotenA)) && (getGEO_Kante_ID_GEO_Art(e) == getIdentitaet(topKante)))
                    {
                        currentGeoKante = n;
                        traverseForward = false;
                        break;
                    }
                }
            }
            n = n.nextSibling();
        }

        if(currentGeoKante.isNull())
        {
            return NULL;
        }

        // traverse along the GEO_Kante chain until the value of begrenzungA has reached zero
        // (afterwards, currentGeoKante will point to the GEO_Kante where the Bereich_Objekt starts)
        while(begrenzungA > getGEO_Kante_GEO_Laenge(currentGeoKante.toElement()) + TRAVERSE_DELTA)
        {
            begrenzungA -= getGEO_Kante_GEO_Laenge(currentGeoKante.toElement());
            begrenzungB -= getGEO_Kante_GEO_Laenge(currentGeoKante.toElement());
            QDomNode n = containerElement.firstChild();
            while (!n.isNull())
            {
                if (n.isElement())
                {
                    QDomElement e = n.toElement();
                    if(e.tagName() == "GEO_Kante" && n != currentGeoKante)
                    {
                        if(traverseForward && (getGEO_Kante_ID_GEO_Knoten_A(e) == getGEO_Kante_ID_GEO_Knoten_B(currentGeoKante.toElement())) && (getGEO_Kante_ID_GEO_Art(e) == getIdentitaet(topKante)))
                        {
                            currentGeoKante = n;
                            traverseForward = true;
                            break;
                        }
                        else if(traverseForward && (getGEO_Kante_ID_GEO_Knoten_B(e) == getGEO_Kante_ID_GEO_Knoten_B(currentGeoKante.toElement())) && (getGEO_Kante_ID_GEO_Art(e) == getIdentitaet(topKante)))
                        {
                            currentGeoKante = n;
                            traverseForward = false;
                            break;
                        }
                        else if(!traverseForward && (getGEO_Kante_ID_GEO_Knoten_B(e) == getGEO_Kante_ID_GEO_Knoten_A(currentGeoKante.toElement())) && (getGEO_Kante_ID_GEO_Art(e) == getIdentitaet(topKante)))
                        {
                            currentGeoKante = n;
                            traverseForward = false;
                            break;
                        }
                        else if(!traverseForward && (getGEO_Kante_ID_GEO_Knoten_A(e) == getGEO_Kante_ID_GEO_Knoten_A(currentGeoKante.toElement())) && (getGEO_Kante_ID_GEO_Art(e) == getIdentitaet(topKante)))
                        {
                            currentGeoKante = n;
                            traverseForward = true;
                            break;
                        }
                    }
                }
                n = n.nextSibling();
            }
        }

        // adjust the remaining distance if it is longer than the edge (due to double precision)
        if(begrenzungA > getGEO_Kante_GEO_Laenge(currentGeoKante.toElement()))
        {
            begrenzungA = getGEO_Kante_GEO_Laenge(currentGeoKante.toElement());
        }

        // Determine the GEO_Punkt objects corresponding to the starting edge (where the drawing of the Bereich_Objekt starts).
        // KnotenA is always the node where the remaining distance starts, so if the traversing direction is reverse,
        // the nodes will be swapped.
        QDomNode punktA = QDomNode();
        QDomNode punktB = QDomNode();
        QDomNode n2 = containerElement.firstChild();
        while (!n2.isNull() && (punktA.isNull() || punktB.isNull()))
        {
            if (n2.isElement())
            {
                QDomElement e = n2.toElement();
                if(e.tagName() == "GEO_Punkt")
                {
                    if(getGEO_Punkt_ID_GEO_Knoten(e) == getGEO_Kante_ID_GEO_Knoten_A(currentGeoKante.toElement()) && traverseForward)
                    {
                        punktA = n2;
                    }
                    else if(getGEO_Punkt_ID_GEO_Knoten(e) == getGEO_Kante_ID_GEO_Knoten_A(currentGeoKante.toElement()) && !traverseForward)
                    {
                        punktB = n2;
                    }
                    else if(getGEO_Punkt_ID_GEO_Knoten(e) == getGEO_Kante_ID_GEO_Knoten_B(currentGeoKante.toElement()) && traverseForward)
                    {
                        punktB = n2;
                    }
                    else if(getGEO_Punkt_ID_GEO_Knoten(e) == getGEO_Kante_ID_GEO_Knoten_B(currentGeoKante.toElement()) && !traverseForward)
                    {
                        punktA = n2;
                    }
                }
            }
            n2 = n2.nextSibling();
        }

        double px = 0;
        double py = 0;

        // Retrieve the coordinates of the GEO_Punkt objects and calculate the starting coordinates for the Bereich_Objekt
        if(!punktA.isNull() && !punktB.isNull())
        {
            double ax = getGEO_Punkt_GK_X(punktA.toElement());
            double ay = getGEO_Punkt_GK_Y(punktA.toElement());
            double bx = getGEO_Punkt_GK_X(punktB.toElement());
            double by = getGEO_Punkt_GK_Y(punktB.toElement());

            double lengthFactor = begrenzungA / getGEO_Kante_GEO_Laenge(currentGeoKante.toElement());
            double xAdd = qAbs(bx - ax) * lengthFactor;
            double yAdd = qAbs(by - ay) * lengthFactor;

            if(ax <= bx && ay <= by)
            {
                px = ax + xAdd;
                py = ay + yAdd;
            }
            else if(ax > bx && ay > by)
            {
                px = ax - xAdd;
                py = ay - yAdd;
            }
            else if(ax <= bx && ay > by)
            {
                px = ax + xAdd;
                py = ay - yAdd;
            }
            else if(ax > bx && ay <= by)
            {
                px = ax - xAdd;
                py = ay + yAdd;
            }
        }

        // traverse along the GEO_Kante chain until the value of begrenzungB has reached zero
        // (afterwards, currentGeoKante will point to the GEO_Kante where the Bereich_Objekt ends)
        while(begrenzungB > getGEO_Kante_GEO_Laenge(currentGeoKante.toElement()) + TRAVERSE_DELTA)
        {
            begrenzungB -= getGEO_Kante_GEO_Laenge(currentGeoKante.toElement());
            QDomNode n = containerElement.firstChild();
            while (!n.isNull())
            {
                if (n.isElement())
                {
                    QDomElement e = n.toElement();
                    if(e.tagName() == "GEO_Kante" && n != currentGeoKante)
                    {
                        if(traverseForward && (getGEO_Kante_ID_GEO_Knoten_A(e) == getGEO_Kante_ID_GEO_Knoten_B(currentGeoKante.toElement())) && (getGEO_Kante_ID_GEO_Art(e) == getIdentitaet(topKante)))
                        {
                            currentGeoKante = n;
                            traverseForward = true;
                            break;
                        }
                        else if(traverseForward && (getGEO_Kante_ID_GEO_Knoten_B(e) == getGEO_Kante_ID_GEO_Knoten_B(currentGeoKante.toElement())) && (getGEO_Kante_ID_GEO_Art(e) == getIdentitaet(topKante)))
                        {
                            currentGeoKante = n;
                            traverseForward = false;
                            break;
                        }
                        else if(!traverseForward && (getGEO_Kante_ID_GEO_Knoten_B(e) == getGEO_Kante_ID_GEO_Knoten_A(currentGeoKante.toElement())) && (getGEO_Kante_ID_GEO_Art(e) == getIdentitaet(topKante)))
                        {
                            currentGeoKante = n;
                            traverseForward = false;
                            break;
                        }
                        else if(!traverseForward && (getGEO_Kante_ID_GEO_Knoten_A(e) == getGEO_Kante_ID_GEO_Knoten_A(currentGeoKante.toElement())) && (getGEO_Kante_ID_GEO_Art(e) == getIdentitaet(topKante)))
                        {
                            currentGeoKante = n;
                            traverseForward = true;
                            break;
                        }
                    }
                }
                n = n.nextSibling();
            }

            // Determine the GEO_Punkt objects corresponding to the current edge.
            // KnotenA is always the node where the remaining distance starts, so if the traversing direction is reverse,
            // the nodes will be swapped.
            punktA = QDomNode();
            punktB = QDomNode();
            QDomNode n2 = containerElement.firstChild();
            while (!n2.isNull() && (punktA.isNull() || punktB.isNull()))
            {
                if (n2.isElement())
                {
                    QDomElement e = n2.toElement();
                    if(e.tagName() == "GEO_Punkt")
                    {
                        if(getGEO_Punkt_ID_GEO_Knoten(e) == getGEO_Kante_ID_GEO_Knoten_A(currentGeoKante.toElement()) && traverseForward)
                        {
                            punktA = n2;
                        }
                        else if(getGEO_Punkt_ID_GEO_Knoten(e) == getGEO_Kante_ID_GEO_Knoten_A(currentGeoKante.toElement()) && !traverseForward)
                        {
                            punktB = n2;
                        }
                        else if(getGEO_Punkt_ID_GEO_Knoten(e) == getGEO_Kante_ID_GEO_Knoten_B(currentGeoKante.toElement()) && traverseForward)
                        {
                            punktB = n2;
                        }
                        else if(getGEO_Punkt_ID_GEO_Knoten(e) == getGEO_Kante_ID_GEO_Knoten_B(currentGeoKante.toElement()) && !traverseForward)
                        {
                            punktA = n2;
                        }
                    }
                }
                n2 = n2.nextSibling();
            }

            // Retrieve the coordinates of the GEO_Punkt objects and draw the intermediate lines of the Bereich_Objekt
            if(!punktA.isNull() && !punktB.isNull())
            {
                double ax = getGEO_Punkt_GK_X(punktA.toElement());
                double ay = getGEO_Punkt_GK_Y(punktA.toElement());
                //double bx = getGEO_Punkt_GK_X(punktB.toElement());
                //double by = getGEO_Punkt_GK_Y(punktB.toElement());

                QGraphicsItem* item = (QGraphicsItem*) graphicsScene->addLine(px, -py, ax, -ay, pen);
                graphicsitemlist.append(item);
                px = ax;
                py = ay;
            }
        }

        // adjust the remaining distance if it is longer than the edge (due to double precision)
        if(begrenzungB > getGEO_Kante_GEO_Laenge(currentGeoKante.toElement()))
        {
            begrenzungB = getGEO_Kante_GEO_Laenge(currentGeoKante.toElement());
        }

        // Determine the GEO_Punkt objects corresponding to the ending edge (where the drawing of the Bereich_Objekt ends).
        // KnotenA is always the node where the remaining distance starts, so if the traversing direction is reverse,
        // the nodes will be swapped.
        punktA = QDomNode();
        punktB = QDomNode();
        QDomNode n3 = containerElement.firstChild();
        while (!n3.isNull() && (punktA.isNull() || punktB.isNull()))
        {
            if (n3.isElement())
            {
                QDomElement e = n3.toElement();
                if(e.tagName() == "GEO_Punkt")
                {
                    if(getGEO_Punkt_ID_GEO_Knoten(e) == getGEO_Kante_ID_GEO_Knoten_A(currentGeoKante.toElement()) && traverseForward)
                    {
                        punktA = n3;
                    }
                    else if(getGEO_Punkt_ID_GEO_Knoten(e) == getGEO_Kante_ID_GEO_Knoten_A(currentGeoKante.toElement()) && !traverseForward)
                    {
                        punktB = n3;
                    }
                    else if(getGEO_Punkt_ID_GEO_Knoten(e) == getGEO_Kante_ID_GEO_Knoten_B(currentGeoKante.toElement()) && traverseForward)
                    {
                        punktB = n3;
                    }
                    else if(getGEO_Punkt_ID_GEO_Knoten(e) == getGEO_Kante_ID_GEO_Knoten_B(currentGeoKante.toElement()) && !traverseForward)
                    {
                        punktA = n3;
                    }
                }
            }
            n3 = n3.nextSibling();
        }

        // Retrieve the coordinates of the GEO_Punkt objects and calculate the ending coordinates for the Bereich_Objekt
        if(!punktA.isNull() && !punktB.isNull())
        {
            double ax = getGEO_Punkt_GK_X(punktA.toElement());
            double ay = getGEO_Punkt_GK_Y(punktA.toElement());
            double bx = getGEO_Punkt_GK_X(punktB.toElement());
            double by = getGEO_Punkt_GK_Y(punktB.toElement());

            double lengthFactor = begrenzungB / getGEO_Kante_GEO_Laenge(currentGeoKante.toElement());
            double xAdd = qAbs(bx - ax) * lengthFactor;
            double yAdd = qAbs(by - ay) * lengthFactor;

            double endx = 0;
            double endy = 0;

            if(ax <= bx && ay <= by)
            {
                endx = ax + xAdd;
                endy = ay + yAdd;
            }
            else if(ax > bx && ay > by)
            {
                endx = ax - xAdd;
                endy = ay - yAdd;
            }
            else if(ax <= bx && ay > by)
            {
                endx = ax + xAdd;
                endy = ay - yAdd;
            }
            else if(ax > bx && ay <= by)
            {
                endx = ax - xAdd;
                endy = ay + yAdd;
            }

            QGraphicsItem* item = (QGraphicsItem*) graphicsScene->addLine(px, -py, endx, -endy, pen);
            graphicsitemlist.append(item);
        }



        teilbereichElement = teilbereichElement.nextSiblingElement("Bereich_Objekt_Teilbereich");
    }

    QGraphicsItem* groupitem = graphicsScene->createItemGroup(graphicsitemlist);

    return groupitem;
}

QString Parser::getIdentitaet(QDomElement e)
{
    QDomElement identitaetElement = e.firstChildElement("Identitaet");
    QDomElement identitaetWertElement = identitaetElement.firstChildElement("Wert");
    return identitaetWertElement.text();
}

QString Parser::getPunkt_Objekt_ID_TOP_Kante(QDomElement e)
{
    QDomElement topKanteElement = e.firstChildElement("Punkt_Objekt_TOP_Kante");
    QDomElement idTopKanteElement = topKanteElement.firstChildElement("ID_TOP_Kante");
    QDomElement idTopKanteWertElement = idTopKanteElement.firstChildElement("Wert");
    return idTopKanteWertElement.text();
}

double Parser::getPunkt_Objekt_Abstand(QDomElement e)
{
    QDomElement topKanteElement = e.firstChildElement("Punkt_Objekt_TOP_Kante");
    QDomElement abstandElement = topKanteElement.firstChildElement("Abstand");
    QDomElement abstandWertElement = abstandElement.firstChildElement("Wert");
    return abstandWertElement.text().toDouble();
}

double Parser::getPunkt_Objekt_Seitlicher_Abstand(QDomElement e)
{
    QDomElement topKanteElement = e.firstChildElement("Punkt_Objekt_TOP_Kante");
    QDomElement seitlicherAbstandElement = topKanteElement.firstChildElement("Seitlicher_Abstand");
    QDomElement seitlicherAbstandWertElement = seitlicherAbstandElement.firstChildElement("Wert");
    return seitlicherAbstandWertElement.text().toDouble();
}

QString Parser::getPunkt_Objekt_Seitliche_Lage(QDomElement e)
{
    QDomElement topKanteElement = e.firstChildElement("Punkt_Objekt_TOP_Kante");
    QDomElement seitlicheLageElement = topKanteElement.firstChildElement("Seitliche_Lage");
    QDomElement seitlicheLageWertElement = seitlicheLageElement.firstChildElement("Wert");
    return seitlicheLageWertElement.text();
}

QString Parser::getPunkt_Objekt_Wirkrichtung(QDomElement e)
{
    QDomElement topKanteElement = e.firstChildElement("Punkt_Objekt_TOP_Kante");
    QDomElement wirkrichtungElement = topKanteElement.firstChildElement("Wirkrichtung");
    QDomElement wirkrichtungWertElement = wirkrichtungElement.firstChildElement("Wert");
    return wirkrichtungWertElement.text();
}

QString Parser::getGEO_Kante_ID_GEO_Knoten_A(QDomElement e)
{
    QDomElement idGeoKnotenAElement = e.firstChildElement("ID_GEO_Knoten_A");
    QDomElement idGeoKnotenAWertElement = idGeoKnotenAElement.firstChildElement("Wert");
    return idGeoKnotenAWertElement.text();
}

QString Parser::getGEO_Kante_ID_GEO_Knoten_B(QDomElement e)
{
    QDomElement idGeoKnotenBElement = e.firstChildElement("ID_GEO_Knoten_B");
    QDomElement idGeoKnotenBWertElement = idGeoKnotenBElement.firstChildElement("Wert");
    return idGeoKnotenBWertElement.text();
}

QString Parser::getGEO_Kante_ID_GEO_Art(QDomElement e)
{
    QDomElement idGeoArtElement = e.firstChildElement("ID_GEO_Art");
    QDomElement idGeoArtWertElement = idGeoArtElement.firstChildElement("Wert");
    return idGeoArtWertElement.text();
}

double Parser::getGEO_Kante_GEO_Laenge(QDomElement e)
{
    QDomElement kanteAllgElement = e.firstChildElement("GEO_Kante_Allg");
    QDomElement laengeElement = kanteAllgElement.firstChildElement("GEO_Laenge");
    QDomElement laengeWertElement = laengeElement.firstChildElement("Wert");
    return laengeWertElement.text().toDouble();
}

QString Parser::getGEO_Punkt_ID_GEO_Knoten(QDomElement e)
{
    QDomElement idGeoKnotenElement = e.firstChildElement("ID_GEO_Knoten");
    QDomElement idGeoKnotenWertElement = idGeoKnotenElement.firstChildElement("Wert");
    return idGeoKnotenWertElement.text();
}

double Parser::getGEO_Punkt_GK_X(QDomElement e)
{
    QDomElement punktAllgElement = e.firstChildElement("GEO_Punkt_Allg");
    QDomElement gkxElement = punktAllgElement.firstChildElement("GK_X");
    QDomElement gkxWertElement = gkxElement.firstChildElement("Wert");
    return gkxWertElement.text().toDouble();
}

double Parser::getGEO_Punkt_GK_Y(QDomElement e)
{
    QDomElement punktAllgElement = e.firstChildElement("GEO_Punkt_Allg");
    QDomElement gkyElement = punktAllgElement.firstChildElement("GK_Y");
    QDomElement gkyWertElement = gkyElement.firstChildElement("Wert");
    return gkyWertElement.text().toDouble();
}

QString Parser::getTOP_Knoten_ID_GEO_Knoten(QDomElement e)
{
    QDomElement idGeoKnotenElement = e.firstChildElement("ID_GEO_Knoten");
    QDomElement idGeoKnotenWertElement = idGeoKnotenElement.firstChildElement("Wert");
    return idGeoKnotenWertElement.text();
}

QString Parser::getTOP_Kante_ID_TOP_Knoten_A(QDomElement e)
{
    QDomElement idTopKnotenAElement = e.firstChildElement("ID_TOP_Knoten_A");
    QDomElement idTopKnotenAWertElement = idTopKnotenAElement.firstChildElement("Wert");
    return idTopKnotenAWertElement.text();
}

QString Parser::getTOP_Kante_ID_TOP_Knoten_B(QDomElement e)
{
    QDomElement idTopKnotenBElement = e.firstChildElement("ID_TOP_Knoten_B");
    QDomElement idTopKnotenBWertElement = idTopKnotenBElement.firstChildElement("Wert");
    return idTopKnotenBWertElement.text();
}

QString Parser::getTOP_Kante_TOP_Anschluss_A(QDomElement e)
{
    QDomElement kanteAllgElement = e.firstChildElement("TOP_Kante_Allg");
    QDomElement anschlussElement = kanteAllgElement.firstChildElement("TOP_Anschluss_A");
    QDomElement anschlussWertElement = anschlussElement.firstChildElement("Wert");
    return anschlussWertElement.text();
}

QString Parser::getTOP_Kante_TOP_Anschluss_B(QDomElement e)
{
    QDomElement kanteAllgElement = e.firstChildElement("TOP_Kante_Allg");
    QDomElement anschlussElement = kanteAllgElement.firstChildElement("TOP_Anschluss_B");
    QDomElement anschlussWertElement = anschlussElement.firstChildElement("Wert");
    return anschlussWertElement.text();
}

double Parser::getTOP_Kante_TOP_Laenge(QDomElement e)
{
    QDomElement kanteAllgElement = e.firstChildElement("TOP_Kante_Allg");
    QDomElement laengeElement = kanteAllgElement.firstChildElement("TOP_Laenge");
    QDomElement laengeWertElement = laengeElement.firstChildElement("Wert");
    return laengeWertElement.text().toDouble();
}
