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

#ifndef OBJECTLISTMODEL_H
#define OBJECTLISTMODEL_H

#include <QAbstractTableModel>

#include "mainwindow.h"

class PlanProDocument;

class ObjectListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ObjectListModel(QObject *parent = nullptr);
    void setDocument(PlanProDocument* d);
    DomItem* getItem(const QModelIndex& index) const;
    QModelIndex getModelIndexById(QString id) const;

    QVariant data(const QModelIndex& index, int role) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;

public slots:
    void changeViewMode(MainWindow::ViewMode mode);
    void changeCategory(const QString& cat);
    void modelAboutToBeChanged();
    void modelChanged();

private:
    QString createSectionText(const DomItem* item, int section) const;

    PlanProDocument* doc;
    QString category;
    MainWindow::ViewMode viewMode;
};

#endif // OBJECTLISTMODEL_H
