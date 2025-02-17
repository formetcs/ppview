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
 * \file textfiledialog.h
 *
 * \brief Declaration of the TextFileDialog class.
 */

#ifndef TEXTFILEDIALOG_H
#define TEXTFILEDIALOG_H

#include <QDialog>

class QDialogButtonBox;
class QTextEdit;


/*!
 * \brief Class to load text from a file and display it in a dialog.
 *
 * It is used mainly to show license or readme files.
 */
class TextFileDialog : public QDialog
{
    Q_OBJECT
public:
    /*!
     * \brief Constructs a new TextFileDialog.
     *
     * \param parent the parent widget
     */
    TextFileDialog(QWidget* parent = 0);

    /*!
     * \brief Load the file which should be displayed.
     *
     * The file must be located in the same directory as the application binary on Windows and Linux,
     * or in the Resources directory inside the application bundle on macOS.
     * It is allowed to specify paths relative to these locations. Absolute path values are not supported.
     *
     * \param filename the file name
     * \param useMarkdown true to interpret the text as Markdown, false to interpret it as plain text
     */
    void loadFile(QString filename, bool useMarkdown = false);

    /*!
     * \brief Set the error text that is shown instead of the file content if the file could not be loaded.
     *
     * It no user-defined error text is set, a default text is used.
     *
     * \param errortext the error text to display
     */
    void setErrorText(QString errortext);


private:
    QTextEdit* textEdit; /*!< \brief The TextEdit widget showing the text. */
    QDialogButtonBox* buttonBox; /*!< \brief The button box containing the Close button. */

    QString errorText; /*!< \brief User-defined error text if a file could not be loaded. */
};

#endif // TEXTFILEDIALOG_H
