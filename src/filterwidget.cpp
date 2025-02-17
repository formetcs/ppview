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
 * \file filterwidget.cpp
 *
 * \brief Implementation of the FilterWidget class.
 */

#include <QtGui>
#include <QtWidgets>
#include "filterwidget.h"
#include "preferences.h"

FilterWidget::FilterWidget()
{
    QGroupBox * gbAnsteuerungElement = new QGroupBox("Ansteuerung_Element");
    QGroupBox * gbBahnsteige = new QGroupBox("Bahnsteige");
    QGroupBox * gbBahnuebergang = new QGroupBox("Bahnuebergang");
    QGroupBox * gbEtcs = new QGroupBox("Balisentechnik_ETCS");
    QGroupBox * gbFahrstrasse = new QGroupBox("Fahrstrasse");
    QGroupBox * gbGeodaten = new QGroupBox("Geodaten");
    QGroupBox * gbGleis = new QGroupBox("Gleis");
    QGroupBox * gbOrtung = new QGroupBox("Ortung");
    QGroupBox * gbPzb = new QGroupBox("PZB");
    QGroupBox * gbSignale = new QGroupBox("Signale");
    QGroupBox * gbWeichenUndGleissperren = new QGroupBox("Weichen_und_Gleissperren");

    QVBoxLayout* layoutAnsteuerungElement = new QVBoxLayout;
    QVBoxLayout* layoutBahnsteige = new QVBoxLayout;
    QVBoxLayout* layoutBahnuebergang = new QVBoxLayout;
    QVBoxLayout* layoutEtcs = new QVBoxLayout;
    QVBoxLayout* layoutFahrstrasse = new QVBoxLayout;
    QVBoxLayout* layoutGeodaten = new QVBoxLayout;
    QVBoxLayout* layoutGleis = new QVBoxLayout;
    QVBoxLayout* layoutOrtung = new QVBoxLayout;
    QVBoxLayout* layoutPzb = new QVBoxLayout;
    QVBoxLayout* layoutSignale = new QVBoxLayout;
    QVBoxLayout* layoutWeichenUndGleissperren = new QVBoxLayout;

    QVBoxLayout* masterlayout = new QVBoxLayout;
    buttongroup = new QButtonGroup(this);
    buttongroup->setExclusive(false);

    // Ansteuerung_Element
    addCheckBox("Stell_Bereich", cbStellBereich, layoutAnsteuerungElement);
    // Bahnsteige
    addCheckBox("Bahnsteig_Dach", cbBahnsteigDach, layoutBahnsteige);
    addCheckBox("Bahnsteig_Kante", cbBahnsteigKante, layoutBahnsteige);
    addCheckBox("Bahnsteig_Zugang", cbBahnsteigZugang, layoutBahnsteige);
    // Bahnuebergang
    addCheckBox("BUE_Anlage", cbBueAnlage, layoutBahnuebergang);
    addCheckBox("BUE_Gefahrraum_Eckpunkt", cbBueGefahrraumEckpunkt, layoutBahnuebergang);
    addCheckBox("BUE_Gleisbezogener_Gefahrraum", cbBueGleisbezogenerGefahrraum, layoutBahnuebergang);
    addCheckBox("BUE_Kante", cbBueKante, layoutBahnuebergang);
    addCheckBox("GFR_Tripelspiegel", cbGfrTripelspiegel, layoutBahnuebergang);
    addCheckBox("Schrankenantrieb", cbSchrankenantrieb, layoutBahnuebergang);
    // Balisentechnik_ETCS
    addCheckBox("Datenpunkt", cbDatenpunkt, layoutEtcs);
    addCheckBox("ZUB_Bereichsgrenze", cbZubBereichsgrenze, layoutEtcs);
    addCheckBox("ZUB_Streckeneigenschaft", cbZubStreckeneigenschaft, layoutEtcs);
    // Fahrstrasse
    addCheckBox("Fstr_Fahrweg", cbFstrFahrweg, layoutFahrstrasse);
    addCheckBox("Sonstiger_Punkt", cbSonstigerPunkt, layoutFahrstrasse);
    // Geodaten
    addCheckBox("GEO_Kante", cbGeoKante, layoutGeodaten);
    addCheckBox("GEO_Knoten", cbGeoKnoten, layoutGeodaten);
    addCheckBox("GEO_Punkt", cbGeoPunkt, layoutGeodaten);
    addCheckBox("Geschwindigkeitsprofil", cbGeschwindigkeitsprofil, layoutGeodaten);
    addCheckBox("Hoehenpunkt", cbHoehenpunkt, layoutGeodaten);
    addCheckBox("Strecke", cbStrecke, layoutGeodaten);
    addCheckBox("Technischer_Bereich", cbTechnischerBereich, layoutGeodaten);
    addCheckBox("Technischer_Punkt", cbTechnischerPunkt, layoutGeodaten);
    addCheckBox("TOP_Kante", cbTopKante, layoutGeodaten);
    addCheckBox("TOP_Knoten", cbTopKnoten, layoutGeodaten);
    addCheckBox("Ueberhoehung", cbUeberhoehung, layoutGeodaten);
    // Gleis
    addCheckBox("Gleis_Abschnitt", cbGleisAbschnitt, layoutGleis);
    addCheckBox("Gleis_Art", cbGleisArt, layoutGleis);
    addCheckBox("Gleis_Baubereich", cbGleisBaubereich, layoutGleis);
    addCheckBox("Gleis_Bezeichnung", cbGleisBezeichnung, layoutGleis);
    addCheckBox("Gleis_Fahrbahn", cbGleisFahrbahn, layoutGleis);
    addCheckBox("Gleis_Lichtraum", cbGleisLichtraum, layoutGleis);
    addCheckBox("Gleis_Schaltgruppe", cbGleisSchaltgruppe, layoutGleis);
    // Ortung
    addCheckBox("FMA_Element", cbFmaElement, layoutOrtung);
    addCheckBox("FMA_Komponente", cbFmaKomponente, layoutOrtung);
    addCheckBox("Zugeinwirkung", cbZugeinwirkung, layoutOrtung);
    // PZB
    addCheckBox("PZB_Element", cbPzbElement, layoutPzb);
    // Signale
    addCheckBox("Signal", cbSignal, layoutSignale);
    addCheckBox("Signal_Befestigung", cbSignalBefestigung, layoutSignale);
    // Weichen_und_Gleissperren
    addCheckBox("Gleis_Abschluss", cbGleisAbschluss, layoutWeichenUndGleissperren);
    addCheckBox("W_Kr_Gsp_Komponente", cbWKrGspKomponente, layoutWeichenUndGleissperren);


    gbAnsteuerungElement->setLayout(layoutAnsteuerungElement);
    gbBahnsteige->setLayout(layoutBahnsteige);
    gbBahnuebergang->setLayout(layoutBahnuebergang);
    gbEtcs->setLayout(layoutEtcs);
    gbFahrstrasse->setLayout(layoutFahrstrasse);
    gbGeodaten->setLayout(layoutGeodaten);
    gbGleis->setLayout(layoutGleis);
    gbOrtung->setLayout(layoutOrtung);
    gbPzb->setLayout(layoutPzb);
    gbSignale->setLayout(layoutSignale);
    gbWeichenUndGleissperren->setLayout(layoutWeichenUndGleissperren);

    masterlayout->addWidget(gbAnsteuerungElement);
    masterlayout->addWidget(gbBahnsteige);
    masterlayout->addWidget(gbBahnuebergang);
    masterlayout->addWidget(gbEtcs);
    masterlayout->addWidget(gbFahrstrasse);
    masterlayout->addWidget(gbGeodaten);
    masterlayout->addWidget(gbGleis);
    masterlayout->addWidget(gbOrtung);
    masterlayout->addWidget(gbPzb);
    masterlayout->addWidget(gbSignale);
    masterlayout->addWidget(gbWeichenUndGleissperren);
    masterlayout->addStretch();

    QWidget* widget = new QWidget();
    widget->setLayout(masterlayout);
    setWidget(widget);

    connect(buttongroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(changeFilter(QAbstractButton*)));

}

