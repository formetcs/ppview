#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QProgressDialog>

#include "graphicsscenebuilder.h"
#include "planprodocument.h"
#include "graphicsscene.h"


const double TRAVERSE_DELTA = 0.1; // to avoid problems with double precision


GraphicsSceneBuilder::GraphicsSceneBuilder(PlanProDocument* doc, GraphicsScene* scene)
{
    document = doc;
    graphicsScene = scene;
}


bool GraphicsSceneBuilder::createGraphicsScene()
{
    if(!document || !graphicsScene)
    {
        return false;
    }

    graphicsScene->clear();
    QList<PlanProDocument::ObjectListItem> objectlist = document->getCombinedObjectList();
    QProgressDialog progress(QObject::tr("Creating track layout..."), QObject::tr("Abort"), 0, objectlist.count());
    progress.setModal(true);
    progress.setMinimumDuration(500);
    for (int i = 0; i < objectlist.count(); ++i)
    {
        progress.setValue(i);
        if(progress.wasCanceled())
        {
            graphicsScene->clear();
            return false;
        }
        PlanProDocument::ObjectListItem oli = objectlist.at(i);
        PlanProDocument::PlanningState state = oli.state;
        DomItem* item = (state == PlanProDocument::PlanningStateStart) ? oli.itemStart : oli.itemEnd;
        QPen combinedPen;
        QBrush combinedBrush;
        if(state == PlanProDocument::PlanningStateStart)
        {
            combinedPen = QPen(Qt::yellow);
            combinedBrush = QBrush(Qt::yellow);
        }
        else if(state == PlanProDocument::PlanningStateEnd)
        {
            combinedPen = QPen(Qt::red);
            combinedBrush = QBrush(Qt::red);
        }
        else
        {
            combinedPen = QPen(Qt::black);
            combinedBrush = QBrush(Qt::black);
        }
        QString name = item->getName();
        QString id = item->getFirstValueAtPath("Identitaet/Wert");
        if(name == "GEO_Punkt")
        {
            QString tooltip = "GEO_Punkt\n";
            tooltip += id;
            double x = item->getFirstValueAtPath("GEO_Punkt_Allg/GK_X/Wert").toDouble();
            double y = item->getFirstValueAtPath("GEO_Punkt_Allg/GK_Y/Wert").toDouble();
            if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
            {
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(x - 2, -y - 2, 4, 4, QPen(Qt::black), QBrush(Qt::white));
                gitem->setToolTip(tooltip);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                gitem->setZValue(-3);
            }
            if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
            {
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(x - 2, -y - 2, 4, 4, QPen(Qt::black), QBrush(Qt::white));
                gitem->setToolTip(tooltip);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                gitem->setZValue(-3);
            }
            QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(x - 2, -y - 2, 4, 4, combinedPen, combinedBrush);
            gitem->setToolTip(tooltip);
            gitem->setData(GRAPHICSITEM_TYPE, name);
            gitem->setData(GRAPHICSITEM_ID, id);
            gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
            gitem->setFlag(QGraphicsItem::ItemIsSelectable);
            gitem->setZValue(-3);
        }
        else if(name == "GEO_Knoten")
        {
            QString tooltip = "GEO_Knoten\n";
            tooltip += id;
            for (int j = 0; j < objectlist.count(); ++j)
            {
                PlanProDocument::ObjectListItem oli2 = objectlist.at(j);
                PlanProDocument::PlanningState state2 = oli2.state;
                DomItem* item2 = (state2 == PlanProDocument::PlanningStateStart) ? oli2.itemStart : oli2.itemEnd;
                if(item2->getName() == "GEO_Punkt" && item2->getFirstValueAtPath("ID_GEO_Knoten/Wert") == id)
                {
                    double px = item2->getFirstValueAtPath("GEO_Punkt_Allg/GK_X/Wert").toDouble();
                    double py = item2->getFirstValueAtPath("GEO_Punkt_Allg/GK_Y/Wert").toDouble();
                    if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
                    {
                        QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(px - 2, -py - 2, 4, 4, QPen(Qt::black), QBrush(Qt::black));
                        gitem->setToolTip(tooltip);
                        gitem->setData(GRAPHICSITEM_TYPE, name);
                        gitem->setData(GRAPHICSITEM_ID, id);
                        gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                        gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                        gitem->setZValue(-2);
                    }
                    if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
                    {
                        QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(px - 2, -py - 2, 4, 4, QPen(Qt::black), QBrush(Qt::black));
                        gitem->setToolTip(tooltip);
                        gitem->setData(GRAPHICSITEM_TYPE, name);
                        gitem->setData(GRAPHICSITEM_ID, id);
                        gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                        gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                        gitem->setZValue(-2);
                    }
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(px - 2, -py - 2, 4, 4, combinedPen, combinedBrush);
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                    gitem->setZValue(-2);
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
                PlanProDocument::ObjectListItem oli2 = objectlist.at(j);
                PlanProDocument::PlanningState state2 = oli2.state;
                DomItem* item2 = (state2 == PlanProDocument::PlanningStateStart) ? oli2.itemStart : oli2.itemEnd;
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
                if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(ax, -ay, bx, -by, QPen(Qt::black));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                    gitem->setZValue(-20);
                }
                if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(ax, -ay, bx, -by, QPen(Qt::black));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                    gitem->setZValue(-20);
                }
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(ax, -ay, bx, -by, combinedPen);
                gitem->setToolTip(tooltip);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                gitem->setZValue(-20);
            }
        }
        else if(name == "TOP_Knoten")
        {
            QString tooltip = "TOP_Knoten\n";
            tooltip += id;
            QString idknoten = item->getFirstValueAtPath("ID_GEO_Knoten/Wert");
            for (int j = 0; j < objectlist.count(); ++j)
            {
                PlanProDocument::ObjectListItem oli2 = objectlist.at(j);
                PlanProDocument::PlanningState state2 = oli2.state;
                DomItem* item2 = (state2 == PlanProDocument::PlanningStateStart) ? oli2.itemStart : oli2.itemEnd;
                if(item2->getName() == "GEO_Punkt" && item2->getFirstValueAtPath("ID_GEO_Knoten/Wert") == idknoten)
                {
                    double px = item2->getFirstValueAtPath("GEO_Punkt_Allg/GK_X/Wert").toDouble();
                    double py = item2->getFirstValueAtPath("GEO_Punkt_Allg/GK_Y/Wert").toDouble();
                    if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
                    {
                        QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(px - 2, -py - 2, 4, 4, QPen(Qt::red), QBrush(Qt::red));
                        gitem->setToolTip(tooltip);
                        gitem->setData(GRAPHICSITEM_TYPE, name);
                        gitem->setData(GRAPHICSITEM_ID, id);
                        gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                        gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                        gitem->setZValue(-1);
                    }
                    if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
                    {
                        QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(px - 2, -py - 2, 4, 4, QPen(Qt::red), QBrush(Qt::red));
                        gitem->setToolTip(tooltip);
                        gitem->setData(GRAPHICSITEM_TYPE, name);
                        gitem->setData(GRAPHICSITEM_ID, id);
                        gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                        gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                        gitem->setZValue(-1);
                    }
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(px - 2, -py - 2, 4, 4, combinedPen, combinedBrush);
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                    gitem->setZValue(-1);
                    break;
                }
            }
        }
        else if(name == "TOP_Kante")
        {
            QString tooltip = "TOP_Kante\n";
            tooltip += id;
            QList<QLineF> linelist;
            for (int j = 0; j < objectlist.count(); ++j)
            {
                PlanProDocument::ObjectListItem oli2 = objectlist.at(j);
                PlanProDocument::PlanningState state2 = oli2.state;
                DomItem* item2 = (state2 == PlanProDocument::PlanningStateStart) ? oli2.itemStart : oli2.itemEnd;
                if(item2->getName() == "GEO_Kante" && item2->getFirstValueAtPath("ID_GEO_Art/Wert") == id)
                {
                    QString idknotena = item2->getFirstValueAtPath("ID_GEO_Knoten_A/Wert");
                    QString idknotenb = item2->getFirstValueAtPath("ID_GEO_Knoten_B/Wert");
                    DomItem* punktA = NULL;
                    DomItem* punktB = NULL;
                    for (int k = 0; k < objectlist.count() && (!punktA || !punktB); ++k)
                    {
                        PlanProDocument::ObjectListItem oli3 = objectlist.at(k);
                        PlanProDocument::PlanningState state3 = oli3.state;
                        DomItem* item3 = (state3 == PlanProDocument::PlanningStateStart) ? oli3.itemStart : oli3.itemEnd;
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
                        QLineF segment(ax, -ay, bx, -by);
                        linelist.append(segment);
                    }
                }
            }
            if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
            {
                QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
                graphicsScene->addItem(groupitem);
                for (int j = 0; j < linelist.count(); ++j)
                {
                    QLineF segment = linelist.at(j);
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, QPen(Qt::red));
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    groupitem->addToGroup(gitem);
                }
                groupitem->setToolTip(tooltip);
                groupitem->setData(GRAPHICSITEM_TYPE, name);
                groupitem->setData(GRAPHICSITEM_ID, id);
                groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
                groupitem->setZValue(-10);
            }
            if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
            {
                QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
                graphicsScene->addItem(groupitem);
                for (int j = 0; j < linelist.count(); ++j)
                {
                    QLineF segment = linelist.at(j);
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, QPen(Qt::red));
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    groupitem->addToGroup(gitem);
                }
                groupitem->setToolTip(tooltip);
                groupitem->setData(GRAPHICSITEM_TYPE, name);
                groupitem->setData(GRAPHICSITEM_ID, id);
                groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
                groupitem->setZValue(-10);
            }
            QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
            graphicsScene->addItem(groupitem);
            for (int j = 0; j < linelist.count(); ++j)
            {
                QLineF segment = linelist.at(j);
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, combinedPen);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                groupitem->addToGroup(gitem);
            }
            groupitem->setToolTip(tooltip);
            groupitem->setData(GRAPHICSITEM_TYPE, name);
            groupitem->setData(GRAPHICSITEM_ID, id);
            groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
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

            QPointF point = calculatePunktObjekt(item);
            if(!point.isNull())
            {
                if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen(Qt::black), QBrush(Qt::yellow));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen(Qt::black), QBrush(Qt::yellow));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, combinedPen, combinedBrush);
                gitem->setToolTip(tooltip);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                gitem->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
        else if(name == "ZUB_Bereichsgrenze")
        {
            QString km = item->getFirstValueAtPath("Punkt_Objekt_Strecke/Strecke_Km/Wert");
            QString tooltip = "ZUB_Bereichsgrenze (Km: " + km + ")\n";
            tooltip += id;

            QPointF point = calculatePunktObjekt(item);
            if(!point.isNull())
            {
                if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen(Qt::red), QBrush(Qt::yellow));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen(Qt::red), QBrush(Qt::yellow));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, combinedPen, combinedBrush);
                gitem->setToolTip(tooltip);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                gitem->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
        else if(name == "Bahnsteig_Zugang")
        {
            QString km = item->getFirstValueAtPath("Punkt_Objekt_Strecke/Strecke_Km/Wert");
            QString tooltip = "Bahnsteig_Zugang (Km: " + km + ")\n";
            tooltip += id;

            QPointF point = calculatePunktObjekt(item);
            if(!point.isNull())
            {
                if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen(Qt::cyan), QBrush(Qt::cyan));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen(Qt::cyan), QBrush(Qt::cyan));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, combinedPen, combinedBrush);
                gitem->setToolTip(tooltip);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                gitem->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
        else if(name == "BUE_Anlage")
        {
            QString bezeichnunglageplankurztext = item->getFirstValueAtPath("Bezeichnung/Bezeichnung_Lageplan_Kurz/Wert");
            QString km = item->getFirstValueAtPath("Punkt_Objekt_Strecke/Strecke_Km/Wert");
            QString tooltip = "BUE_Anlage (Km: " + km + ")\n";
            tooltip += id + "\n";
            tooltip += bezeichnunglageplankurztext;

            QPointF point = calculatePunktObjekt(item);
            if(!point.isNull())
            {
                if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen(Qt::blue), QBrush(Qt::blue));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen(Qt::blue), QBrush(Qt::blue));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, combinedPen, combinedBrush);
                gitem->setToolTip(tooltip);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                gitem->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
        else if(name == "BUE_Gefahrraum_Eckpunkt")
        {
            QString km = item->getFirstValueAtPath("Punkt_Objekt_Strecke/Strecke_Km/Wert");
            QString tooltip = "BUE_Gefahrraum_Eckpunkt (Km: " + km + ")\n";
            tooltip += id;

            QPointF point = calculatePunktObjekt(item);
            if(!point.isNull())
            {
                if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen(Qt::blue), QBrush(Qt::blue));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen(Qt::blue), QBrush(Qt::blue));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, combinedPen, combinedBrush);
                gitem->setToolTip(tooltip);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                gitem->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
        else if(name == "BUE_Kante")
        {
            QString km = item->getFirstValueAtPath("Punkt_Objekt_Strecke/Strecke_Km/Wert");
            QString tooltip = "BUE_Kante (Km: " + km + ")\n";
            tooltip += id;

            QPointF point = calculatePunktObjekt(item);
            if(!point.isNull())
            {
                if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen(Qt::blue), QBrush(Qt::blue));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen(Qt::blue), QBrush(Qt::blue));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, combinedPen, combinedBrush);
                gitem->setToolTip(tooltip);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                gitem->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
        else if(name == "GFR_Tripelspiegel")
        {
            QString km = item->getFirstValueAtPath("Punkt_Objekt_Strecke/Strecke_Km/Wert");
            QString tooltip = "GFR_Tripelspiegel (Km: " + km + ")\n";
            tooltip += id;

            QPointF point = calculatePunktObjekt(item);
            if(!point.isNull())
            {
                if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen(Qt::blue), QBrush(Qt::blue));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen(Qt::blue), QBrush(Qt::blue));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, combinedPen, combinedBrush);
                gitem->setToolTip(tooltip);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                gitem->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
        else if(name == "Schrankenantrieb")
        {
            QString km = item->getFirstValueAtPath("Punkt_Objekt_Strecke/Strecke_Km/Wert");
            QString tooltip = "Schrankenantrieb (Km: " + km + ")\n";
            tooltip += id;

            QPointF point = calculatePunktObjekt(item);
            if(!point.isNull())
            {
                if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen(Qt::blue), QBrush(Qt::blue));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen(Qt::blue), QBrush(Qt::blue));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, combinedPen, combinedBrush);
                gitem->setToolTip(tooltip);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                gitem->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
        else if(name == "Sonstiger_Punkt")
        {
            QString km = item->getFirstValueAtPath("Punkt_Objekt_Strecke/Strecke_Km/Wert");
            QString tooltip = "Sonstiger_Punkt (Km: " + km + ")\n";
            tooltip += id;

            QPointF point = calculatePunktObjekt(item);
            if(!point.isNull())
            {
                if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen(Qt::magenta), QBrush(Qt::magenta));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen(Qt::magenta), QBrush(Qt::magenta));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, combinedPen, combinedBrush);
                gitem->setToolTip(tooltip);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                gitem->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
        else if(name == "Hoehenpunkt")
        {
            QString km = item->getFirstValueAtPath("Punkt_Objekt_Strecke/Strecke_Km/Wert");
            QString tooltip = "Hoehenpunkt (Km: " + km + ")\n";
            tooltip += id;

            QPointF point = calculatePunktObjekt(item);
            if(!point.isNull())
            {
                if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen(Qt::black), QBrush(Qt::red));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen(Qt::black), QBrush(Qt::red));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, combinedPen, combinedBrush);
                gitem->setToolTip(tooltip);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                gitem->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
        else if(name == "Technischer_Punkt")
        {
            QString km = item->getFirstValueAtPath("Punkt_Objekt_Strecke/Strecke_Km/Wert");
            QString tooltip = "Technischer_Punkt (Km: " + km + ")\n";
            tooltip += id;

            QPointF point = calculatePunktObjekt(item);
            if(!point.isNull())
            {
                if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen(Qt::black), QBrush(Qt::red));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen(Qt::black), QBrush(Qt::red));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, combinedPen, combinedBrush);
                gitem->setToolTip(tooltip);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                gitem->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
        else if(name == "Ueberhoehung")
        {
            QString km = item->getFirstValueAtPath("Punkt_Objekt_Strecke/Strecke_Km/Wert");
            QString tooltip = "Ueberhoehung (Km: " + km + ")\n";
            tooltip += id;

            QPointF point = calculatePunktObjekt(item);
            if(!point.isNull())
            {
                if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen(Qt::black), QBrush(Qt::red));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen(Qt::black), QBrush(Qt::red));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, combinedPen, combinedBrush);
                gitem->setToolTip(tooltip);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                gitem->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
        else if(name == "FMA_Element")
        {
            QString km = item->getFirstValueAtPath("Punkt_Objekt_Strecke/Strecke_Km/Wert");
            QString tooltip = "FMA_Element (Km: " + km + ")\n";
            tooltip += id;

            QPointF point = calculatePunktObjekt(item);
            if(!point.isNull())
            {
                if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen("brown"), QBrush("brown"));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen("brown"), QBrush("brown"));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, combinedPen, combinedBrush);
                gitem->setToolTip(tooltip);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                gitem->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
        else if(name == "FMA_Komponente")
        {
            QString bezeichnunglageplankurztext = item->getFirstValueAtPath("Bezeichnung/Bezeichnung_Lageplan_Kurz/Wert");
            QString km = item->getFirstValueAtPath("Punkt_Objekt_Strecke/Strecke_Km/Wert");
            QString tooltip = "FMA_Komponente (Km: " + km + ")\n";
            tooltip += id + "\n";
            tooltip += bezeichnunglageplankurztext;

            QPointF point = calculatePunktObjekt(item);
            if(!point.isNull())
            {
                if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen("brown"), QBrush("brown"));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen("brown"), QBrush("brown"));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, combinedPen, combinedBrush);
                gitem->setToolTip(tooltip);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                gitem->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
        else if(name == "Zugeinwirkung")
        {
            QString bezeichnunglageplankurztext = item->getFirstValueAtPath("Bezeichnung/Bezeichnung_Lageplan_Kurz/Wert");
            QString km = item->getFirstValueAtPath("Punkt_Objekt_Strecke/Strecke_Km/Wert");
            QString tooltip = "Zugeinwirkung (Km: " + km + ")\n";
            tooltip += id + "\n";
            tooltip += bezeichnunglageplankurztext;

            QPointF point = calculatePunktObjekt(item);
            if(!point.isNull())
            {
                if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen("brown"), QBrush("brown"));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen("brown"), QBrush("brown"));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, combinedPen, combinedBrush);
                gitem->setToolTip(tooltip);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                gitem->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
        else if(name == "PZB_Element")
        {
            QString km = item->getFirstValueAtPath("Punkt_Objekt_Strecke/Strecke_Km/Wert");
            QString tooltip = "PZB_Element (Km: " + km + ")\n";
            tooltip += id;

            QPointF point = calculatePunktObjekt(item);
            if(!point.isNull())
            {
                if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen(Qt::gray), QBrush(Qt::gray));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen(Qt::gray), QBrush(Qt::gray));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, combinedPen, combinedBrush);
                gitem->setToolTip(tooltip);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                gitem->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
        else if(name == "Signal")
        {
            QString bezeichnunglageplankurztext = item->getFirstValueAtPath("Bezeichnung/Bezeichnung_Lageplan_Kurz/Wert");
            QString km = item->getFirstValueAtPath("Punkt_Objekt_Strecke/Strecke_Km/Wert");
            QString tooltip = "Signal (Km: " + km + ")\n";
            tooltip += id + "\n";
            tooltip += bezeichnunglageplankurztext;

            QPointF point = calculatePunktObjekt(item);
            if(!point.isNull())
            {
                if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen(Qt::green), QBrush(Qt::green));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen(Qt::green), QBrush(Qt::green));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, combinedPen, combinedBrush);
                gitem->setToolTip(tooltip);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                gitem->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
        else if(name == "Signal_Befestigung")
        {
            QString km = item->getFirstValueAtPath("Punkt_Objekt_Strecke/Strecke_Km/Wert");
            QString tooltip = "Signal_Befestigung (Km: " + km + ")\n";
            tooltip += id;

            QPointF point = calculatePunktObjekt(item);
            if(!point.isNull())
            {
                if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen(Qt::black), QBrush("greenyellow"));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen(Qt::black), QBrush("greenyellow"));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, combinedPen, combinedBrush);
                gitem->setToolTip(tooltip);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                gitem->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
        else if(name == "Gleis_Abschluss")
        {
            QString km = item->getFirstValueAtPath("Punkt_Objekt_Strecke/Strecke_Km/Wert");
            QString tooltip = "Gleis_Abschluss (Km: " + km + ")\n";
            tooltip += id;

            QPointF point = calculatePunktObjekt(item);
            if(!point.isNull())
            {
                if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen("coral"), QBrush("coral"));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen("coral"), QBrush("coral"));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, combinedPen, combinedBrush);
                gitem->setToolTip(tooltip);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                gitem->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
        else if(name == "W_Kr_Gsp_Komponente")
        {
            QString km = item->getFirstValueAtPath("Punkt_Objekt_Strecke/Strecke_Km/Wert");
            QString tooltip = "W_Kr_Gsp_Komponente (Km: " + km + ")\n";
            tooltip += id;

            QPointF point = calculatePunktObjekt(item);
            if(!point.isNull())
            {
                if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen("coral"), QBrush("coral"));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
                {
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, QPen("coral"), QBrush("coral"));
                    gitem->setToolTip(tooltip);
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    gitem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addEllipse(point.x() - 2, -point.y() - 2, 4, 4, combinedPen, combinedBrush);
                gitem->setToolTip(tooltip);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                gitem->setFlag(QGraphicsItem::ItemIsSelectable);
            }
        }
        else if(name == "ZUB_Streckeneigenschaft")
        {
            QString tooltip = "ZUB_Streckeneigenschaft\n";
            tooltip += id;

            QList<QLineF> linelist = calculateBereichObjekt(item);
            if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
            {
                QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
                graphicsScene->addItem(groupitem);
                for (int j = 0; j < linelist.count(); ++j)
                {
                    QLineF segment = linelist.at(j);
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, QPen(Qt::yellow));
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    groupitem->addToGroup(gitem);
                }
                groupitem->setToolTip(tooltip);
                groupitem->setData(GRAPHICSITEM_TYPE, name);
                groupitem->setData(GRAPHICSITEM_ID, id);
                groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
                groupitem->setZValue(-5);
            }
            if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
            {
                QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
                graphicsScene->addItem(groupitem);
                for (int j = 0; j < linelist.count(); ++j)
                {
                    QLineF segment = linelist.at(j);
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, QPen(Qt::yellow));
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    groupitem->addToGroup(gitem);
                }
                groupitem->setToolTip(tooltip);
                groupitem->setData(GRAPHICSITEM_TYPE, name);
                groupitem->setData(GRAPHICSITEM_ID, id);
                groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
                groupitem->setZValue(-5);
            }
            QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
            graphicsScene->addItem(groupitem);
            for (int j = 0; j < linelist.count(); ++j)
            {
                QLineF segment = linelist.at(j);
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, combinedPen);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                groupitem->addToGroup(gitem);
            }
            groupitem->setToolTip(tooltip);
            groupitem->setData(GRAPHICSITEM_TYPE, name);
            groupitem->setData(GRAPHICSITEM_ID, id);
            groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
            groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
            groupitem->setZValue(-5);
        }
        else if(name == "Stell_Bereich")
        {
            QString tooltip = "Stell_Bereich\n";
            tooltip += id;

            QList<QLineF> linelist = calculateBereichObjekt(item);
            if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
            {
                QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
                graphicsScene->addItem(groupitem);
                for (int j = 0; j < linelist.count(); ++j)
                {
                    QLineF segment = linelist.at(j);
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, QPen("orange"));
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    groupitem->addToGroup(gitem);
                }
                groupitem->setToolTip(tooltip);
                groupitem->setData(GRAPHICSITEM_TYPE, name);
                groupitem->setData(GRAPHICSITEM_ID, id);
                groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
                groupitem->setZValue(-5);
            }
            if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
            {
                QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
                graphicsScene->addItem(groupitem);
                for (int j = 0; j < linelist.count(); ++j)
                {
                    QLineF segment = linelist.at(j);
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, QPen("orange"));
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    groupitem->addToGroup(gitem);
                }
                groupitem->setToolTip(tooltip);
                groupitem->setData(GRAPHICSITEM_TYPE, name);
                groupitem->setData(GRAPHICSITEM_ID, id);
                groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
                groupitem->setZValue(-5);
            }
            QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
            graphicsScene->addItem(groupitem);
            for (int j = 0; j < linelist.count(); ++j)
            {
                QLineF segment = linelist.at(j);
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, combinedPen);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                groupitem->addToGroup(gitem);
            }
            groupitem->setToolTip(tooltip);
            groupitem->setData(GRAPHICSITEM_TYPE, name);
            groupitem->setData(GRAPHICSITEM_ID, id);
            groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
            groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
            groupitem->setZValue(-5);
        }
        else if(name == "Bahnsteig_Dach")
        {
            QString tooltip = "Bahnsteig_Dach\n";
            tooltip += id;

            QList<QLineF> linelist = calculateBereichObjekt(item);
            if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
            {
                QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
                graphicsScene->addItem(groupitem);
                for (int j = 0; j < linelist.count(); ++j)
                {
                    QLineF segment = linelist.at(j);
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, QPen(Qt::cyan));
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    groupitem->addToGroup(gitem);
                }
                groupitem->setToolTip(tooltip);
                groupitem->setData(GRAPHICSITEM_TYPE, name);
                groupitem->setData(GRAPHICSITEM_ID, id);
                groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
                groupitem->setZValue(-5);
            }
            if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
            {
                QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
                graphicsScene->addItem(groupitem);
                for (int j = 0; j < linelist.count(); ++j)
                {
                    QLineF segment = linelist.at(j);
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, QPen(Qt::cyan));
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    groupitem->addToGroup(gitem);
                }
                groupitem->setToolTip(tooltip);
                groupitem->setData(GRAPHICSITEM_TYPE, name);
                groupitem->setData(GRAPHICSITEM_ID, id);
                groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
                groupitem->setZValue(-5);
            }
            QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
            graphicsScene->addItem(groupitem);
            for (int j = 0; j < linelist.count(); ++j)
            {
                QLineF segment = linelist.at(j);
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, combinedPen);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                groupitem->addToGroup(gitem);
            }
            groupitem->setToolTip(tooltip);
            groupitem->setData(GRAPHICSITEM_TYPE, name);
            groupitem->setData(GRAPHICSITEM_ID, id);
            groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
            groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
            groupitem->setZValue(-5);
        }
        else if(name == "Bahnsteig_Kante")
        {
            QString tooltip = "Bahnsteig_Kante\n";
            tooltip += id;

            QList<QLineF> linelist = calculateBereichObjekt(item);
            if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
            {
                QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
                graphicsScene->addItem(groupitem);
                for (int j = 0; j < linelist.count(); ++j)
                {
                    QLineF segment = linelist.at(j);
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, QPen(Qt::cyan));
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    groupitem->addToGroup(gitem);
                }
                groupitem->setToolTip(tooltip);
                groupitem->setData(GRAPHICSITEM_TYPE, name);
                groupitem->setData(GRAPHICSITEM_ID, id);
                groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
                groupitem->setZValue(-5);
            }
            if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
            {
                QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
                graphicsScene->addItem(groupitem);
                for (int j = 0; j < linelist.count(); ++j)
                {
                    QLineF segment = linelist.at(j);
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, QPen(Qt::cyan));
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    groupitem->addToGroup(gitem);
                }
                groupitem->setToolTip(tooltip);
                groupitem->setData(GRAPHICSITEM_TYPE, name);
                groupitem->setData(GRAPHICSITEM_ID, id);
                groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
                groupitem->setZValue(-5);
            }
            QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
            graphicsScene->addItem(groupitem);
            for (int j = 0; j < linelist.count(); ++j)
            {
                QLineF segment = linelist.at(j);
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, combinedPen);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                groupitem->addToGroup(gitem);
            }
            groupitem->setToolTip(tooltip);
            groupitem->setData(GRAPHICSITEM_TYPE, name);
            groupitem->setData(GRAPHICSITEM_ID, id);
            groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
            groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
            groupitem->setZValue(-5);
        }
        else if(name == "BUE_Gleisbezogener_Gefahrraum")
        {
            QString tooltip = "BUE_Gleisbezogener_Gefahrraum\n";
            tooltip += id;

            QList<QLineF> linelist = calculateBereichObjekt(item);
            if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
            {
                QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
                graphicsScene->addItem(groupitem);
                for (int j = 0; j < linelist.count(); ++j)
                {
                    QLineF segment = linelist.at(j);
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, QPen(Qt::blue));
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    groupitem->addToGroup(gitem);
                }
                groupitem->setToolTip(tooltip);
                groupitem->setData(GRAPHICSITEM_TYPE, name);
                groupitem->setData(GRAPHICSITEM_ID, id);
                groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
                groupitem->setZValue(-5);
            }
            if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
            {
                QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
                graphicsScene->addItem(groupitem);
                for (int j = 0; j < linelist.count(); ++j)
                {
                    QLineF segment = linelist.at(j);
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, QPen(Qt::blue));
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    groupitem->addToGroup(gitem);
                }
                groupitem->setToolTip(tooltip);
                groupitem->setData(GRAPHICSITEM_TYPE, name);
                groupitem->setData(GRAPHICSITEM_ID, id);
                groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
                groupitem->setZValue(-5);
            }
            QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
            graphicsScene->addItem(groupitem);
            for (int j = 0; j < linelist.count(); ++j)
            {
                QLineF segment = linelist.at(j);
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, combinedPen);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                groupitem->addToGroup(gitem);
            }
            groupitem->setToolTip(tooltip);
            groupitem->setData(GRAPHICSITEM_TYPE, name);
            groupitem->setData(GRAPHICSITEM_ID, id);
            groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
            groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
            groupitem->setZValue(-5);
        }
        else if(name == "Fstr_Fahrweg")
        {
            QString tooltip = "Fstr_Fahrweg\n";
            tooltip += id;

            QList<QLineF> linelist = calculateBereichObjekt(item);
            if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
            {
                QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
                graphicsScene->addItem(groupitem);
                for (int j = 0; j < linelist.count(); ++j)
                {
                    QLineF segment = linelist.at(j);
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, QPen(Qt::magenta));
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    groupitem->addToGroup(gitem);
                }
                groupitem->setToolTip(tooltip);
                groupitem->setData(GRAPHICSITEM_TYPE, name);
                groupitem->setData(GRAPHICSITEM_ID, id);
                groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
                groupitem->setZValue(-5);
            }
            if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
            {
                QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
                graphicsScene->addItem(groupitem);
                for (int j = 0; j < linelist.count(); ++j)
                {
                    QLineF segment = linelist.at(j);
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, QPen(Qt::magenta));
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    groupitem->addToGroup(gitem);
                }
                groupitem->setToolTip(tooltip);
                groupitem->setData(GRAPHICSITEM_TYPE, name);
                groupitem->setData(GRAPHICSITEM_ID, id);
                groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
                groupitem->setZValue(-5);
            }
            QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
            graphicsScene->addItem(groupitem);
            for (int j = 0; j < linelist.count(); ++j)
            {
                QLineF segment = linelist.at(j);
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, combinedPen);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                groupitem->addToGroup(gitem);
            }
            groupitem->setToolTip(tooltip);
            groupitem->setData(GRAPHICSITEM_TYPE, name);
            groupitem->setData(GRAPHICSITEM_ID, id);
            groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
            groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
            groupitem->setZValue(-5);
        }
        else if(name == "Geschwindigkeitsprofil")
        {
            QString tooltip = "Geschwindigkeitsprofil\n";
            tooltip += id;

            QList<QLineF> linelist = calculateBereichObjekt(item);
            if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
            {
                QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
                graphicsScene->addItem(groupitem);
                for (int j = 0; j < linelist.count(); ++j)
                {
                    QLineF segment = linelist.at(j);
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, QPen(Qt::red));
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    groupitem->addToGroup(gitem);
                }
                groupitem->setToolTip(tooltip);
                groupitem->setData(GRAPHICSITEM_TYPE, name);
                groupitem->setData(GRAPHICSITEM_ID, id);
                groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
                groupitem->setZValue(-5);
            }
            if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
            {
                QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
                graphicsScene->addItem(groupitem);
                for (int j = 0; j < linelist.count(); ++j)
                {
                    QLineF segment = linelist.at(j);
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, QPen(Qt::red));
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    groupitem->addToGroup(gitem);
                }
                groupitem->setToolTip(tooltip);
                groupitem->setData(GRAPHICSITEM_TYPE, name);
                groupitem->setData(GRAPHICSITEM_ID, id);
                groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
                groupitem->setZValue(-5);
            }
            QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
            graphicsScene->addItem(groupitem);
            for (int j = 0; j < linelist.count(); ++j)
            {
                QLineF segment = linelist.at(j);
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, combinedPen);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                groupitem->addToGroup(gitem);
            }
            groupitem->setToolTip(tooltip);
            groupitem->setData(GRAPHICSITEM_TYPE, name);
            groupitem->setData(GRAPHICSITEM_ID, id);
            groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
            groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
            groupitem->setZValue(-5);
        }
        else if(name == "Strecke")
        {
            QString tooltip = "Strecke\n";
            tooltip += id;

            QList<QLineF> linelist = calculateBereichObjekt(item);
            if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
            {
                QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
                graphicsScene->addItem(groupitem);
                for (int j = 0; j < linelist.count(); ++j)
                {
                    QLineF segment = linelist.at(j);
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, QPen(Qt::red));
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    groupitem->addToGroup(gitem);
                }
                groupitem->setToolTip(tooltip);
                groupitem->setData(GRAPHICSITEM_TYPE, name);
                groupitem->setData(GRAPHICSITEM_ID, id);
                groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
                groupitem->setZValue(-5);
            }
            if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
            {
                QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
                graphicsScene->addItem(groupitem);
                for (int j = 0; j < linelist.count(); ++j)
                {
                    QLineF segment = linelist.at(j);
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, QPen(Qt::red));
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    groupitem->addToGroup(gitem);
                }
                groupitem->setToolTip(tooltip);
                groupitem->setData(GRAPHICSITEM_TYPE, name);
                groupitem->setData(GRAPHICSITEM_ID, id);
                groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
                groupitem->setZValue(-5);
            }
            QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
            graphicsScene->addItem(groupitem);
            for (int j = 0; j < linelist.count(); ++j)
            {
                QLineF segment = linelist.at(j);
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, combinedPen);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                groupitem->addToGroup(gitem);
            }
            groupitem->setToolTip(tooltip);
            groupitem->setData(GRAPHICSITEM_TYPE, name);
            groupitem->setData(GRAPHICSITEM_ID, id);
            groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
            groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
            groupitem->setZValue(-5);
        }
        else if(name == "Technischer_Bereich")
        {
            QString tooltip = "Technischer_Bereich\n";
            tooltip += id;

            QList<QLineF> linelist = calculateBereichObjekt(item);
            if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
            {
                QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
                graphicsScene->addItem(groupitem);
                for (int j = 0; j < linelist.count(); ++j)
                {
                    QLineF segment = linelist.at(j);
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, QPen(Qt::red));
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    groupitem->addToGroup(gitem);
                }
                groupitem->setToolTip(tooltip);
                groupitem->setData(GRAPHICSITEM_TYPE, name);
                groupitem->setData(GRAPHICSITEM_ID, id);
                groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
                groupitem->setZValue(-5);
            }
            if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
            {
                QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
                graphicsScene->addItem(groupitem);
                for (int j = 0; j < linelist.count(); ++j)
                {
                    QLineF segment = linelist.at(j);
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, QPen(Qt::red));
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    groupitem->addToGroup(gitem);
                }
                groupitem->setToolTip(tooltip);
                groupitem->setData(GRAPHICSITEM_TYPE, name);
                groupitem->setData(GRAPHICSITEM_ID, id);
                groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
                groupitem->setZValue(-5);
            }
            QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
            graphicsScene->addItem(groupitem);
            for (int j = 0; j < linelist.count(); ++j)
            {
                QLineF segment = linelist.at(j);
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, combinedPen);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                groupitem->addToGroup(gitem);
            }
            groupitem->setToolTip(tooltip);
            groupitem->setData(GRAPHICSITEM_TYPE, name);
            groupitem->setData(GRAPHICSITEM_ID, id);
            groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
            groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
            groupitem->setZValue(-5);
        }
        else if(name == "Gleis_Abschnitt")
        {
            QString tooltip = "Gleis_Abschnitt\n";
            tooltip += id;

            QList<QLineF> linelist = calculateBereichObjekt(item);
            if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
            {
                QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
                graphicsScene->addItem(groupitem);
                for (int j = 0; j < linelist.count(); ++j)
                {
                    QLineF segment = linelist.at(j);
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, QPen("olive"));
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    groupitem->addToGroup(gitem);
                }
                groupitem->setToolTip(tooltip);
                groupitem->setData(GRAPHICSITEM_TYPE, name);
                groupitem->setData(GRAPHICSITEM_ID, id);
                groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
                groupitem->setZValue(-5);
            }
            if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
            {
                QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
                graphicsScene->addItem(groupitem);
                for (int j = 0; j < linelist.count(); ++j)
                {
                    QLineF segment = linelist.at(j);
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, QPen("olive"));
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    groupitem->addToGroup(gitem);
                }
                groupitem->setToolTip(tooltip);
                groupitem->setData(GRAPHICSITEM_TYPE, name);
                groupitem->setData(GRAPHICSITEM_ID, id);
                groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
                groupitem->setZValue(-5);
            }
            QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
            graphicsScene->addItem(groupitem);
            for (int j = 0; j < linelist.count(); ++j)
            {
                QLineF segment = linelist.at(j);
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, combinedPen);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                groupitem->addToGroup(gitem);
            }
            groupitem->setToolTip(tooltip);
            groupitem->setData(GRAPHICSITEM_TYPE, name);
            groupitem->setData(GRAPHICSITEM_ID, id);
            groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
            groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
            groupitem->setZValue(-5);
        }
        else if(name == "Gleis_Art")
        {
            QString tooltip = "Gleis_Art\n";
            tooltip += id;

            QList<QLineF> linelist = calculateBereichObjekt(item);
            if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
            {
                QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
                graphicsScene->addItem(groupitem);
                for (int j = 0; j < linelist.count(); ++j)
                {
                    QLineF segment = linelist.at(j);
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, QPen("olive"));
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    groupitem->addToGroup(gitem);
                }
                groupitem->setToolTip(tooltip);
                groupitem->setData(GRAPHICSITEM_TYPE, name);
                groupitem->setData(GRAPHICSITEM_ID, id);
                groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
                groupitem->setZValue(-5);
            }
            if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
            {
                QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
                graphicsScene->addItem(groupitem);
                for (int j = 0; j < linelist.count(); ++j)
                {
                    QLineF segment = linelist.at(j);
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, QPen("olive"));
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    groupitem->addToGroup(gitem);
                }
                groupitem->setToolTip(tooltip);
                groupitem->setData(GRAPHICSITEM_TYPE, name);
                groupitem->setData(GRAPHICSITEM_ID, id);
                groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
                groupitem->setZValue(-5);
            }
            QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
            graphicsScene->addItem(groupitem);
            for (int j = 0; j < linelist.count(); ++j)
            {
                QLineF segment = linelist.at(j);
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, combinedPen);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                groupitem->addToGroup(gitem);
            }
            groupitem->setToolTip(tooltip);
            groupitem->setData(GRAPHICSITEM_TYPE, name);
            groupitem->setData(GRAPHICSITEM_ID, id);
            groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
            groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
            groupitem->setZValue(-5);
        }
        else if(name == "Gleis_Baubereich")
        {
            QString tooltip = "Gleis_Baubereich\n";
            tooltip += id;

            QList<QLineF> linelist = calculateBereichObjekt(item);
            if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
            {
                QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
                graphicsScene->addItem(groupitem);
                for (int j = 0; j < linelist.count(); ++j)
                {
                    QLineF segment = linelist.at(j);
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, QPen("olive"));
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    groupitem->addToGroup(gitem);
                }
                groupitem->setToolTip(tooltip);
                groupitem->setData(GRAPHICSITEM_TYPE, name);
                groupitem->setData(GRAPHICSITEM_ID, id);
                groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
                groupitem->setZValue(-5);
            }
            if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
            {
                QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
                graphicsScene->addItem(groupitem);
                for (int j = 0; j < linelist.count(); ++j)
                {
                    QLineF segment = linelist.at(j);
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, QPen("olive"));
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    groupitem->addToGroup(gitem);
                }
                groupitem->setToolTip(tooltip);
                groupitem->setData(GRAPHICSITEM_TYPE, name);
                groupitem->setData(GRAPHICSITEM_ID, id);
                groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
                groupitem->setZValue(-5);
            }
            QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
            graphicsScene->addItem(groupitem);
            for (int j = 0; j < linelist.count(); ++j)
            {
                QLineF segment = linelist.at(j);
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, combinedPen);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                groupitem->addToGroup(gitem);
            }
            groupitem->setToolTip(tooltip);
            groupitem->setData(GRAPHICSITEM_TYPE, name);
            groupitem->setData(GRAPHICSITEM_ID, id);
            groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
            groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
            groupitem->setZValue(-5);
        }
        else if(name == "Gleis_Bezeichnung")
        {
            QString tooltip = "Gleis_Bezeichnung\n";
            tooltip += id;

            QList<QLineF> linelist = calculateBereichObjekt(item);
            if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
            {
                QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
                graphicsScene->addItem(groupitem);
                for (int j = 0; j < linelist.count(); ++j)
                {
                    QLineF segment = linelist.at(j);
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, QPen("olive"));
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    groupitem->addToGroup(gitem);
                }
                groupitem->setToolTip(tooltip);
                groupitem->setData(GRAPHICSITEM_TYPE, name);
                groupitem->setData(GRAPHICSITEM_ID, id);
                groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
                groupitem->setZValue(-5);
            }
            if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
            {
                QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
                graphicsScene->addItem(groupitem);
                for (int j = 0; j < linelist.count(); ++j)
                {
                    QLineF segment = linelist.at(j);
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, QPen("olive"));
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    groupitem->addToGroup(gitem);
                }
                groupitem->setToolTip(tooltip);
                groupitem->setData(GRAPHICSITEM_TYPE, name);
                groupitem->setData(GRAPHICSITEM_ID, id);
                groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
                groupitem->setZValue(-5);
            }
            QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
            graphicsScene->addItem(groupitem);
            for (int j = 0; j < linelist.count(); ++j)
            {
                QLineF segment = linelist.at(j);
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, combinedPen);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                groupitem->addToGroup(gitem);
            }
            groupitem->setToolTip(tooltip);
            groupitem->setData(GRAPHICSITEM_TYPE, name);
            groupitem->setData(GRAPHICSITEM_ID, id);
            groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
            groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
            groupitem->setZValue(-5);
        }
        else if(name == "Gleis_Fahrbahn")
        {
            QString tooltip = "Gleis_Fahrbahn\n";
            tooltip += id;

            QList<QLineF> linelist = calculateBereichObjekt(item);
            if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
            {
                QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
                graphicsScene->addItem(groupitem);
                for (int j = 0; j < linelist.count(); ++j)
                {
                    QLineF segment = linelist.at(j);
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, QPen("olive"));
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    groupitem->addToGroup(gitem);
                }
                groupitem->setToolTip(tooltip);
                groupitem->setData(GRAPHICSITEM_TYPE, name);
                groupitem->setData(GRAPHICSITEM_ID, id);
                groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
                groupitem->setZValue(-5);
            }
            if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
            {
                QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
                graphicsScene->addItem(groupitem);
                for (int j = 0; j < linelist.count(); ++j)
                {
                    QLineF segment = linelist.at(j);
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, QPen("olive"));
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    groupitem->addToGroup(gitem);
                }
                groupitem->setToolTip(tooltip);
                groupitem->setData(GRAPHICSITEM_TYPE, name);
                groupitem->setData(GRAPHICSITEM_ID, id);
                groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
                groupitem->setZValue(-5);
            }
            QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
            graphicsScene->addItem(groupitem);
            for (int j = 0; j < linelist.count(); ++j)
            {
                QLineF segment = linelist.at(j);
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, combinedPen);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                groupitem->addToGroup(gitem);
            }
            groupitem->setToolTip(tooltip);
            groupitem->setData(GRAPHICSITEM_TYPE, name);
            groupitem->setData(GRAPHICSITEM_ID, id);
            groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
            groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
            groupitem->setZValue(-5);
        }
        else if(name == "Gleis_Lichtraum")
        {
            QString tooltip = "Gleis_Lichtraum\n";
            tooltip += id;

            QList<QLineF> linelist = calculateBereichObjekt(item);
            if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
            {
                QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
                graphicsScene->addItem(groupitem);
                for (int j = 0; j < linelist.count(); ++j)
                {
                    QLineF segment = linelist.at(j);
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, QPen("olive"));
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    groupitem->addToGroup(gitem);
                }
                groupitem->setToolTip(tooltip);
                groupitem->setData(GRAPHICSITEM_TYPE, name);
                groupitem->setData(GRAPHICSITEM_ID, id);
                groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
                groupitem->setZValue(-5);
            }
            if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
            {
                QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
                graphicsScene->addItem(groupitem);
                for (int j = 0; j < linelist.count(); ++j)
                {
                    QLineF segment = linelist.at(j);
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, QPen("olive"));
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    groupitem->addToGroup(gitem);
                }
                groupitem->setToolTip(tooltip);
                groupitem->setData(GRAPHICSITEM_TYPE, name);
                groupitem->setData(GRAPHICSITEM_ID, id);
                groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
                groupitem->setZValue(-5);
            }
            QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
            graphicsScene->addItem(groupitem);
            for (int j = 0; j < linelist.count(); ++j)
            {
                QLineF segment = linelist.at(j);
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, combinedPen);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                groupitem->addToGroup(gitem);
            }
            groupitem->setToolTip(tooltip);
            groupitem->setData(GRAPHICSITEM_TYPE, name);
            groupitem->setData(GRAPHICSITEM_ID, id);
            groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
            groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
            groupitem->setZValue(-5);
        }
        else if(name == "Gleis_Schaltgruppe")
        {
            QString tooltip = "Gleis_Schaltgruppe\n";
            tooltip += id;

            QList<QLineF> linelist = calculateBereichObjekt(item);
            if(state == PlanProDocument::PlanningStateStart || state == PlanProDocument::PlanningStateBoth)
            {
                QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
                graphicsScene->addItem(groupitem);
                for (int j = 0; j < linelist.count(); ++j)
                {
                    QLineF segment = linelist.at(j);
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, QPen("olive"));
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                    groupitem->addToGroup(gitem);
                }
                groupitem->setToolTip(tooltip);
                groupitem->setData(GRAPHICSITEM_TYPE, name);
                groupitem->setData(GRAPHICSITEM_ID, id);
                groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateStart);
                groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
                groupitem->setZValue(-5);
            }
            if(state == PlanProDocument::PlanningStateEnd || state == PlanProDocument::PlanningStateBoth)
            {
                QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
                graphicsScene->addItem(groupitem);
                for (int j = 0; j < linelist.count(); ++j)
                {
                    QLineF segment = linelist.at(j);
                    QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, QPen("olive"));
                    gitem->setData(GRAPHICSITEM_TYPE, name);
                    gitem->setData(GRAPHICSITEM_ID, id);
                    gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                    groupitem->addToGroup(gitem);
                }
                groupitem->setToolTip(tooltip);
                groupitem->setData(GRAPHICSITEM_TYPE, name);
                groupitem->setData(GRAPHICSITEM_ID, id);
                groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateEnd);
                groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
                groupitem->setZValue(-5);
            }
            QGraphicsItemGroup* groupitem = new QGraphicsItemGroup();
            graphicsScene->addItem(groupitem);
            for (int j = 0; j < linelist.count(); ++j)
            {
                QLineF segment = linelist.at(j);
                QGraphicsItem* gitem = (QGraphicsItem*) graphicsScene->addLine(segment, combinedPen);
                gitem->setData(GRAPHICSITEM_TYPE, name);
                gitem->setData(GRAPHICSITEM_ID, id);
                gitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
                groupitem->addToGroup(gitem);
            }
            groupitem->setToolTip(tooltip);
            groupitem->setData(GRAPHICSITEM_TYPE, name);
            groupitem->setData(GRAPHICSITEM_ID, id);
            groupitem->setData(GRAPHICSITEM_VIEWMODE, MainWindow::ViewModeStateComparison);
            groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
            groupitem->setZValue(-5);
        }
    }
    progress.setValue(objectlist.count());
    graphicsScene->setSceneRect(graphicsScene->itemsBoundingRect());
    return true;
}


