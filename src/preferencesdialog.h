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

#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget* parent = nullptr);
    ~PreferencesDialog();

    void loadSettings();
    void saveSettings();

private slots:
    void handleObjectListSelection();
    void handlePushButtonSetLineColorClicked();
    void handlePushButtonSetFillColorClicked();
    void handlePushButtonSetForegroundColorClicked();
    void handlePushButtonSetBackgroundColorClicked();
    void penPropertiesChanged();
    void addSmt();
    void removeSmt();
    void editSmt();
    void handleSmtListSelection();

private:
    void handleSetLayoutColor(bool linecolor);
    void handleSetObjectListColor(bool foreground);
    QStringList getAvailableLanguages();
    QPen getSelectedPen();
    void setSelectedPen(QPen p);
    QBrush getSelectedBrush();
    void setSelectedBrush(QBrush b);

    Ui::PreferencesDialog* ui;

    QHash<QString,QPen> changedPens;
    QHash<QString,QBrush> changedBrushes;

    QHash<QString,QString> changedSmtCommands;
    QHash<QString,QString> changedSmtArguments;

    QStringList punktObjektNames;
    QStringList bereichObjektNames;
};

#endif // PREFERENCESDIALOG_H
