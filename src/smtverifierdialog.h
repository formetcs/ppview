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

#ifndef SMTVERIFIERDIALOG_H
#define SMTVERIFIERDIALOG_H

#include <QDialog>


class QProcess;
class PlanProDocument;

namespace Ui {
class SmtVerifierDialog;
}

class SmtVerifierDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SmtVerifierDialog(PlanProDocument* doc, QWidget* parent = nullptr);
    ~SmtVerifierDialog();
    void reset();
    QStringList getCounterexampleList();

signals:
    void counterexamplesFound();

private slots:
    void openTestcase();
    void saveAsSmtlibFile();
    void startVerification();
    void handleSmtOutput();
    void stopProcess();

private:
    QString createSmtQuery();
    QString includeFile(QString filename, QString parentFile);
    bool isTypeSelected(QString type);

    Ui::SmtVerifierDialog* ui;
    PlanProDocument* document;

    QProcess* smtprocess;
    int msecs;
    QString logic;
    QStringList variables;
    QString fileContent;
    QString options;
    QStringList counterexampleList;
};

#endif // SMTVERIFIERDIALOG_H
