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

struct FilterState
{
    QString name;
    bool state;
};

class FilterWidget : public QScrollArea
{
    Q_OBJECT
public:
    FilterWidget();
    void updateFilterSettings();
    void setAllFilters(bool enabled);
    QList<FilterState> getFilterState();
    void readSettings();
    void writeSettings();

public slots:
    void selectAllFilters();
    void deselectAllFilters();

signals:
    void filterStateChanged(QString key, bool state);

private slots:
    void changeFilter(QAbstractButton* button);


private:
    void addCheckBox(const QString& text, QCheckBox* cb, QVBoxLayout* layout);

    // Ansteuerung_Element
    QCheckBox* cbStellBereich;
    // Bahnsteige
    QCheckBox* cbBahnsteigDach;
    QCheckBox* cbBahnsteigKante;
    QCheckBox* cbBahnsteigZugang;
    // Bahnuebergang
    QCheckBox* cbBueAnlage;
    QCheckBox* cbBueGefahrraumEckpunkt;
    QCheckBox* cbBueGleisbezogenerGefahrraum;
    QCheckBox* cbBueKante;
    QCheckBox* cbGfrTripelspiegel;
    QCheckBox* cbSchrankenantrieb;
    // Balisentechnik_ETCS
    QCheckBox* cbDatenpunkt;
    QCheckBox* cbZubBereichsgrenze;
    QCheckBox* cbZubStreckeneigenschaft;
    // Fahrstrasse
    QCheckBox* cbFstrFahrweg;
    QCheckBox* cbSonstigerPunkt;
    // Geodaten
    QCheckBox* cbGeoKante;
    QCheckBox* cbGeoKnoten;
    QCheckBox* cbGeoPunkt;
    QCheckBox* cbGeschwindigkeitsprofil;
    QCheckBox* cbHoehenpunkt;
    QCheckBox* cbStrecke;
    QCheckBox* cbTechnischerBereich;
    QCheckBox* cbTechnischerPunkt;
    QCheckBox* cbTopKante;
    QCheckBox* cbTopKnoten;
    QCheckBox* cbUeberhoehung;
    // Gleis
    QCheckBox* cbGleisAbschnitt;
    QCheckBox* cbGleisArt;
    QCheckBox* cbGleisBaubereich;
    QCheckBox* cbGleisBezeichnung;
    QCheckBox* cbGleisFahrbahn;
    QCheckBox* cbGleisLichtraum;
    QCheckBox* cbGleisSchaltgruppe;
    // Ortung
    QCheckBox* cbFmaElement;
    QCheckBox* cbFmaKomponente;
    QCheckBox* cbZugeinwirkung;
    // PZB
    QCheckBox* cbPzbElement;
    // Signale
    QCheckBox* cbSignal;
    QCheckBox* cbSignalBefestigung;
    // Weichen_und_Gleissperren
    QCheckBox* cbGleisAbschluss;
    QCheckBox* cbWKrGspKomponente;


    QButtonGroup* buttongroup;
};

#endif // FILTERWIDGET_H