QPointF GraphicsSceneBuilder::calculatePunktObjekt(const DomItem* o)
{
    if(!document || !graphicsScene)
    {
        return QPointF();
    }

    const DomItem* lstZustandItem = o->parent()->parent();
    QString lstZustandItemName = lstZustandItem->getName();
    PlanProDocument::PlanningState currentItemState = PlanProDocument::PlanningStateEnd;
    if(lstZustandItemName == "LST_Zustand_Start")
    {
        currentItemState = PlanProDocument::PlanningStateStart;
    }

    // if the Punkt_Objekt has more than one referencing TOP_Kante, we take only the first
    // (the resulting point must be the same for all edges)
    const DomItem* punktObjektTopKante = o->getFirstChildItem("Punkt_Objekt_TOP_Kante");
    double remainingDistance = punktObjektTopKante->getFirstValueAtPath("Abstand/Wert").toDouble();

    DomItem* topKante = document->getObjectById(punktObjektTopKante->getFirstValueAtPath("ID_TOP_Kante/Wert"), currentItemState);
    QString idTopKante = topKante->getFirstValueAtPath("Identitaet/Wert");
    DomItem* topKnotenA = document->getObjectById(topKante->getFirstValueAtPath("ID_TOP_Knoten_A/Wert"), currentItemState);
    QString idGeoKnoten = topKnotenA->getFirstValueAtPath("ID_GEO_Knoten/Wert");

    DomItem* currentGeoKante = NULL;
    bool traverseForward = true;

    // find the starting GEO_Kante
    QList<DomItem*> objectlist = document->getObjectList(currentItemState);
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
        return QPointF();
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

        return QPointF(px, py);
    }

    return QPointF();
}


