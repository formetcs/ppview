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
 * \file objectinfowidget.h
 *
 * \brief Declaration of the ObjectInfoWidget class.
 */

#ifndef OBJECTINFOWIDGET_H
#define OBJECTINFOWIDGET_H

#include <QRegularExpression>
#include <QTextBrowser>

#include "domitem.h"


/*!
 * \brief Class to display the properties of PlanPro objects in a HTML-like table view.
 */
class ObjectInfoWidget : public QTextBrowser
{
    Q_OBJECT
public:
    /*!
     * \brief Creates a new ObjectInfoWidget.
     *
     * \param parent the parent widget
     */
    explicit ObjectInfoWidget(QWidget* parent = nullptr);


public slots:
    /*!
     * \brief Displays the info text for the given PlanPro objects.
     *
     * \param objectlist a list of DomItem objects pointing to the root of each PlanPro object
     */
    void setInfoText(QList<DomItem*> objectlist);

signals:
    /*!
     * \brief Signal to indicate that a linked UUID has been clicked.
     *
     * \param id the UUID string that has been clicked
     */
    void uuidClicked(const QString& id);

private slots:
    /*!
     * \brief Translates the click to a link in the text browser into an uuidClicked() signal.
     *
     * The URLs in the HTML view are coded with ppview:// prefix to identify them as PlanPro UUIDs.
     * This method translates such an URL into a plain UUID string and emits the uuidClicked() signal.
     *
     * \param link URL of the link that has been clicked
     */
    void handleHyperlinkSelection(const QUrl& link);

private:
    /*!
     * \brief Get the maximum depth of a given DomItem subtree.
     *
     * A DomItem without children has depth 1. Each child level increases the depth by 1.
     *
     * \param o the DomItem that should be evaluated
     * \return the maximum depth of that item
     */
    int getMaxDomDepth(const DomItem* o) const;

    /*!
     * \brief Get the number of leaves of a given DomItem subtree.
     *
     * A leaf is an item that does not have any child items.
     *
     * \param o the DomItem that should be evaluated
     * \return the leaf count of that item
     */
    int getLeafCount(const DomItem* o) const;

    /*!
     * \brief Generate a string of all attributes of a DomItem.
     *
     * \param o the DomItem
     * \return the attribute string
     */
    QString generateAttributeString(const DomItem* o) const;

    /*!
     * \brief Generate the info text for a given PlanPro object.
     *
     * This method calls generateInfoTextRec() recursively for all child items to generate the HTML table.
     *
     * \param o the DomItem object pointing to the root of the PlanPro object
     * \return the string containing a HTML table with the object properties
     */
    QString generateInfoText(const DomItem* o) const;

    /*!
     * \brief Generate parts of the info text HTML table for a given child item depth.
     *
     * \param o the current DomItem
     * \param currentDepth the current child depth
     * \param maxDepth the maximum child depth
     * \param originalId the UUID from the PlanPro object where the current DomItem is part of
     * \return part of a HTML table containing object properties for a specific child depth
     */
    QString generateInfoTextRec(const DomItem* o, int currentDepth, int maxDepth, QString originalId) const;

    QRegularExpression uuidRegExp; /*!< \brief The Regular Expression used to match the UUID. */
};

#endif // OBJECTINFOWIDGET_H
