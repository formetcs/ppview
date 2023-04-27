#include "objectlistmodel.h"
#include "QtGui/qbrush.h"

ObjectListModel::ObjectListModel(QObject *parent)
    : QAbstractTableModel{parent}
{
    doc = NULL;
    state = PlanProDocument::End;
    category = QString();
}

void ObjectListModel::setDocument(PlanProDocument* d)
{
    doc = d;
    emit layoutChanged();
}

void ObjectListModel::changePlanningState(PlanProDocument::PlanningState st)
{
    state = st;
    emit layoutChanged();
}

void ObjectListModel::changeCategory(const QString& cat)
{
    category = cat;
    emit layoutChanged();
}

void ObjectListModel::modelChanged()
{
    emit layoutChanged();
}

QString ObjectListModel::createSectionText(DomItem* item, int section) const
{
    if(section == 0)
    {
        return item->getName();
    }
    if(section == 1)
    {
        return item->getFirstValueAtPath("Identitaet/Wert");
    }
    if(section == 2)
    {
        return item->getFirstValueAtPath("Bezeichnung/Bezeichnung_Lageplan_Kurz/Wert");
    }
    return QString();
}


QVariant ObjectListModel::data(const QModelIndex& index, int role) const
{
    //qDebug("List:data");
    if (!index.isValid())
        return QVariant();

    if(state == PlanProDocument::Both)
    {
        QList<PlanProDocument::ObjectListItem> objectlist = doc->getCombinedObjectList(category);

        if (index.row() >= objectlist.count())
            return QVariant();

        PlanProDocument::ObjectListItem item = objectlist.at(index.row());

        if(role == Qt::BackgroundRole && item.state == PlanProDocument::Both)
        {
            return QBrush(Qt::white);
        }
        if(role == Qt::BackgroundRole && item.state == PlanProDocument::Start)
        {
            return QBrush(Qt::yellow);
        }
        if(role == Qt::BackgroundRole && item.state == PlanProDocument::End)
        {
            return QBrush(Qt::red);
        }
        if(role == Qt::DisplayRole)
        {
            return createSectionText(item.itemEnd, index.column());
        }
        return QVariant();
    }

    QList<DomItem*> objectlist = doc->getObjectList(state, category);

    if (index.row() >= objectlist.count())
        return QVariant();

    DomItem* item = objectlist.at(index.row());

    if(role == Qt::DisplayRole)
    {
        return createSectionText(item, index.column());
    }

    return QVariant();
}

Qt::ItemFlags ObjectListModel::flags(const QModelIndex& index) const
{
    //qDebug("List:flags");
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractTableModel::flags(index);
}

QVariant ObjectListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    //qDebug("List:headerData");
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch (section)
        {
        case 0:
            return tr("Type");
        case 1:
            return tr("ID");
        case 2:
            return tr("Name");
        case 3:
            return tr("Description");
        default:
            return QVariant();
        }
    }

    return QVariant();
}

int ObjectListModel::rowCount(const QModelIndex& parent) const
{
    //qDebug("List:rowCount");
    Q_UNUSED(parent);

    if(state == PlanProDocument::Both)
    {
        return doc->getCombinedObjectList(category).count();
    }

    return doc->getObjectList(state, category).count();
}

int ObjectListModel::columnCount(const QModelIndex& parent) const
{
    //qDebug("List:columnCount");
    Q_UNUSED(parent);

    return 4;
}
