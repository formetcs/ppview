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

#include "preferences.h"
#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"

#include <QColorDialog>
#include <QDir>

#define LAYOUT_START "[Layout:Start]"
#define LAYOUT_END "[Layout:End]"
#define LAYOUT_BOTH "[Layout:Both]"

PreferencesDialog::PreferencesDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PreferencesDialog)
{
    punktObjektNames << "GEO_Punkt" << "GEO_Knoten" << "TOP_Knoten" <<
        "Datenpunkt" << "ZUB_Bereichsgrenze" << "Bahnsteig_Zugang" << "BUE_Anlage" <<
        "BUE_Gefahrraum_Eckpunkt" << "BUE_Kante" << "GFR_Tripelspiegel" <<
        "Schrankenantrieb" << "Sonstiger_Punkt" << "Hoehenpunkt" << "Technischer_Punkt" <<
        "Ueberhoehung" << "FMA_Element" << "FMA_Komponente" << "Zugeinwirkung" << "PZB_Element" <<
        "Signal" << "Signal_Befestigung" << "Gleis_Abschluss" << "W_Kr_Gsp_Komponente";

    bereichObjektNames  << "GEO_Kante" << "TOP_Kante" << "ZUB_Streckeneigenschaft" <<
        "Stell_Bereich" << "Bahnsteig_Dach" << "Bahnsteig_Kante" <<
        "BUE_Gleisbezogener_Gefahrraum" << "Fstr_Fahrweg" << "Geschwindigkeitsprofil" <<
        "Strecke" << "Technischer_Bereich" << "Gleis_Abschnitt" << "Gleis_Art" << "Gleis_Baubereich" <<
        "Gleis_Bezeichnung" << "Gleis_Fahrbahn" << "Gleis_Lichtraum" << "Gleis_Schaltgruppe";

    ui->setupUi(this);
    loadSettings();

    connect(ui->listWidgetObjectTypes, SIGNAL(itemSelectionChanged()), this, SLOT(handleObjectListSelection()));
    connect(ui->pushButtonSetLineColor, SIGNAL(clicked()), this, SLOT(handlePushButtonSetLineColorClicked()));
    connect(ui->pushButtonSetFillColor, SIGNAL(clicked()), this, SLOT(handlePushButtonSetFillColorClicked()));
    connect(ui->spinBoxPenWidth, SIGNAL(valueChanged(int)), this, SLOT(penPropertiesChanged()));
    connect(ui->comboBoxPenStyle, SIGNAL(currentIndexChanged(int)), this, SLOT(penPropertiesChanged()));
    connect(ui->pushButtonSetForegroundColor, SIGNAL(clicked()), this, SLOT(handlePushButtonSetForegroundColorClicked()));
    connect(ui->pushButtonSetBackgroundColor, SIGNAL(clicked()), this, SLOT(handlePushButtonSetBackgroundColorClicked()));
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

QStringList PreferencesDialog::getAvailableLanguages()
{
#if defined Q_OS_WIN
    QDir dir(QApplication::applicationDirPath() + "/translations");
#elif defined Q_OS_MACOS
    QDir dir(QApplication::applicationDirPath() + "/../Resources/translations");
#else // Q_OS_LINUX and others
    QDir dir(QApplication::applicationDirPath() + "/translations");
#endif
    QStringList filters;
    filters << "ppview_*.qm";
    QStringList items = dir.entryList(filters, QDir::Files);
    items.replaceInStrings("ppview_", "");
    items.replaceInStrings(".qm", "");
    return items;
}

void PreferencesDialog::loadSettings()
{
    Preferences* prefs = Preferences::getInstance();

    ui->spinBoxZoomStep->setValue(prefs->getZoomStep());
    ui->spinBoxMinZoom->setValue(prefs->getMinZoom());
    ui->spinBoxRotateStep->setValue(prefs->getRotateStep());
    ui->spinBoxMaxRecursionDepth->setValue(prefs->getMaxRecursionDepth());

    QStringList languageList = getAvailableLanguages();
    QString language = prefs->getLanguage();
    int index = languageList.indexOf(language);
    ui->comboBoxLanguage->addItems(languageList);
    ui->comboBoxLanguage->setCurrentIndex(index);

    ui->listWidgetObjectTypes->addItem(new QListWidgetItem(tr("[Combined View: Start]")));
    ui->listWidgetObjectTypes->addItem(new QListWidgetItem(tr("[Combined View: End]")));
    ui->listWidgetObjectTypes->addItem(new QListWidgetItem(tr("[Combined View: Both]")));
    changedPens.insert(LAYOUT_START, prefs->getGraphicsViewStartPen());
    changedBrushes.insert(LAYOUT_START, prefs->getGraphicsViewStartBrush());
    changedPens.insert(LAYOUT_END, prefs->getGraphicsViewEndPen());
    changedBrushes.insert(LAYOUT_END, prefs->getGraphicsViewEndBrush());
    changedPens.insert(LAYOUT_BOTH, prefs->getGraphicsViewBothPen());
    changedBrushes.insert(LAYOUT_BOTH, prefs->getGraphicsViewBothBrush());

    for(int i = 0; i < punktObjektNames.count(); ++i)
    {
        QString currentName = punktObjektNames.at(i);
        changedPens.insert(currentName, prefs->getGraphicsViewPen(currentName));
        changedBrushes.insert(currentName, prefs->getGraphicsViewBrush(currentName));
        ui->listWidgetObjectTypes->addItem(new QListWidgetItem(currentName));
    }
    for(int i = 0; i < bereichObjektNames.count(); ++i)
    {
        QString currentName = bereichObjektNames.at(i);
        changedPens.insert(currentName, prefs->getGraphicsViewPen(currentName));
        ui->listWidgetObjectTypes->addItem(new QListWidgetItem(currentName));
    }

    ui->listWidgetObjectListItems->item(0)->setForeground(prefs->getObjectListFgStartBrush());
    ui->listWidgetObjectListItems->item(0)->setBackground(prefs->getObjectListBgStartBrush());
    ui->listWidgetObjectListItems->item(1)->setForeground(prefs->getObjectListFgEndBrush());
    ui->listWidgetObjectListItems->item(1)->setBackground(prefs->getObjectListBgEndBrush());
    ui->listWidgetObjectListItems->item(2)->setForeground(prefs->getObjectListFgBothBrush());
    ui->listWidgetObjectListItems->item(2)->setBackground(prefs->getObjectListBgBothBrush());
}

void PreferencesDialog::saveSettings()
{
    Preferences* prefs = Preferences::getInstance();

    prefs->setZoomStep(ui->spinBoxZoomStep->value());
    prefs->setMinZoom(ui->spinBoxMinZoom->value());
    prefs->setRotateStep(ui->spinBoxRotateStep->value());
    prefs->setMaxRecursionDepth(ui->spinBoxMaxRecursionDepth->value());

    prefs->setLanguage(ui->comboBoxLanguage->currentText());

    prefs->setGraphicsViewStartPen(changedPens.value(LAYOUT_START));
    prefs->setGraphicsViewStartBrush(changedBrushes.value(LAYOUT_START));
    prefs->setGraphicsViewEndPen(changedPens.value(LAYOUT_END));
    prefs->setGraphicsViewEndBrush(changedBrushes.value(LAYOUT_END));
    prefs->setGraphicsViewBothPen(changedPens.value(LAYOUT_BOTH));
    prefs->setGraphicsViewBothBrush(changedBrushes.value(LAYOUT_BOTH));

    for(int i = 0; i < punktObjektNames.count(); ++i)
    {
        QString currentName = punktObjektNames.at(i);
        prefs->setGraphicsViewPen(currentName, changedPens.value(currentName));
        prefs->setGraphicsViewBrush(currentName, changedBrushes.value(currentName));
    }
    for(int i = 0; i < bereichObjektNames.count(); ++i)
    {
        QString currentName = bereichObjektNames.at(i);
        prefs->setGraphicsViewPen(currentName, changedPens.value(currentName));
    }

    prefs->setObjectListFgStartBrush(ui->listWidgetObjectListItems->item(0)->foreground());
    prefs->setObjectListBgStartBrush(ui->listWidgetObjectListItems->item(0)->background());
    prefs->setObjectListFgEndBrush(ui->listWidgetObjectListItems->item(1)->foreground());
    prefs->setObjectListBgEndBrush(ui->listWidgetObjectListItems->item(1)->background());
    prefs->setObjectListFgBothBrush(ui->listWidgetObjectListItems->item(2)->foreground());
    prefs->setObjectListBgBothBrush(ui->listWidgetObjectListItems->item(2)->background());
}

QPen PreferencesDialog::getSelectedPen()
{
    Preferences* prefs = Preferences::getInstance();
    QList<QListWidgetItem*> selectedList = ui->listWidgetObjectTypes->selectedItems();
    if(selectedList.isEmpty())
    {
        return QPen();
    }
    QListWidgetItem* selectedItem = selectedList.at(0); // No multiple selection -> only first item
    if(ui->listWidgetObjectTypes->row(selectedItem) == 0) // CombinedView: Start
    {
        return changedPens.value(LAYOUT_START, prefs->getGraphicsViewStartPen());
    }
    else if(ui->listWidgetObjectTypes->row(selectedItem) == 1) // CombinedView: End
    {
        return changedPens.value(LAYOUT_END, prefs->getGraphicsViewEndPen());
    }
    else if(ui->listWidgetObjectTypes->row(selectedItem) == 2) // CombinedView: Both
    {
        return changedPens.value(LAYOUT_BOTH, prefs->getGraphicsViewBothPen());
    }
    else // Selection of an object name
    {
        QString itemtext = selectedItem->text();
        return changedPens.value(itemtext, prefs->getGraphicsViewPen(itemtext));
    }
}

void PreferencesDialog::setSelectedPen(QPen p)
{
    QList<QListWidgetItem*> selectedList = ui->listWidgetObjectTypes->selectedItems();
    if(selectedList.isEmpty())
    {
        return;
    }
    QListWidgetItem* selectedItem = selectedList.at(0); // No multiple selection -> only first item
    if(ui->listWidgetObjectTypes->row(selectedItem) == 0) // CombinedView: Start
    {
        changedPens.insert(LAYOUT_START, p);
    }
    else if(ui->listWidgetObjectTypes->row(selectedItem) == 1) // CombinedView: End
    {
        changedPens.insert(LAYOUT_END, p);
    }
    else if(ui->listWidgetObjectTypes->row(selectedItem) == 2) // CombinedView: Both
    {
        changedPens.insert(LAYOUT_BOTH, p);
    }
    else // Selection of an object name
    {
        QString itemtext = selectedItem->text();
        changedPens.insert(itemtext, p);
    }
}

QBrush PreferencesDialog::getSelectedBrush()
{
    Preferences* prefs = Preferences::getInstance();
    QList<QListWidgetItem*> selectedList = ui->listWidgetObjectTypes->selectedItems();
    if(selectedList.isEmpty())
    {
        return QBrush();
    }
    QListWidgetItem* selectedItem = selectedList.at(0); // No multiple selection -> only first item
    if(ui->listWidgetObjectTypes->row(selectedItem) == 0) // CombinedView: Start
    {
        return changedBrushes.value(LAYOUT_START, prefs->getGraphicsViewStartBrush());
    }
    else if(ui->listWidgetObjectTypes->row(selectedItem) == 1) // CombinedView: End
    {
        return changedBrushes.value(LAYOUT_END, prefs->getGraphicsViewEndBrush());
    }
    else if(ui->listWidgetObjectTypes->row(selectedItem) == 2) // CombinedView: Both
    {
        return changedBrushes.value(LAYOUT_BOTH, prefs->getGraphicsViewBothBrush());
    }
    else // Selection of an object name
    {
        QString itemtext = selectedItem->text();
        return changedBrushes.value(itemtext, prefs->getGraphicsViewBrush(itemtext));
    }
}

void PreferencesDialog::setSelectedBrush(QBrush b)
{
    QList<QListWidgetItem*> selectedList = ui->listWidgetObjectTypes->selectedItems();
    if(selectedList.isEmpty())
    {
        return;
    }
    QListWidgetItem* selectedItem = selectedList.at(0); // No multiple selection -> only first item
    if(ui->listWidgetObjectTypes->row(selectedItem) == 0) // CombinedView: Start
    {
        changedBrushes.insert(LAYOUT_START, b);
    }
    else if(ui->listWidgetObjectTypes->row(selectedItem) == 1) // CombinedView: End
    {
        changedBrushes.insert(LAYOUT_END, b);
    }
    else if(ui->listWidgetObjectTypes->row(selectedItem) == 2) // CombinedView: Both
    {
        changedBrushes.insert(LAYOUT_BOTH, b);
    }
    else // Selection of an object name
    {
        QString itemtext = selectedItem->text();
        changedBrushes.insert(itemtext, b);
    }
}

void PreferencesDialog::handleObjectListSelection()
{
    if(ui->listWidgetObjectTypes->selectedItems().isEmpty())
    {
        return;
    }
    QPen pen = getSelectedPen();
    QBrush brush = getSelectedBrush();
    QPalette palette1;
    palette1.setColor(QPalette::Window, pen.color());
    ui->frameLineColor->setPalette(palette1);
    ui->spinBoxPenWidth->setValue(pen.width());
    int penStyleIndex;
    if(pen.style() == Qt::SolidLine)
    {
        penStyleIndex = 0;
    }
    else if(pen.style() == Qt::DashLine)
    {
        penStyleIndex = 1;
    }
    else if(pen.style() == Qt::DotLine)
    {
        penStyleIndex = 2;
    }
    else // pen.style() == Qt::DashDotLine
    {
        penStyleIndex = 3;
    }
    ui->comboBoxPenStyle->setCurrentIndex(penStyleIndex);
    QPalette palette2;
    palette2.setColor(QPalette::Window, brush.color());
    ui->frameFillColor->setPalette(palette2);

    QList<QListWidgetItem*> selectedList = ui->listWidgetObjectTypes->selectedItems();
    QListWidgetItem* selectedItem = selectedList.at(0); // No multiple selection -> only first item
    QString itemtext = selectedItem->text();
    if(bereichObjektNames.contains(itemtext))
    {
        ui->pushButtonSetFillColor->setEnabled(false);
        ui->frameFillColor->setEnabled(false);
    }
    else
    {
        ui->pushButtonSetFillColor->setEnabled(true);
        ui->frameFillColor->setEnabled(true);
    }
}

void PreferencesDialog::handlePushButtonSetLineColorClicked()
{
    handleSetLayoutColor(true);
}

void PreferencesDialog::handlePushButtonSetFillColorClicked()
{
    handleSetLayoutColor(false);
}

void PreferencesDialog::handlePushButtonSetForegroundColorClicked()
{
    handleSetObjectListColor(true);
}

void PreferencesDialog::handlePushButtonSetBackgroundColorClicked()
{
    handleSetObjectListColor(false);
}

void PreferencesDialog::penPropertiesChanged()
{
    QPen pen = getSelectedPen();
    pen.setWidth(ui->spinBoxPenWidth->value());
    Qt::PenStyle penstyle;
    if(ui->comboBoxPenStyle->currentIndex() == 0)
    {
        penstyle = Qt::SolidLine;
    }
    else if(ui->comboBoxPenStyle->currentIndex() == 1)
    {
        penstyle = Qt::DashLine;
    }
    else if(ui->comboBoxPenStyle->currentIndex() == 2)
    {
        penstyle = Qt::DotLine;
    }
    else
    {
        penstyle = Qt::DashDotLine;
    }
    pen.setStyle(penstyle);
    setSelectedPen(pen);
}

void PreferencesDialog::handleSetLayoutColor(bool linecolor)
{
    QPen pen = getSelectedPen();
    QBrush brush = getSelectedBrush();
    QColor resultColor;
    if(linecolor)
    {
        resultColor = QColorDialog::getColor(pen.color(), this, tr("Set Line Color"));
    }
    else
    {
        resultColor = QColorDialog::getColor(brush.color(), this, tr("Set Fill Color"));
    }
    if(!resultColor.isValid())
    {
        return;
    }
    if(linecolor)
    {
        pen.setColor(resultColor);
        setSelectedPen(pen);
    }
    else
    {
        brush.setColor(resultColor);
        setSelectedBrush(brush);
    }
    handleObjectListSelection(); // to update the color boxes
}

void PreferencesDialog::handleSetObjectListColor(bool foreground)
{
    QList<QListWidgetItem*> selectedList = ui->listWidgetObjectListItems->selectedItems();
    if(selectedList.isEmpty())
    {
        return;
    }
    QListWidgetItem* selectedItem = selectedList.at(0); // No multiple selection -> only first item
    QBrush brush;
    if(foreground)
    {
        brush = selectedItem->foreground();
    }
    else
    {
        brush = selectedItem->background();
    }
    QColor resultColor = QColorDialog::getColor(brush.color(), this, tr("Set Color"));
    if(!resultColor.isValid())
    {
        return;
    }
    brush.setColor(resultColor);
    if(foreground)
    {
        selectedItem->setForeground(brush);
    }
    else
    {
        selectedItem->setBackground(brush);
    }
}
