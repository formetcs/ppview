#ifndef OBJECTINFOWIDGET_H
#define OBJECTINFOWIDGET_H

#include <QTextBrowser>

#include "domitem.h"

class ObjectInfoWidget : public QTextBrowser
{
    Q_OBJECT
public:
    explicit ObjectInfoWidget(QWidget* parent = nullptr);


public slots:
    void setInfoText(QList<DomItem*> objectlist);

private:
    int getMaxDomDepth(DomItem* o);
    int getLeafCount(DomItem* o);
    QString generateInfoText(DomItem* o);
    QString generateInfoTextRec(DomItem* o, int currentDepth, int maxDepth);
};

#endif // OBJECTINFOWIDGET_H
