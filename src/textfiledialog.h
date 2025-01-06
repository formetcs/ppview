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

#ifndef TEXTFILEDIALOG_H
#define TEXTFILEDIALOG_H

#include <QDialog>

class QDialogButtonBox;
class QTextEdit;

class TextFileDialog : public QDialog
{
    Q_OBJECT
public:
    TextFileDialog(QWidget* parent = 0);

    void loadFile(QString filename, bool useMarkdown = false);
    void setErrorText(QString errortext);


private:
    QTextEdit* textEdit;
    QDialogButtonBox* buttonBox;

    QString errorText;
};

#endif // TEXTFILEDIALOG_H