QList<QLineF> GraphicsSceneBuilder::calculateBereichObjekt(const DomItem* o)
{
    QList<QLineF> returnlist;
    if(!document || !graphicsScene)
    {
        return returnlist;
    }

    const DomItem* lstZustandItem = o->parent()->parent();
    QString lstZustandItemName = lstZustandItem->getName();
    PlanProDocument::PlanningState currentItemState = PlanProDocument::PlanningStateEnd;
    if(lstZustandItemName == "LST_Zustand_Start")
    {
        currentItemState = PlanProDocument::PlanningStateStart;
    }

    QList<const DomItem*> teilbereichList = o->getChildItems("Bereich_Objekt_Teilbereich");

    for(int i = 0; i < teilbereichList.count(); ++i)
    {
        const DomItem* teilbereichElement = teilbereichList.at(i);
        QString idTopKante = teilbereichElement->getFirstValueAtPath("ID_TOP_Kante/Wert");
        double begrenzungA = teilbereichElement->getFirstValueAtPath("Begrenzung_A/Wert").toDouble();
        double begrenzungB = teilbereichElement->getFirstValueAtPath("Begrenzung_B/Wert").toDouble();

        DomItem* topKante = document->getObjectById(idTopKante, currentItemState);
        DomItem* topKnotenA = document->getObjectById(topKante->getFirstValueAtPath("ID_TOP_Knoten_A/Wert"), currentItemState);
        QString idGeoKnoten = topKnotenA->getFirstValueAtPath("ID_GEO_Knoten/Wert");

        DomItem* currentGeoKante = NULL;
        bool traverseForward = true;

        // find the starting GEO_Kante
        QList<DomItem*> objectlist = document->getObjectList(currentItemState);
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
            return returnlist;
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

            // Retrieve the coordinates of the GEO_Punkt objects and collect the intermediate lines of the Bereich_Objekt
            if(punktA && punktB)
            {
                double ax = punktA->getFirstValueAtPath("GEO_Punkt_Allg/GK_X/Wert").toDouble();
                double ay = punktA->getFirstValueAtPath("GEO_Punkt_Allg/GK_Y/Wert").toDouble();

                QLineF segment(px, -py, ax, -ay);
                returnlist.append(segment);

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

            QLineF segment(px, -py, endx, -endy);
            returnlist.append(segment);
        }
    }

    return returnlist;
}
