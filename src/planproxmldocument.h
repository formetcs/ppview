#ifndef PLANPROXMLDOCUMENT_H
#define PLANPROXMLDOCUMENT_H

#include "planprodocument.h"

#include <QtXml>

class PlanProXmlDocument : public PlanProDocument
{
public:
    explicit PlanProXmlDocument(QObject *parent = nullptr);
    bool loadFile(const QString& filename);
    bool saveFile(const QString& filename);
    QString getFileName() const;


private:
    void setupDomTree(const QDomDocument& doc);
    DomItem* createChildItem(const QDomNode& n, DomItem* parent);
    QString generateXml();
    QDomElement createChildElement(const DomItem* item, QDomDocument& doc);
    QString fileName;
};

#endif // PLANPROXMLDOCUMENT_H
