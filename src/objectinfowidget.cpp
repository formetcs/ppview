#include "objectinfowidget.h"


ObjectInfoWidget::ObjectInfoWidget(QWidget* parent)
    : QTextBrowser(parent)
{

}

void ObjectInfoWidget::setInfoText(QList<DomItem*> objectlist)
{
    clear();
    QString htmlstring;
    for(int i = 0; i < objectlist.count(); i++)
    {
        DomItem* temp = objectlist.at(i);
        htmlstring += generateInfoText(temp);
        htmlstring += "<br>";
    }
    setHtml(htmlstring);
}

int ObjectInfoWidget::getMaxDomDepth(DomItem* o)
{
    if(o->childCount() == 0)
    {
        return 0;
    }

    int maxdepth = 0;
    for(int i = 0; i < o->childCount(); i++)
    {
        int tempdepth = getMaxDomDepth(o->getChild(i));
        if(tempdepth > maxdepth)
            maxdepth = tempdepth;
    }

    return maxdepth + 1;
}

int ObjectInfoWidget::getLeafCount(DomItem* o)
{
    if(o->childCount() == 0)
    {
        return 1;
    }

    int tempcount = 0;
    for(int i = 0; i < o->childCount(); i++)
    {
        tempcount += getLeafCount(o->getChild(i));
    }

    return tempcount;
}

QString ObjectInfoWidget::generateInfoText(DomItem* o)
{
    int maxdepth = getMaxDomDepth(o);
    QString returnval = QString("<table border='1' cellpadding='2'>\n");
    returnval += QString("<tr><th bgcolor='deepskyblue' colspan ='%1'>%2</th></tr>\n<tr>").arg(maxdepth - 1) .arg(o->getName());
    returnval += generateInfoTextRec(o, 1, maxdepth);
    returnval = returnval.left(returnval.size() - 4);
    returnval += QString("</table>\n");
    return returnval;
}

QString ObjectInfoWidget::generateInfoTextRec(DomItem* o, int currentDepth, int maxDepth)
{
    QString returnval = QString();
    if(o->childCount() == 0)
    {
        returnval += QString("<td colspan ='%1'></td></tr>\n<tr>") .arg(maxDepth - currentDepth);
        return returnval;
    }
    DomItem* wertelement = o->getFirstChildItem("Wert");
    if(wertelement)
    {
        returnval += QString("<td>%1</td></tr>\n<tr>").arg(wertelement->getValue());
        return returnval;
    }
    for(int i = 0; i < o->childCount(); i++)
    {
        DomItem* childnode = o->getChild(i);
        int leafcount = getLeafCount(childnode);
        int colspan = 1;
        if(childnode->getFirstChildItem("Wert"))
        {
            colspan = maxDepth - currentDepth - 1;
        }
        returnval += QString("<td bgcolor='lightskyblue' rowspan ='%1' colspan ='%2'><b>%3</b></td>").arg(leafcount) .arg(colspan) .arg(childnode->getName());
        returnval += generateInfoTextRec(childnode, currentDepth + 1, maxDepth);
    }

    return returnval;
}
