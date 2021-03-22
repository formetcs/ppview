#ifndef NEXTTOPKANTERESULT_H
#define NEXTTOPKANTERESULT_H

#include <QDomNode>


class NextTopKanteResult
{
public:
    NextTopKanteResult();
    NextTopKanteResult(QDomNode tke, bool dir);

    void setTopKanteElement(QDomNode tke);
    QDomNode getTopKanteElement();
    void setDirection(bool dir);
    bool getDirection();

private:
    QDomNode topKanteElement;
    bool direction;
};

#endif // NEXTTOPKANTERESULT_H
