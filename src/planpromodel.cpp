#include <QtCore>
#include "planpromodel.h"

PlanProModel::PlanProModel(QObject* parent)
    :QAbstractItemModel(parent)
{
    doc = NULL;
}

PlanProModel::~PlanProModel()
{
}

void PlanProModel::setDocument(PlanProDocument* d)
{
    doc = d;
    emit layoutChanged();
}

void PlanProModel::modelChanged()
{
    emit layoutChanged();
}

QModelIndex PlanProModel::getModelIndex(DomItem* item)
{
    if(!item)
    {
        return QModelIndex();
    }

    DomItem* parentItem = item->parent();
    QModelIndex parentIndex = getModelIndex(parentItem);
    int row = item->getOwnPosition();

    return index(row, 0, parentIndex);
}

//QList<NextTopKanteResult> PlanProModel::getNextTopKante(QDomNode topKante, bool forward)
//{
//    QList<NextTopKanteResult> returnval;
//    QString srcIdTopKnotenA = topKante.firstChildElement("ID_TOP_Knoten_A").firstChildElement("Wert").text();
//    QString srcIdTopKnotenB = topKante.firstChildElement("ID_TOP_Knoten_B").firstChildElement("Wert").text();
//    QString srcTopAnschlussA = topKante.firstChildElement("TOP_Kante_Allg").firstChildElement("TOP_Anschluss_A").firstChildElement("Wert").text();
//    QString srcTopAnschlussB = topKante.firstChildElement("TOP_Kante_Allg").firstChildElement("TOP_Anschluss_B").firstChildElement("Wert").text();
//    //QDomElement containerElement = getContainerElement();
//    QDomNodeList nodelist = getContainerElement().childNodes();
//    for(int i = 0; i < nodelist.count(); i++)
//    {
//        QDomNode n = nodelist.at(i);
//        if (n.isElement())
//        {
//            //QDomElement e = n.toElement();
//            QString name = n.nodeName();
//            if (name == "TOP_Kante" && n != topKante)
//            {
//                QString dstIdTopKnotenA = n.firstChildElement("ID_TOP_Knoten_A").firstChildElement("Wert").text();
//                QString dstIdTopKnotenB = n.firstChildElement("ID_TOP_Knoten_B").firstChildElement("Wert").text();
//                QString dstTopAnschlussA = n.firstChildElement("TOP_Kante_Allg").firstChildElement("TOP_Anschluss_A").firstChildElement("Wert").text();
//                QString dstTopAnschlussB = n.firstChildElement("TOP_Kante_Allg").firstChildElement("TOP_Anschluss_B").firstChildElement("Wert").text();

//                if (forward && (srcIdTopKnotenB == dstIdTopKnotenA) && srcTopAnschlussB == "Verbindung"
//                        && dstTopAnschlussA == "Verbindung") {
//                    NextTopKanteResult res(n, true);
//                    returnval.append(res);
//                } else if (forward && (srcIdTopKnotenB == dstIdTopKnotenA) && srcTopAnschlussB == "Links"
//                           && dstTopAnschlussA == "Spitze") {
//                    NextTopKanteResult res(n, true);
//                    returnval.append(res);
//                } else if (forward && (srcIdTopKnotenB == dstIdTopKnotenA) && srcTopAnschlussB == "Rechts"
//                           && dstTopAnschlussA == "Spitze") {
//                    NextTopKanteResult res(n, true);
//                    returnval.append(res);
//                } else if (forward && (srcIdTopKnotenB == dstIdTopKnotenA) && srcTopAnschlussB == "Spitze"
//                           && dstTopAnschlussA == "Links") {
//                    NextTopKanteResult res(n, true);
//                    returnval.append(res);
//                } else if (forward && (srcIdTopKnotenB == dstIdTopKnotenA) && srcTopAnschlussB == "Spitze"
//                           && dstTopAnschlussA == "Rechts") {
//                    NextTopKanteResult res(n, true);
//                    returnval.append(res);
//                }

