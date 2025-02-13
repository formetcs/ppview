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


/*!
 * \brief Dialog class handling the complete SMT verification task.
 *
 * Opens a dialog where the user can load a file containing a testcase in the SMT-LIB format.
 * The file must contain special directives prepended by a @ character that specify the logic
 * to use, the required and optional PlanPro types and the logical variables which constitute
 * the potential counterexamples.
 * After the file has been loaded, the user can select the PlanPro types that should be considered
 * for the test and a SMT solver to execute the test. The solver tries to find an assignment for
 * the counterexample variables that make the plan invalid. If counterexamples are found, they
 * are displayed in the search result window and the user can select and inspect them as usual.
 *
 * The dialog stays modeless so the user can switch between dialog and main window.
 */
class SmtVerifierDialog : public QDialog
{
    Q_OBJECT

public:
    /*!
     * \brief Creates a new SmtVerifierDialog.
     *
     * \param doc the PlanPro document
     * \param parent the parent widget
     */
    explicit SmtVerifierDialog(PlanProDocument* doc, QWidget* parent = nullptr);

    /*!
     * \brief Destructor.
     *
     * Also kills the SMT process.
     */
    ~SmtVerifierDialog();

    /*!
     * \brief Kills the SMT process and resets all settings to default values.
     */
    void reset();

    /*!
     * \brief Get the UUIDs of all found counterexamples.
     *
     * \return the list of UUID strings from all counterexamples
     */
    QStringList getCounterexampleList();

signals:
    /*!
     * \brief Signal to indicate that a counterexample has been found.
     *
     * The signal is emitted whenever the counterexample list changes, even when it is cleared after a reset.
     * This allows connected views to clear their output windows.
     */
    void counterexamplesFound();

private slots:
    /*!
     * \brief Target slot for the Load Testcase button.
     *
     * Shows a Open File dialog and loads the selected file into a buffer.
     * Also processes all "@" tags and stores their values.
     * Any include directives are resolved immediately and included into the file buffer at the right position.
     */
    void openTestcase();

    /*!
     * \brief Target slot for the Save As SMT-LIB File button.
     *
     * Shows a Save File dialog, creates the SMT query and saves it to the selected file.
     * The content is the same that would be sent to the SMT process.
     */
    void saveAsSmtlibFile();

    /*!
     * \brief Target slot for the Verify button.
     *
     * Creates the SMT query and sends it to the selected SMT solver.
     */
    void startVerification();

    /*!
     * \brief Handles all output from the SMT process.
     *
     * This slot is connected to the standard output of the active SMT process.
     * It it receives a "unsat" or "unknown" response, the elapsed time is measured and displayed to the user.
     * If it receives a "sat" response, an additional "get-value" query is created, containing all specified variables,
     * and sent to the process. It it receives the counterexamples, the UUIDs are extracted and stored in the counterexample list.
     */
    void handleSmtOutput();

    /*!
     * \brief Target slot for the Stop SMT Process button.
     *
     * Kills a running SMT process.
     */
    void stopProcess();

private:
    /*!
     * \brief Create the SMT query.
     *
     * This method creates the query string in the correct order:
     *
     * - Standard and user-defined options (from @option)
     * - set-logic statement (from @logic)
     * - PlanPro enum types as ADTs
     * - UUID enumeration as ADT (collected from the expanded PlanPro model)
     * - Declaration of the PlanPro datatypes
     * - Expansion of the PlanPro model (selected types only)
     * - Definition of the inPlan function (to restrict variable instantiations to the existing objects)
     * - Content of the test case file (also containing all included content)
     * - check-sat statement
     *
     * \return the string containing the query to be sent to the SMT solver
     */
    QString createSmtQuery();

    /*!
     * \brief Include an additional file into the main test case file.
     *
     * This method is invoked when a "@include" statement occurs in the main file.
     * It resolves the path (which has to be specified relative to the including file),
     * loads the file and returns its contents. The calling method has to include the
     * returned content into the content of the main file.
     *
     * The included file cannot have additional includes or other "@" statements to process.
     *
     * \param filename the name of the included file, with path relative to the parent file
     * \param parentFile the file where to include with full path
     * \return the content of the included file
     */
    QString includeFile(QString filename, QString parentFile);

    /*!
     * \brief Check if a PlanPro object type is selected by the user in the dialog window.
     *
     * \param type the type that should be checked
     * \return true if the type is selected, otherwise false
     */
    bool isTypeSelected(QString type);

    Ui::SmtVerifierDialog* ui; /*!< \brief The Qt Designer dialog. */
    PlanProDocument* document; /*!< \brief The PlanPro document. */

    QProcess* smtprocess; /*!< \brief The process running the SMT solver. */
    int msecs; /*!< \brief The milliseconds since start of the day when the verification was started. */
    QString logic; /*!< \brief The SMT logic to use. */
    QStringList variables; /*!< \brief The counterexample variables. */
    QString fileContent; /*!< \brief The content of the test case file, also containing all included file content. */
    QString options; /*!< \brief Additional options for the SMT process, to be added before the set-logic instruction. */
    QStringList counterexampleList; /*!< \brief The list of UUIDs from all found counterexamples. */
};

#endif // SMTVERIFIERDIALOG_H
