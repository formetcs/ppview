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
 * \file planproxmldocument.h
 *
 * \brief Declaration of the PlanProXmlDocument class.
 */

#ifndef PLANPROXMLDOCUMENT_H
#define PLANPROXMLDOCUMENT_H

#include "planprodocument.h"

#include <QDomElement>


/*!
 * \brief This class extends the PlanProDocument class with functions to load and save XML files.
 *
 * It loads the file into a QDomDocument first, creates a tree of DomItem objects recursively and deletes the unneeded QDomDocument finally.
 * Similarly, when the document should be saved, an intermediate QDomDocument representation is built before it is saved to file.
 */
class PlanProXmlDocument : public PlanProDocument
{
public:
    /*!
     * \brief Creates an empty document with no content.
     * \param parent the parent object
     */
    explicit PlanProXmlDocument(QObject *parent = nullptr);

    /*!
     * \brief Load a PlanPro XML file and set up the tree of DomItem objects.
     *
     * \param filename the file name of the PlanPro XML file
     * \return true if the file was loaded successful, otherwise false
     */
    bool loadFile(const QString& filename);

    /*!
     * \brief Save the current DomItem tree to a PlanPro XML file.
     *
     * \param filename the file name of the PlanPro XML file
     * \return true if the file was loaded successful, otherwise false
     */
    bool saveFile(const QString& filename);

    /*!
     * \brief Get the file name of the currently loaded PlanPro XML file.
     *
     * \return the file name of the loaded PlanPro XML file, or an empty string if no file is loaded
     */
    QString getFileName() const;


private:
    /*!
     * \brief Set up the tree of DomItem objects from a QDomDocument.
     *
     * \param doc the QDomDocument object containing the PlanPro data
     */
    void setupDomTree(const QDomDocument& doc);

    /*!
     * \brief Create recursively a DomItem child tree from a QDomNode object.
     *
     * \param n the QDomNode containing the subtree to convert
     * \param parent the parent item of the newly created DomItem (or NULL if this shoud be the root item)
     * \return the DomItem containing the subtree based on the provided QDomNode
     */
    DomItem* createChildItem(const QDomNode& n, DomItem* parent);

    /*!
     * \brief Generate a XML string of the current DomItem tree.
     *
     * \return a XML string of the current DomItem tree
     */
    QString generateXml();

    /*!
     * \brief Create recursively a QDomElement child tree from a DomItem object.
     *
     * \param item the DomItem containing the subtree to convert
     * \param doc the QDomDocument in which the created element should be stored
     * \return the QDomElement containing the subtree based on the provided DomItem
     */
    QDomElement createChildElement(const DomItem* item, QDomDocument& doc);

    QString fileName; /*!< \brief The file name of the loaded PlanPro XML file. */
};

#endif // PLANPROXMLDOCUMENT_H