//                else if (forward && (srcIdTopKnotenB == dstIdTopKnotenB) && srcTopAnschlussB == "Verbindung"
//                         && dstTopAnschlussB == "Verbindung") {
//                    NextTopKanteResult res(n, false);
//                    returnval.append(res);
//                } else if (forward && (srcIdTopKnotenB == dstIdTopKnotenB) && srcTopAnschlussB == "Links"
//                           && dstTopAnschlussB == "Spitze") {
//                    NextTopKanteResult res(n, false);
//                    returnval.append(res);
//                } else if (forward && (srcIdTopKnotenB == dstIdTopKnotenB) && srcTopAnschlussB == "Rechts"
//                           && dstTopAnschlussB == "Spitze") {
//                    NextTopKanteResult res(n, false);
//                    returnval.append(res);
//                } else if (forward && (srcIdTopKnotenB == dstIdTopKnotenB) && srcTopAnschlussB == "Spitze"
//                           && dstTopAnschlussB == "Links") {
//                    NextTopKanteResult res(n, false);
//                    returnval.append(res);
//                } else if (forward && (srcIdTopKnotenB == dstIdTopKnotenB) && srcTopAnschlussB == "Spitze"
//                           && dstTopAnschlussB == "Rechts") {
//                    NextTopKanteResult res(n, false);
//                    returnval.append(res);
//                }

//                else if (!forward && (srcIdTopKnotenA == dstIdTopKnotenB) && srcTopAnschlussA == "Verbindung"
//                         && dstTopAnschlussB == "Verbindung") {
//                    NextTopKanteResult res(n, false);
//                    returnval.append(res);
//                } else if (!forward && (srcIdTopKnotenA == dstIdTopKnotenB) && srcTopAnschlussA == "Links"
//                           && dstTopAnschlussB == "Spitze") {
//                    NextTopKanteResult res(n, false);
//                    returnval.append(res);
//                } else if (!forward && (srcIdTopKnotenA == dstIdTopKnotenB) && srcTopAnschlussA == "Rechts"
//                           && dstTopAnschlussB == "Spitze") {
//                    NextTopKanteResult res(n, false);
//                    returnval.append(res);
//                } else if (!forward && (srcIdTopKnotenA == dstIdTopKnotenB) && srcTopAnschlussA == "Spitze"
//                           && dstTopAnschlussB == "Links") {
//                    NextTopKanteResult res(n, false);
//                    returnval.append(res);
//                } else if (!forward && (srcIdTopKnotenA == dstIdTopKnotenB) && srcTopAnschlussA == "Spitze"
//                           && dstTopAnschlussB == "Rechts") {
//                    NextTopKanteResult res(n, false);
//                    returnval.append(res);
//                }

//                else if (!forward && (srcIdTopKnotenA == dstIdTopKnotenA) && srcTopAnschlussA == "Verbindung"
//                         && dstTopAnschlussA == "Verbindung") {
//                    NextTopKanteResult res(n, true);
//                    returnval.append(res);
//                } else if (!forward && (srcIdTopKnotenA == dstIdTopKnotenA) && srcTopAnschlussA == "Links"
//                           && dstTopAnschlussA == "Spitze") {
//                    NextTopKanteResult res(n, true);
//                    returnval.append(res);
//                } else if (!forward && (srcIdTopKnotenA == dstIdTopKnotenA) && srcTopAnschlussA == "Rechts"
//                           && dstTopAnschlussA == "Spitze") {
//                    NextTopKanteResult res(n, true);
//                    returnval.append(res);
//                } else if (!forward && (srcIdTopKnotenA == dstIdTopKnotenA) && srcTopAnschlussA == "Spitze"
//                           && dstTopAnschlussA == "Links") {
//                    NextTopKanteResult res(n, true);
//                    returnval.append(res);
//                } else if (!forward && (srcIdTopKnotenA == dstIdTopKnotenA) && srcTopAnschlussA == "Spitze"
//                           && dstTopAnschlussA == "Rechts") {
//                    NextTopKanteResult res(n, true);
//                    returnval.append(res);
//                }
//            }
//        }
//    }
//    return returnval;
//}

