#include "objectinfowidget.h"

ObjectInfoWidget::ObjectInfoWidget(QWidget* parent)
    : QTextBrowser(parent)
{
    setOpenLinks(false);
    connect(this, SIGNAL(anchorClicked(QUrl)), this, SLOT(handleHyperlinkSelection(QUrl)));
    uuidRegExp = QRegularExpression("[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12}");
}

void ObjectInfoWidget::setInfoText(QList<DomItem*> objectlist)
{
    clear();
    QString htmlstring;
    for(int i = 0; i < objectlist.count(); ++i)
    {
        DomItem* temp = objectlist.at(i);
        htmlstring += generateInfoText(temp);
        htmlstring += "<br>";
    }
    setHtml(htmlstring);
}

void ObjectInfoWidget::handleHyperlinkSelection(const QUrl& link)
{
    QString linkstring = link.toString();
    linkstring = linkstring.remove("ppview://");
    linkstring = linkstring.toUpper();
    emit uuidClicked(linkstring);
}

int ObjectInfoWidget::getMaxDomDepth(const DomItem* o) const
{
    int maxchilddepth = 0;
    for(int i = 0; i < o->childCount(); ++i)
    {
        int tempdepth = getMaxDomDepth(o->getChild(i));
        if(tempdepth > maxchilddepth)
            maxchilddepth = tempdepth;
    }

    return maxchilddepth + 1;
}

int ObjectInfoWidget::getLeafCount(const DomItem* o) const
{
    if(o->childCount() == 0)
    {
        return 1;
    }

    int tempcount = 0;
    for(int i = 0; i < o->childCount(); ++i)
    {
        tempcount += getLeafCount(o->getChild(i));
    }

    return tempcount;
}

QString ObjectInfoWidget::generateAttributeString(const DomItem* o) const
{
    QString returnval = QString();
    if(!o)
    {
        return returnval;
    }
    QHash<QString, QString> attributes = o->getAttributeMap();
    QHashIterator<QString, QString> i(attributes);
    while(i.hasNext())
    {
        i.next();
        QString name = i.key();
        name = name.mid(name.indexOf(":") + 1);
        QString value = i.value();
        value = value.mid(value.indexOf(":") + 1);
        returnval += name + "=\"" + value + '"';
    }
    if(!returnval.isEmpty())
    {
        returnval.append("]");
        returnval.prepend(" [");
    }
    return returnval;
}

QString ObjectInfoWidget::generateInfoText(const DomItem* o) const
{
    int maxdepth = getMaxDomDepth(o);
    QString id = o->getFirstValueAtPath("Identitaet/Wert");
    QString returnval = QString("<table border='1' cellpadding='2'>\n");
    returnval += QString("<tr><th bgcolor='deepskyblue' colspan ='%1'>%2%3</th></tr>\n<tr>").arg(maxdepth) .arg(o->getName(), generateAttributeString(o));
    returnval += generateInfoTextRec(o, 1, maxdepth, id);
    returnval = returnval.left(returnval.size() - 4); // remove the ending "<tr>"
    returnval += QString("</table>\n");
    return returnval;
}

QString ObjectInfoWidget::generateInfoTextRec(const DomItem* o, int currentDepth, int maxDepth, QString originalId) const
{
    QString returnval = QString();
    if(o->childCount() == 0)
    {
        QString valuestring = o->getValue();
        valuestring.truncate(1000);
        QString finalvalue = valuestring;
        QRegularExpressionMatch match = uuidRegExp.match(valuestring);
        if(match.hasMatch() && valuestring != originalId)
        {
            finalvalue = QString("<a href='ppview://%1'>%2</a>").arg(valuestring, valuestring);
        }
        returnval += QString("<td>%1</td></tr>\n<tr>").arg(finalvalue);
        return returnval;
    }
    for(int i = 0; i < o->childCount(); ++i)
    {
        const DomItem* childnode = o->getChild(i);
        int leafcount = getLeafCount(childnode);
        int colspan = 1;
        if(childnode->childCount() == 0)
        {
            colspan = maxDepth - currentDepth;
        }
        returnval += QString("<td bgcolor='lightskyblue' rowspan ='%1' colspan ='%2'><b>%3</b>%4</td>").arg(leafcount) .arg(colspan) .arg(childnode->getName(), generateAttributeString(childnode));
        returnval += generateInfoTextRec(childnode, currentDepth + 1, maxDepth, originalId);
    }
    return returnval;
}
