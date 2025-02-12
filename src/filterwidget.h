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

#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include <QScrollArea>
#include <QList>

class QCheckBox;
class QButtonGroup;
class QAbstractButton;
class QVBoxLayout;


/*!
 * \brief Stucture to combine type name and state of a single filter setting.
 */
struct FilterState
{
    QString name; /*!< \brief The type name for the filter setting. */
    bool state; /*!< \brief The type state for the filter setting. */
};


/*!
 * \brief Widget containing the filter settings for the track layout view.
 *
 * This widget contains checkboxes for each visualizable PlanPro object type, splitted into their functional groups.
 * Visualizable types include the GEO types (graph nodes and edges) and all subtypes of Punkt_Objekt and Bereich_Objekt.
 * If a type is checked, the object is shown in the layout view, otherwise it is hidden.
 * The selection has no impact to other views than the track layout, for example the document structure or object list views.
 */
class FilterWidget : public QScrollArea
{
    Q_OBJECT
public:
    /*!
     * \brief Constructor.
     */
    FilterWidget();

    /*!
     * \brief Emits the filterStateChanged() signal for all checkboxes so that connected views can update their rendering.
     */
    void updateFilterSettings();

    /*!
     * \brief Set all filters to a specific state.
     *
     * \param enabled true to check all checkboxes, false to uncheck all of them
     */
    void setAllFilters(bool enabled);

    /*!
     * \brief Get a list of all filter settings.
     *
     * \return a list of FilterState entries, containing type and state
     */
    QList<FilterState> getFilterState();

    /*!
     * \brief Read the current filter settings from the Preferences.
     */
    void readSettings();

    /*!
     * \brief Write the current filter settings to the Preferences.
     */
    void writeSettings();

public slots:
    /*!
     * \brief Set all filter checkboxes to checked.
     */
    void selectAllFilters();

    /*!
     * \brief Set all filter checkboxes to unchecked.
     */
    void deselectAllFilters();

signals:
    /*!
     * \brief Signal to indicate filter state changes.
     *
     * \param key the type name of the changed filter
     * \param state the new state of the filter
     */
    void filterStateChanged(QString key, bool state);

private slots:
    /*!
     * \brief This method is called when a checkbox is (un)checked and emits the appropriate filterStateChanged() signal.
     *
     * \param button the checkbox whose state has changed
     */
    void changeFilter(QAbstractButton* button);


private:
    /*!
     * \brief Creates the checkboxes and adds them to a layout and to the button group.
     *
     * \param text the label of the checkbox
     * \param cb the checkbox that should be initialized
     * \param layout the layout where the checkbox should be added
     */
    void addCheckBox(const QString& text, QCheckBox* cb, QVBoxLayout* layout);