//double PlanProModel::calculateDistance(QModelIndexList selectedIndexes)
//{
//    QModelIndex index1 = selectedIndexes.at(0);
//    QModelIndex index2 = selectedIndexes.at(1);
//    DomItem* item1 = static_cast<DomItem*>(index1.internalPointer());
//    QDomNode node1 = item1->node();
//    DomItem* item2 = static_cast<DomItem*>(index2.internalPointer());
//    QDomNode node2 = item2->node();
//    if(node1.firstChildElement("Punkt_Objekt_TOP_Kante").isNull() || node2.firstChildElement("Punkt_Objekt_TOP_Kante").isNull())
//    {
//        return -5;
//    }

//    QString identitaet1 = node1.firstChildElement("Identitaet").firstChildElement("Wert").text();
//    QDomElement punktObjektTopKanteElement1 = node1.firstChildElement("Punkt_Objekt_TOP_Kante");
//    while(!punktObjektTopKanteElement1.isNull())
//    {
//        QString idTopKante1 = punktObjektTopKanteElement1.firstChildElement("ID_TOP_Kante").firstChildElement("Wert").text();
//        double abstand1 = punktObjektTopKanteElement1.firstChildElement("Abstand").firstChildElement("Wert").text().toDouble();
//        QString wirkrichtung1 = punktObjektTopKanteElement1.firstChildElement("Wirkrichtung").firstChildElement("Wert").text();

//        QString identitaet2 = node2.firstChildElement("Identitaet").firstChildElement("Wert").text();
//        QDomElement punktObjektTopKanteElement2 = node2.firstChildElement("Punkt_Objekt_TOP_Kante");
//        while(!punktObjektTopKanteElement2.isNull())
//        {
//            QString idTopKante2 = punktObjektTopKanteElement2.firstChildElement("ID_TOP_Kante").firstChildElement("Wert").text();
//            double abstand2 = punktObjektTopKanteElement2.firstChildElement("Abstand").firstChildElement("Wert").text().toDouble();
//            QString wirkrichtung2 = punktObjektTopKanteElement2.firstChildElement("Wirkrichtung").firstChildElement("Wert").text();

//            PunktObjekt po1(identitaet1, idTopKante1, abstand1, wirkrichtung1);
//            PunktObjekt po2(identitaet2, idTopKante2, abstand2, wirkrichtung2);

//            double result = calculateDistance(po1, po2, true);
//            if(result >= 0)
//            {
//                return result;
//            }
//            result = calculateDistance(po1, po2, false);
//            if(result >= 0)
//            {
//                return result;
//            }
//            punktObjektTopKanteElement2 = punktObjektTopKanteElement2.nextSiblingElement("Punkt_Objekt_TOP_Kante");
//        }
//        punktObjektTopKanteElement1 = punktObjektTopKanteElement1.nextSiblingElement("Punkt_Objekt_TOP_Kante");
//    }
//    return -1;
//}

