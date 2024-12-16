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

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE

class QProcess;
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButtonAddHeader_clicked();

    void on_pushButtonParsePlanPro_clicked();

    void on_pushButtonVerify_clicked();

    void on_pushButtonAddTextFile_clicked();

    void handleSmtOutput();

    void on_pushButtonEnterCommand_clicked();

private:
    QString createDatatypeDeclarations(QSet<QString> idSet);
    QString convertIdString(QString rawId);
    Ui::Widget *ui;
    QProcess* smtprocess;
    QString variables;
    int msecs;
};
#endif // WIDGET_H