    // Ansteuerung_Element
    QCheckBox* cbStellBereich; /*!< \brief Checkbox for Stell_Bereich (Group Ansteuerung_Element). */
    // Bahnsteige
    QCheckBox* cbBahnsteigDach; /*!< \brief Checkbox for Bahnsteig_Dach (Group Bahnsteige). */
    QCheckBox* cbBahnsteigKante; /*!< \brief Checkbox for Bahnsteig_Kante (Group Bahnsteige). */
    QCheckBox* cbBahnsteigZugang; /*!< \brief Checkbox for Bahnsteig_Zugang (Group Bahnsteige). */
    // Bahnuebergang
    QCheckBox* cbBueAnlage; /*!< \brief Checkbox for BUE_Anlage (Group Bahnuebergang). */
    QCheckBox* cbBueGefahrraumEckpunkt; /*!< \brief Checkbox for BUE_Gefahrraum_Eckpunkt (Group Bahnuebergang). */
    QCheckBox* cbBueGleisbezogenerGefahrraum; /*!< \brief Checkbox for BUE_Gleisbezogener_Gefahrraum (Group Bahnuebergang). */
    QCheckBox* cbBueKante; /*!< \brief Checkbox for BUE_Kante (Group Bahnuebergang). */
    QCheckBox* cbGfrTripelspiegel; /*!< \brief Checkbox for GFR_Tripelspiegel (Group Bahnuebergang). */
    QCheckBox* cbSchrankenantrieb; /*!< \brief Checkbox for Schrankenantrieb (Group Bahnuebergang). */
    // Balisentechnik_ETCS
    QCheckBox* cbDatenpunkt; /*!< \brief Checkbox for Datenpunkt (Group Balisentechnik_ETCS). */
    QCheckBox* cbZubBereichsgrenze; /*!< \brief Checkbox for ZUB_Bereichsgrenze (Group Balisentechnik_ETCS). */
    QCheckBox* cbZubStreckeneigenschaft; /*!< \brief Checkbox for ZUB_Streckeneigenschaft (Group Balisentechnik_ETCS). */
    // Fahrstrasse
    QCheckBox* cbFstrFahrweg; /*!< \brief Checkbox for Fstr_Fahrweg (Group Fahrstrasse). */
    QCheckBox* cbSonstigerPunkt; /*!< \brief Checkbox for Sonstiger_Punkt (Group Fahrstrasse). */
    // Geodaten
    QCheckBox* cbGeoKante; /*!< \brief Checkbox for GEO_Kante (Group Geodaten). */
    QCheckBox* cbGeoKnoten; /*!< \brief Checkbox for GEO_Knoten (Group Geodaten). */
    QCheckBox* cbGeoPunkt; /*!< \brief Checkbox for GEO_Punkt (Group Geodaten). */
    QCheckBox* cbGeschwindigkeitsprofil; /*!< \brief Checkbox for Geschwindigkeitsprofil (Group Geodaten). */
    QCheckBox* cbHoehenpunkt; /*!< \brief Checkbox for Hoehenpunkt (Group Geodaten). */
    QCheckBox* cbStrecke; /*!< \brief Checkbox for Strecke (Group Geodaten). */
    QCheckBox* cbTechnischerBereich; /*!< \brief Checkbox for Technischer_Bereich (Group Geodaten). */
    QCheckBox* cbTechnischerPunkt; /*!< \brief Checkbox for Technischer_Punkt (Group Geodaten). */
    QCheckBox* cbTopKante; /*!< \brief Checkbox for TOP_Kante (Group Geodaten). */
    QCheckBox* cbTopKnoten; /*!< \brief Checkbox for TOP_Knoten (Group Geodaten). */
    QCheckBox* cbUeberhoehung; /*!< \brief Checkbox for Ueberhoehung (Group Geodaten). */
    // Gleis
    QCheckBox* cbGleisAbschnitt; /*!< \brief Checkbox for Gleis_Abschnitt (Group Gleis). */
    QCheckBox* cbGleisArt; /*!< \brief Checkbox for Gleis_Art (Group Gleis). */
    QCheckBox* cbGleisBaubereich; /*!< \brief Checkbox for Gleis_Baubereich (Group Gleis). */
    QCheckBox* cbGleisBezeichnung; /*!< \brief Checkbox for Gleis_Bezeichnung (Group Gleis). */
    QCheckBox* cbGleisFahrbahn; /*!< \brief Checkbox for Gleis_Fahrbahn (Group Gleis). */
    QCheckBox* cbGleisLichtraum; /*!< \brief Checkbox for Gleis_Lichtraum (Group Gleis). */
    QCheckBox* cbGleisSchaltgruppe; /*!< \brief Checkbox for Gleis_Schaltgruppe (Group Gleis). */
    // Ortung
    QCheckBox* cbFmaElement; /*!< \brief Checkbox for FMA_Element (Group Ortung). */
    QCheckBox* cbFmaKomponente; /*!< \brief Checkbox for FMA_Komponente (Group Ortung). */
    QCheckBox* cbZugeinwirkung; /*!< \brief Checkbox for Zugeinwirkung (Group Ortung). */
    // PZB
    QCheckBox* cbPzbElement; /*!< \brief Checkbox for PZB_Element (Group PZB). */
    // Signale
    QCheckBox* cbSignal; /*!< \brief Checkbox for Signal (Group Signale). */
    QCheckBox* cbSignalBefestigung; /*!< \brief Checkbox for Signal_Befestigung (Group Signale). */
    // Weichen_und_Gleissperren
    QCheckBox* cbGleisAbschluss; /*!< \brief Checkbox for Gleis_Abschluss (Group Weichen_und_Gleissperren). */
    QCheckBox* cbWKrGspKomponente; /*!< \brief Checkbox for W_Kr_Gsp_Komponente (Group Weichen_und_Gleissperren). */


    QButtonGroup* buttongroup; /*!< \brief Button group containing all checkboxes. */
};

#endif // FILTERWIDGET_H