//double PlanProModel::calculateDistance(PunktObjekt startpos, PunktObjekt endpos, bool forward)
//{
//    double returnval = -1;
//    QString startGuid = startpos.getIdTopKante();
//    double startAbstand = startpos.getAbstand();
//    QString startWirkrichtung = startpos.getWirkrichtung();
//    QString endGuid = endpos.getIdTopKante();
//    double endAbstand = endpos.getAbstand();
//    //QString endWirkrichtung = endpos.getWirkrichtung();
//    if (startGuid == endGuid && startWirkrichtung != "gegen" && forward) { // both positions are on the
//        // same edge
//        returnval = endAbstand - startAbstand;
//    } else if (startGuid == endGuid && startWirkrichtung != "gegen" && !forward) { // both positions are
//        // on the same edge
//        returnval = startAbstand - endAbstand;
//    } else if (startGuid == endGuid && startWirkrichtung == "gegen" && !forward) { // both positions are
//        // on the same edge
//        returnval = endAbstand - startAbstand;
//    } else if (startGuid == endGuid && startWirkrichtung == "gegen" && forward) { // both positions are on
//        // the same edge
//        returnval = startAbstand - endAbstand;
//    } else {
//        QDomElement startEdge = getObjectById(startGuid);
//        if (!startEdge.isNull()) {
//            double currentDistance = -1;
//            double remainingDistance = -1;
//            bool direction = true;
//            if (startWirkrichtung != "gegen" && forward) {
//                double edgeLength = startEdge.firstChildElement("TOP_Kante_Allg").firstChildElement("TOP_Laenge").firstChildElement("Wert").text().toDouble();
//                currentDistance = edgeLength - startAbstand;
//                direction = true;
//            } else if (startWirkrichtung != "gegen" && !forward) {
//                currentDistance = startAbstand;
//                direction = false;
//            } else if (startWirkrichtung == "gegen" && forward) {
//                currentDistance = startAbstand;
//                direction = false;
//            } else if (startWirkrichtung == "gegen" && !forward) {
//                double edgeLength = startEdge.firstChildElement("TOP_Kante_Allg").firstChildElement("TOP_Laenge").firstChildElement("Wert").text().toDouble();
//                currentDistance = edgeLength - startAbstand;
//                direction = true;
//            }
//            QList<NextTopKanteResult> edgelist = getNextTopKante(startEdge, direction);
//            for (int i = 0; i < edgelist.size(); i++) {
//                NextTopKanteResult edgeresult = edgelist.at(i);
//                QDomNode tka = edgeresult.getTopKanteElement();
//                QString tka_id = tka.firstChildElement("Identitaet").firstChildElement("Wert").text();
//                bool newdir = edgeresult.getDirection();
//                PunktObjekt newStart(tka_id, 0.0, "in");
//                if (newdir) {
//                    newStart.setAbstand(0.0);
//                } else {
//                    double tka_len = tka.firstChildElement("TOP_Kante_Allg").firstChildElement("TOP_Laenge").firstChildElement("Wert").text().toDouble();
//                    newStart.setAbstand(tka_len);
//                }
//                double tempdist = calculateDistance(newStart, endpos, newdir);
//                if ((remainingDistance < 0 && tempdist >= 0)
//                        || (remainingDistance >= 0 && tempdist >= 0 && tempdist < remainingDistance)) {
//                    remainingDistance = tempdist;
//                }
//            }
//            if (remainingDistance >= 0) {
//                returnval = currentDistance + remainingDistance;
//            }
//        }
//    }

//    return returnval;
//}

QVariant PlanProModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    DomItem* item = static_cast<DomItem*>(index.internalPointer());

    QStringList attributes;
    QHash<QString, QString> attributeMap = item->getAttributeMap();
    QHashIterator<QString, QString> i(attributeMap);
    while(i.hasNext())
    {
        i.next();
        attributes << i.key() + "=\"" + i.value() + '"';
    }

    switch (index.column()) {
    case 0:
        return item->getName();
    case 1:
        return item->getValue().split("\n").join(' ');
    case 2:
        return attributes.join(' ');

    default:
        return QVariant();
    }
}

Qt::ItemFlags PlanProModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

QVariant PlanProModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch (section)
        {
        case 0:
            return tr("Name");
        case 1:
            return tr("Value");
        case 2:
            return tr("Attributes");
        default:
            return QVariant();
        }
    }

    return QVariant();
}

QModelIndex PlanProModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    DomItem* childItem;

    if (!parent.isValid())
    {
        childItem = doc->getRootItem();
    }
    else
    {
        DomItem* parentItem = static_cast<DomItem*>(parent.internalPointer());
        childItem = parentItem->getChild(row);
    }

    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex PlanProModel::parent(const QModelIndex& child) const
{
    if (!child.isValid())
        return QModelIndex();

    DomItem* childItem = static_cast<DomItem*>(child.internalPointer());
    DomItem* parentItem = childItem->parent();

    if (!parentItem)
        return QModelIndex();

    return createIndex(parentItem->getOwnPosition(), 0, parentItem);
}

int PlanProModel::rowCount(const QModelIndex& parent) const
{
    if (parent.column() > 0)
        return 0;

    DomItem* parentItem;

    if (!parent.isValid())
    {
        parentItem = NULL;
    }
    else
    {
        parentItem = static_cast<DomItem*>(parent.internalPointer());
    }

    if(!parentItem)
    {
        return 1;
    }

    return parentItem->childCount();
}

int PlanProModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 3;
}
