/*
 * PlanPro Viewer
 *
 * Copyright (c) 2017-2025, The FormETCS Project. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

/*!
 * \file planproxmldocument.cpp
 *
 * \brief Implementation of the PlanProXmlDocument class.
 */

#include "planproxmldocument.h"

#include <QtCore>
#include <QtXml>

PlanProXmlDocument::PlanProXmlDocument(QObject *parent)
    : PlanProDocument{parent}
{
}

bool PlanProXmlDocument::loadFile(const QString& filename)
{
    bool returnval = false;
    QFile dataFile(filename);
    if (!dataFile.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        returnval = false;
        fileName = QString();
    }
    else
    {
        QDomDocument domDocument = QDomDocument("objectlist");
        if(!domDocument.setContent(&dataFile))
        {
            returnval = false;
            fileName = QString();
        }
        else
        {
            setupDomTree(domDocument);
            clearCache();
            if(getDocumentType() == DocumentTypeInvalid)
            {
                returnval = false;
                fileName = QString();
            }
            else
            {
                returnval = true;
                fileName = filename;
            }

        }
        dataFile.close();
    }
    if(rootItem && !returnval)
    {
        delete rootItem;
        rootItem = NULL;
    }
    clearCache();
    return returnval;
}


bool PlanProXmlDocument::saveFile(const QString& filename)
{
    if(rootItem == NULL)
    {
        return false;
    }
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
      return false;
    }
    QTextStream ts(&file);
    ts << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>" << Qt::endl;
    ts << generateXml();
    file.close();
    fileName = filename;
    return true;
}

QString PlanProXmlDocument::getFileName() const
{
    return fileName;
}

void PlanProXmlDocument::setupDomTree(const QDomDocument& doc)
{
    if(rootItem != NULL)
    {
        delete rootItem;
    }
    QDomElement docElem = doc.documentElement();
    DomItem* docItem = createChildItem(docElem, NULL);
    rootItem = docItem;
}

DomItem* PlanProXmlDocument::createChildItem(const QDomNode& n, DomItem* parent)
{
    QString childName = n.nodeName();
    QString childText = n.nodeValue();
    QDomNamedNodeMap childAttributes = n.attributes();
    QDomNodeList childSubchildNodes = n.childNodes();

    DomItem* childItem = new DomItem(childName, parent);

    for(int i = 0; i < childAttributes.length(); i++)
    {
        QDomNode attrNode = childAttributes.item(i);
        QString attrName = attrNode.nodeName();
        QString attrValue = attrNode.nodeValue();
        childItem->addAttribute(attrName, attrValue);
    }

    for(int i = 0; i < childSubchildNodes.length(); i++)
    {
        QDomNode subchildNode = childSubchildNodes.item(i);
        if(subchildNode.isText())
        {
            childText += subchildNode.nodeValue();
        }
        else if(subchildNode.isElement())
        {
            DomItem* subchildItem = createChildItem(subchildNode, childItem);
            childItem->addChild(subchildItem);
        }
    }

    childItem->setValue(childText);

    return childItem;
}

QString PlanProXmlDocument::generateXml()
{
    QDomDocument domDocument;
    QDomElement root = createChildElement(rootItem, domDocument);
    domDocument.appendChild(root);

    return domDocument.toString(2);
}

QDomElement PlanProXmlDocument::createChildElement(const DomItem* item, QDomDocument& doc)
{
    QDomElement elem = doc.createElement(item->getName());

    QHash<QString, QString> attributes = item->getAttributeMap();
    QHashIterator<QString, QString> i(attributes);
    while(i.hasNext())
    {
        i.next();
        elem.setAttribute(i.key(), i.value());
    }

    QDomText text = doc.createTextNode(item->getValue());
    elem.appendChild(text);

    for(int i = 0; i < item->childCount(); i++)
    {
        const DomItem* childItem = item->getChild(i);
        QDomElement childElement = createChildElement(childItem, doc);
        elem.appendChild(childElement);
    }

    return elem;
}
