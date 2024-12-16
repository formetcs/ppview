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

#ifndef OBJECTINFOWIDGET_H
#define OBJECTINFOWIDGET_H

#include <QRegularExpression>
#include <QTextBrowser>

#include "domitem.h"

class ObjectInfoWidget : public QTextBrowser
{
    Q_OBJECT
public:
    explicit ObjectInfoWidget(QWidget* parent = nullptr);


public slots:
    void setInfoText(QList<DomItem*> objectlist);

signals:
    void uuidClicked(const QString& id);

private slots:
    void handleHyperlinkSelection(const QUrl& link);

private:
    int getMaxDomDepth(const DomItem* o) const;
    int getLeafCount(const DomItem* o) const;
    QString generateAttributeString(const DomItem* o) const;
    QString generateInfoText(const DomItem* o) const;
    QString generateInfoTextRec(const DomItem* o, int currentDepth, int maxDepth, QString originalId) const;

    QRegularExpression uuidRegExp;
};

#endif // OBJECTINFOWIDGET_H