void FilterWidget::readSettings()
{
    for(int i = 0; i < buttongroup->buttons().count(); i++)
    {
        QCheckBox* cb = (QCheckBox*) buttongroup->buttons().at(i);
        bool enabled = Preferences::getInstance()->getFilterSetting(cb->text());
        cb->setChecked(enabled);
    }
}

void FilterWidget::writeSettings()
{
    for(int i = 0; i < buttongroup->buttons().count(); i++)
    {
        QCheckBox* cb = (QCheckBox*) buttongroup->buttons().at(i);
        Preferences::getInstance()->setFilterSetting(cb->text(), cb->isChecked());
    }
}

void FilterWidget::updateFilterSettings()
{
    for(int i = 0; i < buttongroup->buttons().count(); i++)
    {
        QCheckBox* cb = (QCheckBox*) buttongroup->buttons().at(i);
        emit filterStateChanged(cb->text(), cb->isChecked());
    }
}

void FilterWidget::setAllFilters(bool enabled)
{
    for(int i = 0; i < buttongroup->buttons().count(); i++)
    {
        QCheckBox* cb = (QCheckBox*) buttongroup->buttons().at(i);
        cb->setChecked(enabled);
        emit filterStateChanged(cb->text(), cb->isChecked());
    }
}

QList<FilterState> FilterWidget::getFilterState()
{
    QList<FilterState> returnlist;
    for(int i = 0; i < buttongroup->buttons().count(); i++)
    {
        QCheckBox* cb = (QCheckBox*) buttongroup->buttons().at(i);
        FilterState fs;
        fs.name = cb->text();
        fs.state = cb->isChecked();
        returnlist.append(fs);
    }

    return returnlist;
}

void FilterWidget::selectAllFilters()
{
    setAllFilters(true);
}

void FilterWidget::deselectAllFilters()
{
    setAllFilters(false);
}

void FilterWidget::addCheckBox(const QString& text, QCheckBox* cb, QVBoxLayout* layout)
{
    cb = new QCheckBox(text, this);
    layout->addWidget(cb);
    buttongroup->addButton(cb);

    cb->setChecked(true);
}

void FilterWidget::changeFilter(QAbstractButton * button)
{
    QCheckBox* cb = (QCheckBox*) button;
    emit filterStateChanged(cb->text(), cb->isChecked());
}
