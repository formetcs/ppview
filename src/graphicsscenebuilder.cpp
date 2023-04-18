#include <QGraphicsScene>
#include <QGraphicsEllipseItem>

#include "graphicsscenebuilder.h"
#include "planprodocument.h"
#include "graphicsscene.h"

#include <iostream>


const double TRAVERSE_DELTA = 0.1; // to avoid problems with double precision


GraphicsSceneBuilder::GraphicsSceneBuilder(PlanProDocument* doc, GraphicsScene* scene)
{
    document = doc;
    graphicsScene = scene;
}


void GraphicsSceneBuilder::createGraphicsScene()
{
    if(!document || !graphicsScene)
    {
        return;
    }

    graphicsScene->clear();
    QList<DomItem*> objectlist = document->getObjectList(PlanProDocument::End);
    for (int i = 0; i < objectlist.count(); ++i)
    {
        DomItem* item = objectlist.at(i);
        QString name = item->getName();
        QString id = item->getFirstValueAtPath("Identitaet/Wert");
        if(name == "GEO_Punkt")
        {
            QString tooltip = "GEO_Punkt\n";
            tooltip += id;
            double x = item->getFirstValueAtPath("GEO_Punkt_Allg/GK_X/Wert").toDouble();
            double y = item->getFirstValueAtPath("GEO_Punkt_Allg/GK_Y/Wert").toDouble();
            QGraphicsItem* item = (QGraphicsItem*) graphicsScene->addEllipse(x - 2, -y - 2, 4, 4, QPen(Qt::black), QBrush(Qt::white));
            item->setToolTip(tooltip);
            item->setData(GRAPHICSITEM_TYPE, "GEO_Punkt");
            item->setData(GRAPHICSITEM_ID, id);
            item->setFlag(QGraphicsItem::ItemIsSelectable);
            item->setZValue(-3);
        }
        else if(name == "GEO_Knoten")
        {
            QString tooltip = "GEO_Knoten\n";
            tooltip += id;
            for (int j = 0; j < objectlist.count(); ++j)
            {
                DomItem* item2 = objectlist.at(j);
                if(item2->getName() == "GEO_Punkt" && item2->getFirstValueAtPath("ID_GEO_Knoten/Wert") == id)
                {
                    double px = item2->getFirstValueAtPath("GEO_Punkt_Allg/GK_X/Wert").toDouble();
                    double py = item2->getFirstValueAtPath("GEO_Punkt_Allg/GK_Y/Wert").toDouble();
                    QGraphicsItem* item = (QGraphicsItem*) graphicsScene->addEllipse(px - 2, -py - 2, 4, 4, QPen(Qt::black), QBrush(Qt::black));
                    item->setToolTip(tooltip);
                    item->setData(GRAPHICSITEM_TYPE, "GEO_Knoten");
                    item->setData(GRAPHICSITEM_ID, id);
                    item->setFlag(QGraphicsItem::ItemIsSelectable);
                    item->setZValue(-2);
                    break;
                }
            }
        }
        else if(name == "GEO_Kante")
        {
            QString tooltip = "GEO_Kante\n";
            tooltip += id;
            QString idknotena = item->getFirstValueAtPath("ID_GEO_Knoten_A/Wert");
            QString idknotenb = item->getFirstValueAtPath("ID_GEO_Knoten_B/Wert");
            DomItem* punktA = NULL;
            DomItem* punktB = NULL;
            for (int j = 0; j < objectlist.count() && (!punktA || !punktB); ++j)
            {
                DomItem* item2 = objectlist.at(j);
                if(item2->getName() == "GEO_Punkt")
                {
                    if(item2->getFirstValueAtPath("ID_GEO_Knoten/Wert") == idknotena)
                    {
                        punktA = item2;
                    }
                    else if(item2->getFirstValueAtPath("ID_GEO_Knoten/Wert") == idknotenb)
                    {
                        punktB = item2;
                    }
                }
            }
            if(punktA && punktB)
            {
                double ax = punktA->getFirstValueAtPath("GEO_Punkt_Allg/GK_X/Wert").toDouble();
                double ay = punktA->getFirstValueAtPath("GEO_Punkt_Allg/GK_Y/Wert").toDouble();
                double bx = punktB->getFirstValueAtPath("GEO_Punkt_Allg/GK_X/Wert").toDouble();
                double by = punktB->getFirstValueAtPath("GEO_Punkt_Allg/GK_Y/Wert").toDouble();
                QGraphicsItem* item = (QGraphicsItem*) graphicsScene->addLine(ax, -ay, bx, -by);
                item->setToolTip(tooltip);
                item->setData(GRAPHICSITEM_TYPE, "GEO_Kante");
                item->setData(GRAPHICSITEM_ID, id);
                item->setFlag(QGraphicsItem::ItemIsSelectable);
                item->setZValue(-20);
            }
        }
        else if(name == "TOP_Knoten")
        {
            QString tooltip = "TOP_Knoten\n";
            tooltip += id;
            QString idknoten = item->getFirstValueAtPath("ID_GEO_Knoten/Wert");
            for (int j = 0; j < objectlist.count(); ++j)
            {
                DomItem* item2 = objectlist.at(j);
                if(item2->getName() == "GEO_Punkt" && item2->getFirstValueAtPath("ID_GEO_Knoten/Wert") == idknoten)
                {
                    double px = item2->getFirstValueAtPath("GEO_Punkt_Allg/GK_X/Wert").toDouble();
                    double py = item2->getFirstValueAtPath("GEO_Punkt_Allg/GK_Y/Wert").toDouble();
                    QGraphicsItem* item = (QGraphicsItem*) graphicsScene->addEllipse(px - 2, -py - 2, 4, 4, QPen(Qt::red), QBrush(Qt::red));
                    item->setToolTip(tooltip);
                    item->setData(GRAPHICSITEM_TYPE, "TOP_Knoten");
                    item->setData(GRAPHICSITEM_ID, id);
                    item->setFlag(QGraphicsItem::ItemIsSelectable);
                    item->setZValue(-1);
                    break;
                }
            }
        }
        else if(name == "TOP_Kante")
        {
            QString tooltip = "TOP_Kante\n";
            tooltip += id;
            QList<QGraphicsItem*> graphicsitemlist;
            for (int j = 0; j < objectlist.count(); ++j)
            {
                DomItem* item2 = objectlist.at(j);
                if(item2->getName() == "GEO_Kante" && item2->getFirstValueAtPath("ID_GEO_Art/Wert") == id)
                {
                    QString idknotena = item2->getFirstValueAtPath("ID_GEO_Knoten_A/Wert");
                    QString idknotenb = item2->getFirstValueAtPath("ID_GEO_Knoten_B/Wert");
                    DomItem* punktA = NULL;
                    DomItem* punktB = NULL;
                    for (int k = 0; k < objectlist.count() && (!punktA || !punktB); ++k)
                    {
                        DomItem* item3 = objectlist.at(k);
                        if(item3->getName() == "GEO_Punkt")
                        {
                            if(item3->getFirstValueAtPath("ID_GEO_Knoten/Wert") == idknotena)
                            {
                                punktA = item3;
                            }
                            else if(item3->getFirstValueAtPath("ID_GEO_Knoten/Wert") == idknotenb)
                            {
                                punktB = item3;
                            }
                        }
                    }
                    if(punktA && punktB)
                    {
                        double ax = punktA->getFirstValueAtPath("GEO_Punkt_Allg/GK_X/Wert").toDouble();
                        double ay = punktA->getFirstValueAtPath("GEO_Punkt_Allg/GK_Y/Wert").toDouble();
                        double bx = punktB->getFirstValueAtPath("GEO_Punkt_Allg/GK_X/Wert").toDouble();
                        double by = punktB->getFirstValueAtPath("GEO_Punkt_Allg/GK_Y/Wert").toDouble();
                        QGraphicsItem* item = (QGraphicsItem*) graphicsScene->addLine(ax, -ay, bx, -by, QPen(Qt::red));
                        graphicsitemlist.append(item);
                    }
                }
            }
            QGraphicsItem* groupitem = graphicsScene->createItemGroup(graphicsitemlist);
            groupitem->setToolTip(tooltip);
            groupitem->setData(GRAPHICSITEM_TYPE, "TOP_Kante");
            groupitem->setData(GRAPHICSITEM_ID, id);
            groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
            groupitem->setZValue(-10);
        }
        else if(name == "Datenpunkt")
        {
            QString datenpunktbeschreibungtext = item->getFirstValueAtPath("Datenpunkt_Allg/Datenpunkt_Beschreibung/Wert");
            QString km = item->getFirstValueAtPath("Punkt_Objekt_Strecke/Strecke_Km/Wert");
            QString tooltip = "Datenpunkt (Km: " + km + ")\n";
            tooltip += id + "\n";
            tooltip += datenpunktbeschreibungtext;

            QGraphicsEllipseItem* retval = (QGraphicsEllipseItem*) placePunktObjekt(item);
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
                retval->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
        else if(name == "ZUB_Bereichsgrenze")
        {
            QString km = item->getFirstValueAtPath("Punkt_Objekt_Strecke/Strecke_Km/Wert");
            QString tooltip = "ZUB_Bereichsgrenze (Km: " + km + ")\n";
            tooltip += id;

            QGraphicsEllipseItem* retval = (QGraphicsEllipseItem*) placePunktObjekt(item);
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
                retval->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
        else if(name == "Bahnsteig_Zugang")
        {
            QString km = item->getFirstValueAtPath("Punkt_Objekt_Strecke/Strecke_Km/Wert");
            QString tooltip = "Bahnsteig_Zugang (Km: " + km + ")\n";
            tooltip += id;

            QGraphicsEllipseItem* retval = (QGraphicsEllipseItem*) placePunktObjekt(item);
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
                retval->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
        else if(name == "BUE_Anlage")
        {
            QString bezeichnunglageplankurztext = item->getFirstValueAtPath("Bezeichnung/Bezeichnung_Lageplan_Kurz/Wert");
            QString km = item->getFirstValueAtPath("Punkt_Objekt_Strecke/Strecke_Km/Wert");
            QString tooltip = "BUE_Anlage (Km: " + km + ")\n";
            tooltip += id + "\n";
            tooltip += bezeichnunglageplankurztext;

            QGraphicsEllipseItem* retval = (QGraphicsEllipseItem*) placePunktObjekt(item);
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
                retval->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
        else if(name == "Sonstiger_Punkt")
        {
            QString km = item->getFirstValueAtPath("Punkt_Objekt_Strecke/Strecke_Km/Wert");
            QString tooltip = "Sonstiger_Punkt (Km: " + km + ")\n";
            tooltip += id;

            QGraphicsEllipseItem* retval = (QGraphicsEllipseItem*) placePunktObjekt(item);
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
                retval->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
        else if(name == "Hoehenpunkt")
        {
            QString km = item->getFirstValueAtPath("Punkt_Objekt_Strecke/Strecke_Km/Wert");
            QString tooltip = "Hoehenpunkt (Km: " + km + ")\n";
            tooltip += id;

            QGraphicsEllipseItem* retval = (QGraphicsEllipseItem*) placePunktObjekt(item);
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
                retval->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
        else if(name == "Technischer_Punkt")
        {
            QString km = item->getFirstValueAtPath("Punkt_Objekt_Strecke/Strecke_Km/Wert");
            QString tooltip = "Technischer_Punkt (Km: " + km + ")\n";
            tooltip += id;

            QGraphicsEllipseItem* retval = (QGraphicsEllipseItem*) placePunktObjekt(item);
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
                retval->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
        else if(name == "Ueberhoehung")
        {
            QString km = item->getFirstValueAtPath("Punkt_Objekt_Strecke/Strecke_Km/Wert");
            QString tooltip = "Ueberhoehung (Km: " + km + ")\n";
            tooltip += id;

            QGraphicsEllipseItem* retval = (QGraphicsEllipseItem*) placePunktObjekt(item);
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
                retval->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
        else if(name == "Gleis_Abschluss")
        {
            QString km = item->getFirstValueAtPath("Punkt_Objekt_Strecke/Strecke_Km/Wert");
            QString tooltip = "Gleis_Abschluss (Km: " + km + ")\n";
            tooltip += id;

            QGraphicsEllipseItem* retval = (QGraphicsEllipseItem*) placePunktObjekt(item);
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
                retval->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
        else if(name == "NB_Zone_Grenze")
        {
            QString km = item->getFirstValueAtPath("Punkt_Objekt_Strecke/Strecke_Km/Wert");
            QString tooltip = "NB_Zone_Grenze (Km: " + km + ")\n";
            tooltip += id;

            QGraphicsEllipseItem* retval = (QGraphicsEllipseItem*) placePunktObjekt(item);
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
                retval->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
        else if(name == "FMA_Element")
        {
            QString km = item->getFirstValueAtPath("Punkt_Objekt_Strecke/Strecke_Km/Wert");
            QString tooltip = "FMA_Element (Km: " + km + ")\n";
            tooltip += id;

            QGraphicsEllipseItem* retval = (QGraphicsEllipseItem*) placePunktObjekt(item);
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
                retval->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
        else if(name == "FMA_Komponente")
        {
            QString bezeichnunglageplankurztext = item->getFirstValueAtPath("Bezeichnung/Bezeichnung_Lageplan_Kurz/Wert");
            QString km = item->getFirstValueAtPath("Punkt_Objekt_Strecke/Strecke_Km/Wert");
            QString tooltip = "FMA_Komponente (Km: " + km + ")\n";
            tooltip += id + "\n";
            tooltip += bezeichnunglageplankurztext;

            QGraphicsEllipseItem* retval = (QGraphicsEllipseItem*) placePunktObjekt(item);
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
                retval->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
        else if(name == "Zugeinwirkung")
        {
            QString bezeichnunglageplankurztext = item->getFirstValueAtPath("Bezeichnung/Bezeichnung_Lageplan_Kurz/Wert");
            QString km = item->getFirstValueAtPath("Punkt_Objekt_Strecke/Strecke_Km/Wert");
            QString tooltip = "Zugeinwirkung (Km: " + km + ")\n";
            tooltip += id + "\n";
            tooltip += bezeichnunglageplankurztext;

            QGraphicsEllipseItem* retval = (QGraphicsEllipseItem*) placePunktObjekt(item);
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
                retval->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
        else if(name == "PZB_Element")
        {
            QString km = item->getFirstValueAtPath("Punkt_Objekt_Strecke/Strecke_Km/Wert");
            QString tooltip = "PZB_Element (Km: " + km + ")\n";
            tooltip += id;

            QGraphicsEllipseItem* retval = (QGraphicsEllipseItem*) placePunktObjekt(item);
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
                retval->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
        else if(name == "Signal")
        {
            QString bezeichnunglageplankurztext = item->getFirstValueAtPath("Bezeichnung/Bezeichnung_Lageplan_Kurz/Wert");
            QString km = item->getFirstValueAtPath("Punkt_Objekt_Strecke/Strecke_Km/Wert");
            QString tooltip = "Signal (Km: " + km + ")\n";
            tooltip += id + "\n";
            tooltip += bezeichnunglageplankurztext;


            QGraphicsEllipseItem* retval = (QGraphicsEllipseItem*) placePunktObjekt(item);
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
                retval->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
        else if(name == "Signal_Befestigung")
        {
            QString km = item->getFirstValueAtPath("Punkt_Objekt_Strecke/Strecke_Km/Wert");
            QString tooltip = "Signal_Befestigung (Km: " + km + ")\n";
            tooltip += id;

            QGraphicsEllipseItem* retval = (QGraphicsEllipseItem*) placePunktObjekt(item);
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
                retval->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
        else if(name == "W_Kr_Gsp_Komponente")
        {
            QString km = item->getFirstValueAtPath("Punkt_Objekt_Strecke/Strecke_Km/Wert");
            QString tooltip = "W_Kr_Gsp_Komponente (Km: " + km + ")\n";
            tooltip += id;

            QGraphicsEllipseItem* retval = (QGraphicsEllipseItem*) placePunktObjekt(item);
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
                retval->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
        else if(name == "ZUB_Streckeneigenschaft")
        {
            QString tooltip = "ZUB_Streckeneigenschaft\n";
            tooltip += id;

            QGraphicsItem* retval = placeBereichObjekt(item, QPen(Qt::yellow));
            if(retval == NULL)
            {
                std::cout << "ZUB_Streckeneigenschaft not placeable" << std::endl;
            }
            else
            {
                retval->setToolTip(tooltip);
                retval->setData(GRAPHICSITEM_TYPE, "ZUB_Streckeneigenschaft");
                retval->setData(GRAPHICSITEM_ID, id);
                retval->setFlag(QGraphicsItem::ItemIsSelectable);
                retval->setZValue(-5);
            }
        }
        else if(name == "Stell_Bereich")
        {
            QString tooltip = "Stell_Bereich\n";
            tooltip += id;

            QGraphicsItem* retval = placeBereichObjekt(item, QPen("orange"));
            if(retval == NULL)
            {
                std::cout << "Stell_Bereich not placeable" << std::endl;
            }
            else
            {
                retval->setToolTip(tooltip);
                retval->setData(GRAPHICSITEM_TYPE, "Stell_Bereich");
                retval->setData(GRAPHICSITEM_ID, id);
                retval->setFlag(QGraphicsItem::ItemIsSelectable);
                retval->setZValue(-5);
            }
        }
        else if(name == "Bahnsteig_Kante")
        {
            QString tooltip = "Bahnsteig_Kante\n";
            tooltip += id;

            QGraphicsItem* retval = placeBereichObjekt(item, QPen(Qt::cyan));
            if(retval == NULL)
            {
                std::cout << "Bahnsteig_Kante not placeable" << std::endl;
            }
            else
            {
                retval->setToolTip(tooltip);
                retval->setData(GRAPHICSITEM_TYPE, "Bahnsteig_Kante");
                retval->setData(GRAPHICSITEM_ID, id);
                retval->setFlag(QGraphicsItem::ItemIsSelectable);
                retval->setZValue(-5);
            }
        }
        else if(name == "BUE_Gleisbezogener_Gefahrraum")
        {
            QString tooltip = "BUE_Gleisbezogener_Gefahrraum\n";
            tooltip += id;

            QGraphicsItem* retval = placeBereichObjekt(item, QPen(Qt::blue));
            if(retval == NULL)
            {
                std::cout << "BUE_Gleisbezogener_Gefahrraum not placeable" << std::endl;
            }
            else
            {
                retval->setToolTip(tooltip);
                retval->setData(GRAPHICSITEM_TYPE, "BUE_Gleisbezogener_Gefahrraum");
                retval->setData(GRAPHICSITEM_ID, id);
                retval->setFlag(QGraphicsItem::ItemIsSelectable);
                retval->setZValue(-5);
            }
        }
        else if(name == "Fstr_Fahrweg")
        {
            QString tooltip = "Fstr_Fahrweg\n";
            tooltip += id;

            QGraphicsItem* retval = placeBereichObjekt(item, QPen(Qt::magenta));
            if(retval == NULL)
            {
                std::cout << "Fstr_Fahrweg not placeable" << std::endl;
            }
            else
            {
                retval->setToolTip(tooltip);
                retval->setData(GRAPHICSITEM_TYPE, "Fstr_Fahrweg");
                retval->setData(GRAPHICSITEM_ID, id);
                retval->setFlag(QGraphicsItem::ItemIsSelectable);
                retval->setZValue(-5);
            }
        }
        else if(name == "Geschwindigkeitsprofil")
        {
            QString tooltip = "Geschwindigkeitsprofil\n";
            tooltip += id;

            QGraphicsItem* retval = placeBereichObjekt(item, QPen(Qt::red));
            if(retval == NULL)
            {
                std::cout << "Geschwindigkeitsprofil not placeable" << std::endl;
            }
            else
            {
                retval->setToolTip(tooltip);
                retval->setData(GRAPHICSITEM_TYPE, "Geschwindigkeitsprofil");
                retval->setData(GRAPHICSITEM_ID, id);
                retval->setFlag(QGraphicsItem::ItemIsSelectable);
                retval->setZValue(-5);
            }
        }
        else if(name == "Strecke")
        {
            QString tooltip = "Strecke\n";
            tooltip += id;

            QGraphicsItem* retval = placeBereichObjekt(item, QPen(Qt::red));
            if(retval == NULL)
            {
                std::cout << "Strecke not placeable" << std::endl;
            }
            else
            {
                retval->setToolTip(tooltip);
                retval->setData(GRAPHICSITEM_TYPE, "Strecke");
                retval->setData(GRAPHICSITEM_ID, id);
                retval->setFlag(QGraphicsItem::ItemIsSelectable);
                retval->setZValue(-5);
            }
        }
        else if(name == "Technischer_Bereich")
        {
            QString tooltip = "Technischer_Bereich\n";
            tooltip += id;

            QGraphicsItem* retval = placeBereichObjekt(item, QPen(Qt::red));
            if(retval == NULL)
            {
                std::cout << "Technischer_Bereich not placeable" << std::endl;
            }
            else
            {
                retval->setToolTip(tooltip);
                retval->setData(GRAPHICSITEM_TYPE, "Technischer_Bereich");
                retval->setData(GRAPHICSITEM_ID, id);
                retval->setFlag(QGraphicsItem::ItemIsSelectable);
                retval->setZValue(-5);
            }
        }
        else if(name == "Gleis_Abschnitt")
        {
            QString tooltip = "Gleis_Abschnitt\n";
            tooltip += id;

            QGraphicsItem* retval = placeBereichObjekt(item, QPen("olive"));
            if(retval == NULL)
            {
                std::cout << "Gleis_Abschnitt not placeable" << std::endl;
            }
            else
            {
                retval->setToolTip(tooltip);
                retval->setData(GRAPHICSITEM_TYPE, "Gleis_Abschnitt");
                retval->setData(GRAPHICSITEM_ID, id);
                retval->setFlag(QGraphicsItem::ItemIsSelectable);
                retval->setZValue(-5);
            }
        }
        else if(name == "Gleis_Art")
        {
            QString tooltip = "Gleis_Art\n";
            tooltip += id;

            QGraphicsItem* retval = placeBereichObjekt(item, QPen("olive"));
            if(retval == NULL)
            {
                std::cout << "Gleis_Art not placeable" << std::endl;
            }
            else
            {
                retval->setToolTip(tooltip);
                retval->setData(GRAPHICSITEM_TYPE, "Gleis_Art");
                retval->setData(GRAPHICSITEM_ID, id);
                retval->setFlag(QGraphicsItem::ItemIsSelectable);
                retval->setZValue(-5);
            }
        }
        else if(name == "Gleis_Baubereich")
        {
            QString tooltip = "Gleis_Baubereich\n";
            tooltip += id;

            QGraphicsItem* retval = placeBereichObjekt(item, QPen("olive"));
            if(retval == NULL)
            {
                std::cout << "Gleis_Baubereich not placeable" << std::endl;
            }
            else
            {
                retval->setToolTip(tooltip);
                retval->setData(GRAPHICSITEM_TYPE, "Gleis_Baubereich");
                retval->setData(GRAPHICSITEM_ID, id);
                retval->setFlag(QGraphicsItem::ItemIsSelectable);
                retval->setZValue(-5);
            }
        }
        else if(name == "Gleis_Bezeichnung")
        {
            QString tooltip = "Gleis_Bezeichnung\n";
            tooltip += id;

            QGraphicsItem* retval = placeBereichObjekt(item, QPen("olive"));
            if(retval == NULL)
            {
                std::cout << "Gleis_Bezeichnung not placeable" << std::endl;
            }
            else
            {
                retval->setToolTip(tooltip);
                retval->setData(GRAPHICSITEM_TYPE, "Gleis_Bezeichnung");
                retval->setData(GRAPHICSITEM_ID, id);
                retval->setFlag(QGraphicsItem::ItemIsSelectable);
                retval->setZValue(-5);
            }
        }
        else if(name == "Gleis_Fahrbahn")
        {
            QString tooltip = "Gleis_Fahrbahn\n";
            tooltip += id;

            QGraphicsItem* retval = placeBereichObjekt(item, QPen("olive"));
            if(retval == NULL)
            {
                std::cout << "Gleis_Fahrbahn not placeable" << std::endl;
            }
            else
            {
                retval->setToolTip(tooltip);
                retval->setData(GRAPHICSITEM_TYPE, "Gleis_Fahrbahn");
                retval->setData(GRAPHICSITEM_ID, id);
                retval->setFlag(QGraphicsItem::ItemIsSelectable);
                retval->setZValue(-5);
            }
        }
        else if(name == "Gleis_Lichtraum")
        {
            QString tooltip = "Gleis_Lichtraum\n";
            tooltip += id;

            QGraphicsItem* retval = placeBereichObjekt(item, QPen("olive"));
            if(retval == NULL)
            {
                std::cout << "Gleis_Lichtraum not placeable" << std::endl;
            }
            else
            {
                retval->setToolTip(tooltip);
                retval->setData(GRAPHICSITEM_TYPE, "Gleis_Lichtraum");
                retval->setData(GRAPHICSITEM_ID, id);
                retval->setFlag(QGraphicsItem::ItemIsSelectable);
                retval->setZValue(-5);
            }
        }
        else if(name == "Gleis_Schaltgruppe")
        {
            QString tooltip = "Gleis_Schaltgruppe\n";
            tooltip += id;

            QGraphicsItem* retval = placeBereichObjekt(item, QPen("olive"));
            if(retval == NULL)
            {
                std::cout << "Gleis_Schaltgruppe not placeable" << std::endl;
            }
            else
            {
                retval->setToolTip(tooltip);
                retval->setData(GRAPHICSITEM_TYPE, "Gleis_Schaltgruppe");
                retval->setData(GRAPHICSITEM_ID, id);
                retval->setFlag(QGraphicsItem::ItemIsSelectable);
                retval->setZValue(-5);
            }
        }
        else
        {
            std::cout << "Unknown object type " << qPrintable(name) << std::endl;
        }
    }
}


QGraphicsItem* GraphicsSceneBuilder::placePunktObjekt(DomItem* o)
{
    if(!document || !graphicsScene)
    {
        return NULL;
    }

    // if the Punkt_Objekt has more than one referencing TOP_Kante, we take only the first
    // (the resulting point must be the same for all edges)
    DomItem* punktObjektTopKante = o->getFirstChildItem("Punkt_Objekt_TOP_Kante");
    double remainingDistance = punktObjektTopKante->getFirstValueAtPath("Abstand/Wert").toDouble();

    DomItem* topKante = document->getObjectById(punktObjektTopKante->getFirstValueAtPath("ID_TOP_Kante/Wert"), PlanProDocument::End);
    QString idTopKante = topKante->getFirstValueAtPath("Identitaet/Wert");
    DomItem* topKnotenA = document->getObjectById(topKante->getFirstValueAtPath("ID_TOP_Knoten_A/Wert"), PlanProDocument::End);
    QString idGeoKnoten = topKnotenA->getFirstValueAtPath("ID_GEO_Knoten/Wert");

    DomItem* currentGeoKante = NULL;
    bool traverseForward = true;

    // find the starting GEO_Kante
    QList<DomItem*> objectlist = document->getObjectList(PlanProDocument::End);
    for (int i = 0; i < objectlist.count(); ++i)
    {
        DomItem* item = objectlist.at(i);
        if(item->getName() == "GEO_Kante")
        {
            QString idGeoKnotenA = item->getFirstValueAtPath("ID_GEO_Knoten_A/Wert");
            QString idGeoKnotenB = item->getFirstValueAtPath("ID_GEO_Knoten_B/Wert");
            QString idGeoArt = item->getFirstValueAtPath("ID_GEO_Art/Wert");
            if((idGeoKnotenA == idGeoKnoten) && (idGeoArt == idTopKante))
            {
                currentGeoKante = item;
                traverseForward = true;
                break;
            }
            else if((idGeoKnotenB == idGeoKnoten) && (idGeoArt == idTopKante))
            {
                currentGeoKante = item;
                traverseForward = false;
                break;
            }
        }
    }

    if(!currentGeoKante)
    {
        return NULL;
    }

    // traverse along the GEO_Kante chain until the remaining distance has reached zero (with delta value due to double precision)
    // (afterwards, currentGeoKante will point to the GEO_Kante where the Punkt_Objekt has to be placed)
    while(remainingDistance > currentGeoKante->getFirstValueAtPath("GEO_Kante_Allg/GEO_Laenge/Wert").toDouble() + TRAVERSE_DELTA)
    {
        remainingDistance -= currentGeoKante->getFirstValueAtPath("GEO_Kante_Allg/GEO_Laenge/Wert").toDouble();
        for (int i = 0; i < objectlist.count(); ++i)
        {
            DomItem* item = objectlist.at(i);
            if(item->getName() == "GEO_Kante" && item != currentGeoKante)
            {
                QString idGeoKnotenA = item->getFirstValueAtPath("ID_GEO_Knoten_A/Wert");
                QString idGeoKnotenB = item->getFirstValueAtPath("ID_GEO_Knoten_B/Wert");
                QString idGeoArt = item->getFirstValueAtPath("ID_GEO_Art/Wert");
                QString idCurrentGeoKnotenA = currentGeoKante->getFirstValueAtPath("ID_GEO_Knoten_A/Wert");
                QString idCurrentGeoKnotenB = currentGeoKante->getFirstValueAtPath("ID_GEO_Knoten_B/Wert");
                if(traverseForward && (idGeoKnotenA == idCurrentGeoKnotenB) && (idGeoArt == idTopKante))
                {
                    currentGeoKante = item;
                    traverseForward = true;
                    break;
                }
                else if(traverseForward && (idGeoKnotenB == idCurrentGeoKnotenB) && (idGeoArt == idTopKante))
                {
                    currentGeoKante = item;
                    traverseForward = false;
                    break;
                }
                else if(!traverseForward && (idGeoKnotenB == idCurrentGeoKnotenA) && (idGeoArt == idTopKante))
                {
                    currentGeoKante = item;
                    traverseForward = false;
                    break;
                }
                else if(!traverseForward && (idGeoKnotenA == idCurrentGeoKnotenA) && (idGeoArt == idTopKante))
                {
                    currentGeoKante = item;
                    traverseForward = true;
                    break;
                }
            }
        }
    }

    // adjust the remaining distance if it is longer than the edge (due to double precision)
    if(remainingDistance > currentGeoKante->getFirstValueAtPath("GEO_Kante_Allg/GEO_Laenge/Wert").toDouble())
    {
        remainingDistance = currentGeoKante->getFirstValueAtPath("GEO_Kante_Allg/GEO_Laenge/Wert").toDouble();
    }

    // Determine the GEO_Punkt objects corresponding to the target edge (where the Punkt_Objekt will be placed).
    // KnotenA is always the node where the remaining distance starts, so if the traversing direction is reverse,
    // the nodes will be swapped.
    DomItem* punktA = NULL;
    DomItem* punktB = NULL;
    for (int i = 0; i < objectlist.count() && (!punktA || !punktB); ++i)
    {
        DomItem* item = objectlist.at(i);
        if(item->getName() == "GEO_Punkt")
        {
            QString idGeoKnoten = item->getFirstValueAtPath("ID_GEO_Knoten/Wert");
            QString idCurrentGeoKnotenA = currentGeoKante->getFirstValueAtPath("ID_GEO_Knoten_A/Wert");
            QString idCurrentGeoKnotenB = currentGeoKante->getFirstValueAtPath("ID_GEO_Knoten_B/Wert");
            if(idGeoKnoten == idCurrentGeoKnotenA && traverseForward)
            {
                punktA = item;
            }
            else if(idGeoKnoten == idCurrentGeoKnotenA && !traverseForward)
            {
                punktB = item;
            }
            else if(idGeoKnoten == idCurrentGeoKnotenB && traverseForward)
            {
                punktB = item;
            }
            else if(idGeoKnoten == idCurrentGeoKnotenB && !traverseForward)
            {
                punktA = item;
            }
        }
    }

    // Retrieve the coordinates of the GEO_Punkt objects and calculate the coordinates for the Punkt_Objekt
    if(punktA && punktB)
    {
        double ax = punktA->getFirstValueAtPath("GEO_Punkt_Allg/GK_X/Wert").toDouble();
        double ay = punktA->getFirstValueAtPath("GEO_Punkt_Allg/GK_Y/Wert").toDouble();
        double bx = punktB->getFirstValueAtPath("GEO_Punkt_Allg/GK_X/Wert").toDouble();
        double by = punktB->getFirstValueAtPath("GEO_Punkt_Allg/GK_Y/Wert").toDouble();

        double lengthFactor = remainingDistance / currentGeoKante->getFirstValueAtPath("GEO_Kante_Allg/GEO_Laenge/Wert").toDouble();
        double sideFactor = punktObjektTopKante->getFirstValueAtPath("Seitlicher_Abstand/Wert").toDouble() / currentGeoKante->getFirstValueAtPath("GEO_Kante_Allg/GEO_Laenge/Wert").toDouble();
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


QGraphicsItem* GraphicsSceneBuilder::placeBereichObjekt(DomItem* o, const QPen& pen)
{
    if(!document || !graphicsScene)
    {
        return NULL;
    }

    QList<QGraphicsItem*> graphicsitemlist;

    QList<DomItem*> teilbereichList = o->getChildItems("Bereich_Objekt_Teilbereich");

    for(int i = 0; i < teilbereichList.count(); ++i)
    {
        DomItem* teilbereichElement = teilbereichList.at(i);
        QString idTopKante = teilbereichElement->getFirstValueAtPath("ID_TOP_Kante/Wert");
        double begrenzungA = teilbereichElement->getFirstValueAtPath("Begrenzung_A/Wert").toDouble();
        double begrenzungB = teilbereichElement->getFirstValueAtPath("Begrenzung_B/Wert").toDouble();

        DomItem* topKante = document->getObjectById(idTopKante, PlanProDocument::End);
        DomItem* topKnotenA = document->getObjectById(topKante->getFirstValueAtPath("ID_TOP_Knoten_A/Wert"), PlanProDocument::End);
        QString idGeoKnoten = topKnotenA->getFirstValueAtPath("ID_GEO_Knoten/Wert");

        DomItem* currentGeoKante = NULL;
        bool traverseForward = true;

        // find the starting GEO_Kante
        QList<DomItem*> objectlist = document->getObjectList(PlanProDocument::End);
        for (int j = 0; j < objectlist.count(); ++j)
        {
            DomItem* item = objectlist.at(j);
            if(item->getName() == "GEO_Kante")
            {
                QString idGeoKnotenA = item->getFirstValueAtPath("ID_GEO_Knoten_A/Wert");
                QString idGeoKnotenB = item->getFirstValueAtPath("ID_GEO_Knoten_B/Wert");
                QString idGeoArt = item->getFirstValueAtPath("ID_GEO_Art/Wert");
                if((idGeoKnotenA == idGeoKnoten) && (idGeoArt == idTopKante))
                {
                    currentGeoKante = item;
                    traverseForward = true;
                    break;
                }
                else if((idGeoKnotenB == idGeoKnoten) && (idGeoArt == idTopKante))
                {
                    currentGeoKante = item;
                    traverseForward = false;
                    break;
                }
            }
        }

        if(!currentGeoKante)
        {
            return NULL;
        }

        // traverse along the GEO_Kante chain until the value of begrenzungA has reached zero
        // (afterwards, currentGeoKante will point to the GEO_Kante where the Bereich_Objekt starts)
        while(begrenzungA > currentGeoKante->getFirstValueAtPath("GEO_Kante_Allg/GEO_Laenge/Wert").toDouble() + TRAVERSE_DELTA)
        {
            begrenzungA -= currentGeoKante->getFirstValueAtPath("GEO_Kante_Allg/GEO_Laenge/Wert").toDouble();
            begrenzungB -= currentGeoKante->getFirstValueAtPath("GEO_Kante_Allg/GEO_Laenge/Wert").toDouble();
            for (int j = 0; j < objectlist.count(); ++j)
            {
                DomItem* item = objectlist.at(j);
                if(item->getName() == "GEO_Kante" && item != currentGeoKante)
                {
                    QString idGeoKnotenA = item->getFirstValueAtPath("ID_GEO_Knoten_A/Wert");
                    QString idGeoKnotenB = item->getFirstValueAtPath("ID_GEO_Knoten_B/Wert");
                    QString idGeoArt = item->getFirstValueAtPath("ID_GEO_Art/Wert");
                    QString idCurrentGeoKnotenA = currentGeoKante->getFirstValueAtPath("ID_GEO_Knoten_A/Wert");
                    QString idCurrentGeoKnotenB = currentGeoKante->getFirstValueAtPath("ID_GEO_Knoten_B/Wert");
                    if(traverseForward && (idGeoKnotenA == idCurrentGeoKnotenB) && (idGeoArt == idTopKante))
                    {
                        currentGeoKante = item;
                        traverseForward = true;
                        break;
                    }
                    else if(traverseForward && (idGeoKnotenB == idCurrentGeoKnotenB) && (idGeoArt == idTopKante))
                    {
                        currentGeoKante = item;
                        traverseForward = false;
                        break;
                    }
                    else if(!traverseForward && (idGeoKnotenB == idCurrentGeoKnotenA) && (idGeoArt == idTopKante))
                    {
                        currentGeoKante = item;
                        traverseForward = false;
                        break;
                    }
                    else if(!traverseForward && (idGeoKnotenA == idCurrentGeoKnotenA) && (idGeoArt == idTopKante))
                    {
                        currentGeoKante = item;
                        traverseForward = true;
                        break;
                    }
                }
            }
        }

        // adjust the remaining distance if it is longer than the edge (due to double precision)
        if(begrenzungA > currentGeoKante->getFirstValueAtPath("GEO_Kante_Allg/GEO_Laenge/Wert").toDouble())
        {
            begrenzungA = currentGeoKante->getFirstValueAtPath("GEO_Kante_Allg/GEO_Laenge/Wert").toDouble();
        }

        // Determine the GEO_Punkt objects corresponding to the starting edge (where the drawing of the Bereich_Objekt starts).
        // KnotenA is always the node where the remaining distance starts, so if the traversing direction is reverse,
        // the nodes will be swapped.
        DomItem* punktA = NULL;
        DomItem* punktB = NULL;
        for (int j = 0; j < objectlist.count() && (!punktA || !punktB); ++j)
        {
            DomItem* item = objectlist.at(j);
            if(item->getName() == "GEO_Punkt")
            {
                QString idGeoKnoten = item->getFirstValueAtPath("ID_GEO_Knoten/Wert");
                QString idCurrentGeoKnotenA = currentGeoKante->getFirstValueAtPath("ID_GEO_Knoten_A/Wert");
                QString idCurrentGeoKnotenB = currentGeoKante->getFirstValueAtPath("ID_GEO_Knoten_B/Wert");
                if(idGeoKnoten == idCurrentGeoKnotenA && traverseForward)
                {
                    punktA = item;
                }
                else if(idGeoKnoten == idCurrentGeoKnotenA && !traverseForward)
                {
                    punktB = item;
                }
                else if(idGeoKnoten == idCurrentGeoKnotenB && traverseForward)
                {
                    punktB = item;
                }
                else if(idGeoKnoten == idCurrentGeoKnotenB && !traverseForward)
                {
                    punktA = item;
                }
            }
        }

        double px = 0;
        double py = 0;

        // Retrieve the coordinates of the GEO_Punkt objects and calculate the starting coordinates for the Bereich_Objekt
        if(punktA && punktB)
        {
            double ax = punktA->getFirstValueAtPath("GEO_Punkt_Allg/GK_X/Wert").toDouble();
            double ay = punktA->getFirstValueAtPath("GEO_Punkt_Allg/GK_Y/Wert").toDouble();
            double bx = punktB->getFirstValueAtPath("GEO_Punkt_Allg/GK_X/Wert").toDouble();
            double by = punktB->getFirstValueAtPath("GEO_Punkt_Allg/GK_Y/Wert").toDouble();

            double lengthFactor = begrenzungA / currentGeoKante->getFirstValueAtPath("GEO_Kante_Allg/GEO_Laenge/Wert").toDouble();
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
        // collecting all intermediate edges
        while(begrenzungB > currentGeoKante->getFirstValueAtPath("GEO_Kante_Allg/GEO_Laenge/Wert").toDouble() + TRAVERSE_DELTA)
        {
            begrenzungB -= currentGeoKante->getFirstValueAtPath("GEO_Kante_Allg/GEO_Laenge/Wert").toDouble();
            for (int j = 0; j < objectlist.count(); ++j)
            {
                DomItem* item = objectlist.at(j);
                if(item->getName() == "GEO_Kante" && item != currentGeoKante)
                {
                    QString idGeoKnotenA = item->getFirstValueAtPath("ID_GEO_Knoten_A/Wert");
                    QString idGeoKnotenB = item->getFirstValueAtPath("ID_GEO_Knoten_B/Wert");
                    QString idGeoArt = item->getFirstValueAtPath("ID_GEO_Art/Wert");
                    QString idCurrentGeoKnotenA = currentGeoKante->getFirstValueAtPath("ID_GEO_Knoten_A/Wert");
                    QString idCurrentGeoKnotenB = currentGeoKante->getFirstValueAtPath("ID_GEO_Knoten_B/Wert");
                    if(traverseForward && (idGeoKnotenA == idCurrentGeoKnotenB) && (idGeoArt == idTopKante))
                    {
                        currentGeoKante = item;
                        traverseForward = true;
                        break;
                    }
                    else if(traverseForward && (idGeoKnotenB == idCurrentGeoKnotenB) && (idGeoArt == idTopKante))
                    {
                        currentGeoKante = item;
                        traverseForward = false;
                        break;
                    }
                    else if(!traverseForward && (idGeoKnotenB == idCurrentGeoKnotenA) && (idGeoArt == idTopKante))
                    {
                        currentGeoKante = item;
                        traverseForward = false;
                        break;
                    }
                    else if(!traverseForward && (idGeoKnotenA == idCurrentGeoKnotenA) && (idGeoArt == idTopKante))
                    {
                        currentGeoKante = item;
                        traverseForward = true;
                        break;
                    }
                }
            }

            // Determine the GEO_Punkt objects corresponding to the current (intermediate) edge.
            // KnotenA is always the node where the remaining distance starts, so if the traversing direction is reverse,
            // the nodes will be swapped.
            punktA = NULL;
            punktB = NULL;
            for (int j = 0; j < objectlist.count() && (!punktA || !punktB); ++j)
            {
                DomItem* item = objectlist.at(j);
                if(item->getName() == "GEO_Punkt")
                {
                    QString idGeoKnoten = item->getFirstValueAtPath("ID_GEO_Knoten/Wert");
                    QString idCurrentGeoKnotenA = currentGeoKante->getFirstValueAtPath("ID_GEO_Knoten_A/Wert");
                    QString idCurrentGeoKnotenB = currentGeoKante->getFirstValueAtPath("ID_GEO_Knoten_B/Wert");
                    if(idGeoKnoten == idCurrentGeoKnotenA && traverseForward)
                    {
                        punktA = item;
                    }
                    else if(idGeoKnoten == idCurrentGeoKnotenA && !traverseForward)
                    {
                        punktB = item;
                    }
                    else if(idGeoKnoten == idCurrentGeoKnotenB && traverseForward)
                    {
                        punktB = item;
                    }
                    else if(idGeoKnoten == idCurrentGeoKnotenB && !traverseForward)
                    {
                        punktA = item;
                    }
                }
            }

            // Retrieve the coordinates of the GEO_Punkt objects and draw the intermediate lines of the Bereich_Objekt
            if(punktA && punktB)
            {
                double ax = punktA->getFirstValueAtPath("GEO_Punkt_Allg/GK_X/Wert").toDouble();
                double ay = punktA->getFirstValueAtPath("GEO_Punkt_Allg/GK_Y/Wert").toDouble();

                QGraphicsItem* item = (QGraphicsItem*) graphicsScene->addLine(px, -py, ax, -ay, pen);
                graphicsitemlist.append(item);
                px = ax;
                py = ay;
            }
        }

        // adjust the remaining distance if it is longer than the edge (due to double precision)
        if(begrenzungB > currentGeoKante->getFirstValueAtPath("GEO_Kante_Allg/GEO_Laenge/Wert").toDouble())
        {
            begrenzungB = currentGeoKante->getFirstValueAtPath("GEO_Kante_Allg/GEO_Laenge/Wert").toDouble();
        }

        // Determine the GEO_Punkt objects corresponding to the ending edge (where the drawing of the Bereich_Objekt ends).
        // KnotenA is always the node where the remaining distance starts, so if the traversing direction is reverse,
        // the nodes will be swapped.
        punktA = NULL;
        punktB = NULL;
        for (int j = 0; j < objectlist.count() && (!punktA || !punktB); ++j)
        {
            DomItem* item = objectlist.at(j);
            if(item->getName() == "GEO_Punkt")
            {
                QString idGeoKnoten = item->getFirstValueAtPath("ID_GEO_Knoten/Wert");
                QString idCurrentGeoKnotenA = currentGeoKante->getFirstValueAtPath("ID_GEO_Knoten_A/Wert");
                QString idCurrentGeoKnotenB = currentGeoKante->getFirstValueAtPath("ID_GEO_Knoten_B/Wert");
                if(idGeoKnoten == idCurrentGeoKnotenA && traverseForward)
                {
                    punktA = item;
                }
                else if(idGeoKnoten == idCurrentGeoKnotenA && !traverseForward)
                {
                    punktB = item;
                }
                else if(idGeoKnoten == idCurrentGeoKnotenB && traverseForward)
                {
                    punktB = item;
                }
                else if(idGeoKnoten == idCurrentGeoKnotenB && !traverseForward)
                {
                    punktA = item;
                }
            }
        }

        // Retrieve the coordinates of the GEO_Punkt objects and calculate the ending coordinates for the Bereich_Objekt
        if(punktA && punktB)
        {
            double ax = punktA->getFirstValueAtPath("GEO_Punkt_Allg/GK_X/Wert").toDouble();
            double ay = punktA->getFirstValueAtPath("GEO_Punkt_Allg/GK_Y/Wert").toDouble();
            double bx = punktB->getFirstValueAtPath("GEO_Punkt_Allg/GK_X/Wert").toDouble();
            double by = punktB->getFirstValueAtPath("GEO_Punkt_Allg/GK_Y/Wert").toDouble();

            double lengthFactor = begrenzungB / currentGeoKante->getFirstValueAtPath("GEO_Kante_Allg/GEO_Laenge/Wert").toDouble();
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
    }

    QGraphicsItem* groupitem = graphicsScene->createItemGroup(graphicsitemlist);
    return groupitem;
}
