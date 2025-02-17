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
 * \file preferencesdialog.h
 *
 * \brief Declaration of the PreferencesDialog class.
 */

#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>

/*!
 * \brief Namespace for all Qt Designer generated classes.
 */
namespace Ui {
class PreferencesDialog;
}


/*!
 * \brief Dialog class allowing the user to change the application settings.
 *
 * This class provides a user interface for the global Preferences object.
 * The dialog reads from and writes to the Preferences object only.
 * The Preferences object finally stores the values to disk.
 * The application also gets the needed values from Preferences only and
 * never from PreferencesDialog.
 */
class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    /*!
     * \brief Creates a new PreferencesDialog.
     * \param parent the parent widget
     */
    explicit PreferencesDialog(QWidget* parent = nullptr);

    /*!
     * \brief Destructor.
     */
    ~PreferencesDialog();

    /*!
     * \brief Loads the settings from the global Preferences instance.
     */
    void loadSettings();

    /*!
     * \brief Write the settings to the global Preferences instance.
     *
     * This method is not invoked automatically when the user clicks the OK button.
     * The method that opened the dialog has to test if the return value was QDialog::Accepted and call this method by itself.
     *
     * Does not write the settings to disk. Call Preferences::writeSettings() afterwards to do this.
     */
    void saveSettings();

private slots:
    /*!
     * \brief Target slot for selections in the object type list.
     *
     * This method retrieves the pen and brush for the selected object type with getSelectedPen() and getSelectedBrush()
     * and updates the line width spin box, the style combo box and the color frames.
     * It also enables or disables the Set Fill Color button depending on the selected type.
     */
    void handleObjectListSelection();

    /*!
     * \brief Target slot for clicks on the Set Line Color button.
     *
     * Calls handleSetLayoutColor() with argument true.
     */
    void handlePushButtonSetLineColorClicked();

    /*!
     * \brief Target slot for clicks on the Set Fill Color button.
     *
     * Calls handleSetLayoutColor() with argument false.
     */
    void handlePushButtonSetFillColorClicked();

    /*!
     * \brief Target slot for clicks on the Set Foreground Color button.
     *
     * Calls handleSetObjectListColor() with argument true.
     */
    void handlePushButtonSetForegroundColorClicked();

    /*!
     * \brief Target slot for clicks on the Set Background Color button.
     *
     * Calls handleSetObjectListColor() with argument false.
     */
    void handlePushButtonSetBackgroundColorClicked();

    /*!
     * \brief Target slot for changes on the pen width and pen style.
     *
     * Writes the new properties with setSelectedPen().
     */
    void penPropertiesChanged();

    /*!
     * \brief Target slot for clicks on the Add SMT button.
     *
     * Adds a new SMT solver with id, command and arguments to the list.
     * Has no effect if the list already contains a solver with the same id.
     */
    void addSmt();

    /*!
     * \brief Target slot for clicks on the Remove SMT button.
     *
     * Removes the selected SMT solver from the list.
     */
    void removeSmt();

    /*!
     * \brief Target slot for clicks on the Edit SMT button.
     *
     * Updates the id, command and arguments of the selected SMT solver.
     */
    void editSmt();

    /*!
     * \brief Target slot for selections in the SMT list.
     *
     * Updates the line edit widgets with the id, command and arguments from the selected SMT solver.
     */
    void handleSmtListSelection();

private:
    /*!
     * \brief Handle color changes for the track layout view.
     *
     * This method retrieves the pen and brush for the selected object type with getSelectedPen() and getSelectedBrush(),
     * shows a color dialog to the user and writes back the new colors with setSelectedPen() and setSelectedBrush().
     *
     * \param linecolor true if the line color should be changed, false if the fill color should be changed
     */
    void handleSetLayoutColor(bool linecolor);

    /*!
     * \brief Handle color changes for the object list view.
     *
     * This method retrieves the colors of the selected object directly from the list, shows a color dialog to the user
     * and updated the list items with the new colors.
     *
     * \param foreground true if the foreground color should be changed, false if the background color should be changed
     */
    void handleSetObjectListColor(bool foreground);

    /*!
     * \brief Get the list of languages which are currently installed.
     *
     * This method searches for the "ppview_*.qm" files in the "translations" directory.
     *
     * \return the list of languages.
     */
    QStringList getAvailableLanguages();

    /*!
     * \brief Get the pen for the object type that is currently selected in the object type list.
     *
     * \return the pen for the selected object
     */
    QPen getSelectedPen();

    /*!
     * \brief Set the pen for the object type that is currently selected in the object type list.
     *
     * \param p the pen for the selected object
     */
    void setSelectedPen(QPen p);

    /*!
     * \brief Get the brush for the object type that is currently selected in the object type list.
     *
     * \return the brush for the selected object
     */
    QBrush getSelectedBrush();

    /*!
     * \brief Set the brush for the object type that is currently selected in the object type list.
     *
     * \param b the brush for the selected object
     */
    void setSelectedBrush(QBrush b);

    Ui::PreferencesDialog* ui; /*!< \brief The Qt Designer dialog. */

    QHash<QString,QPen> changedPens; /*!< \brief Hash map containing the currently changed pens. */
    QHash<QString,QBrush> changedBrushes; /*!< \brief Hash map containing the currently changed brushes. */

    QHash<QString,QString> changedSmtCommands; /*!< \brief Hash map containing the currently changed SMT commands. */
    QHash<QString,QString> changedSmtArguments; /*!< \brief Hash map containing the currently changed SMT arguments. */

    QStringList punktObjektNames; /*!< \brief Constant list of all Punkt_Objekt subtype names. */
    QStringList bereichObjektNames; /*!< \brief Constant list of all Bereich_Objekt subtype names. */
};

#endif // PREFERENCESDIALOG_H
