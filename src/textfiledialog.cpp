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

#include "textfiledialog.h"

#include <QApplication>
#include <QDialogButtonBox>
#include <QFile>
#include <QTextEdit>
#include <QVBoxLayout>

TextFileDialog::TextFileDialog(QWidget* parent)
    : QDialog(parent)
{
    textEdit = new QTextEdit();
    textEdit->setReadOnly(true);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(textEdit);
    layout->addWidget(buttonBox);
    setLayout(layout);
}

void TextFileDialog::loadFile(QString filename, bool useMarkdown)
{
#if defined Q_OS_WIN
    QString filepath = QApplication::applicationDirPath() + "/" + filename;
#elif defined Q_OS_MACOS
    QString filepath = QApplication::applicationDirPath() + "/../Resources/" + filename;
#else // Q_OS_LINUX and others
    QString filepath = QApplication::applicationDirPath() + "/" + filename;
#endif
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        if(errorText.isEmpty())
        {
            textEdit->setPlainText(QString(tr("Could not open file %1")).arg(filepath));
        }
        else
        {
            textEdit->setPlainText(errorText);
        }
    }
    else
    {
        QString content = file.readAll();
        if(useMarkdown)
        {
            textEdit->setMarkdown(content);
        }
        else
        {
            textEdit->setPlainText(content);
        }
    }
}

void TextFileDialog::setErrorText(QString errortext)
{
    errorText = errortext;
}
