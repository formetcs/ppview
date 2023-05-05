#ifndef PLANPRODOCUMENT_H
#define PLANPRODOCUMENT_H

#include "domitem.h"

#include <QObject>

class PlanProDocument : public QObject
{
    Q_OBJECT
public:
    enum DocumentType
    {
        DocumentTypeInvalid = 0,
        DocumentTypeState = 1,
        DocumentTypePlanning = 2,
        DocumentTypeUnknown = 99
    };

    enum PlanningState
    {
        PlanningStateStart = 0,
        PlanningStateEnd = 1,
        PlanningStateBoth = 2
    };

    struct ObjectListItem
    {
        DomItem* itemStart;
        DomItem* itemEnd;
        QString id;
        PlanningState state;

        ObjectListItem()
        {
            itemStart = NULL;
            itemEnd = NULL;
            id = "00000000-0000-0000-0000-000000000000";
            state = PlanningStateBoth;
        }

        bool isValid()
        {
            return (itemStart || itemEnd);
        }
    };

    explicit PlanProDocument(QObject *parent = nullptr);
    ~PlanProDocument();
    DomItem* getRootItem();
    QDateTime getTimestamp() const;
    QString getToolName() const;
    QString getToolVersion() const;
    QString getRemark() const;
    void setRemark(const QString& rem);
    void updateHeader(const QString& toolname, const QString& toolversion);
    DocumentType getDocumentType() const;
    QStringList getCategoryList() const;
    QList<DomItem*> getObjectList(PlanningState state, const QString& category = QString());
    QList<ObjectListItem> getCombinedObjectList(const QString& category = QString());
    DomItem* getObjectById(const QString& id, PlanningState state = PlanningStateEnd);
    ObjectListItem getObjectListItemById(const QString& id);
    void clearCache();

public slots:
    void documentChanged();

signals:
    void dataChanged();

protected:
    DomItem* rootItem;
    mutable DocumentType cachedDocumentType;
    QHash<QString, QList<DomItem*>> cachedStartObjectList;
    QHash<QString, QList<DomItem*>> cachedEndObjectList;
    QHash<QString, QList<ObjectListItem>> cachedCombinedObjectList;

};

#endif // PLANPRODOCUMENT_H
