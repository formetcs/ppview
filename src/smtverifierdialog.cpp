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

#include "smtverifierdialog.h"
#include "planprodocument.h"
#include "ui_smtverifierdialog.h"
#include "preferences.h"

#include <QFileDialog>
#include <QProcess>

SmtVerifierDialog::SmtVerifierDialog(PlanProDocument* doc, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::SmtVerifierDialog)
{
    document = doc;
    ui->setupUi(this);
    smtprocess = new QProcess();
    msecs = 0;
    logic = QString("ALL");
    fileContent = QString();
    options = QString();
    variables.clear();
    counterexampleList.clear();
    connect(smtprocess, SIGNAL(readyRead()), this, SLOT(handleSmtOutput()));
    connect(ui->pushButtonLoadTestcase, SIGNAL(clicked()), this, SLOT(openTestcase()));
    connect(ui->pushButtonVerify, SIGNAL(clicked()), this, SLOT(startVerification()));
    connect(ui->pushButtonStopProcess, SIGNAL(clicked()), this, SLOT(stopProcess()));
    connect(ui->pushButtonSaveSmtfile, SIGNAL(clicked()), this, SLOT(saveAsSmtlibFile()));
    ui->pushButtonVerify->setEnabled(false);
    ui->pushButtonSaveSmtfile->setEnabled(false);
}

SmtVerifierDialog::~SmtVerifierDialog()
{
    smtprocess->kill();
    smtprocess->waitForFinished();
    delete smtprocess;
    delete ui;
}

void SmtVerifierDialog::reset()
{
    smtprocess->kill();
    smtprocess->waitForFinished();
    logic = QString("ALL");
    fileContent = QString();
    options = QString();
    variables.clear();
    counterexampleList.clear();
    emit counterexamplesFound(); // to clear the result window
    ui->comboBoxProver->clear();
    ui->listWidgetTypes->clear();
    ui->textEditDescription->clear();
    ui->textEditOutput->clear();
    ui->pushButtonVerify->setEnabled(false);
    ui->pushButtonSaveSmtfile->setEnabled(false);
}

QStringList SmtVerifierDialog::getCounterexampleList()
{
    return counterexampleList;
}

void SmtVerifierDialog::openTestcase()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Load Testcase"),
                                                    QDir::homePath(),
                                                    tr("SMT-LIB Files (*.smt2);;Text Files (*.txt);;All Files (*.*)"));

    if(filename.isEmpty())
        return;

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        ui->textEditOutput->setText(tr("File %1 could not be opened").arg(filename));
        return;
    }

    bool fileValid = false;
    ui->comboBoxProver->clear();
    ui->listWidgetTypes->clear();
    ui->textEditDescription->clear();
    ui->textEditOutput->clear();
    logic = QString("ALL");
    fileContent = QString();
    options = QString();
    variables.clear();
    counterexampleList.clear();
    emit counterexamplesFound(); // to clear the result window

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        fileContent.append(line);
        fileContent.append("\n");
        if(line.contains("@ppview-smt 1"))
        {
            fileValid = true;
        }
        if(line.contains("@description"))
        {
            line.remove("@description");
            line.remove(";");
            QString text = ui->textEditDescription->toPlainText();
            if(!text.isEmpty())
            {
                text.append(" ");
            }
            text.append(line.simplified());
            ui->textEditDescription->setText(text);
        }
        if(line.contains("@provers"))
        {
            line.remove("@provers");
            line.remove(";");
            line = line.simplified();
            ui->comboBoxProver->addItems(line.split(" ", Qt::SkipEmptyParts));
        }
        if(line.contains("@logic"))
        {
            line.remove("@logic");
            line.remove(";");
            logic = line.simplified();
        }
        if(line.contains("@option"))
        {
            line.remove("@option");
            line.remove(";");
            options.append(line.simplified());
            options.append("\n");
        }
        if(line.contains("@include"))
        {
            line.remove("@include");
            line.remove(";");
            line = line.simplified();
            fileContent.append(includeFile(line, filename));
        }
        if(line.contains("@variables"))
        {
            line.remove("@variables");
            line.remove(";");
            line = line.simplified();
            variables = line.split(" ", Qt::SkipEmptyParts);
        }
        if(line.contains("@types-required"))
        {
            line.remove("@types-required");
            line.remove(";");
            line = line.simplified();
            QStringList itemlist = line.split(" ", Qt::SkipEmptyParts);
            for(int i = 0; i < itemlist.count(); ++i)
            {
                QString itemtext = itemlist.at(i);
                QListWidgetItem* lwi = new QListWidgetItem(itemtext);
                lwi->setFlags(Qt::ItemIsUserCheckable);
                lwi->setCheckState(Qt::Checked);
                ui->listWidgetTypes->addItem(lwi);
            }
        }
        if(line.contains("@types-optional"))
        {
            line.remove("@types-optional");
            line.remove(";");
            line = line.simplified();
            QStringList itemlist = line.split(" ", Qt::SkipEmptyParts);
            for(int i = 0; i < itemlist.count(); ++i)
            {
                QString itemtext = itemlist.at(i);
                QListWidgetItem* lwi = new QListWidgetItem(itemtext);
                lwi->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
                lwi->setCheckState(Qt::Unchecked);
                ui->listWidgetTypes->addItem(lwi);
            }
        }
    }
    if(!fileValid)
    {
        ui->textEditOutput->setText(tr("File %1 is no valid test case").arg(filename));
        ui->comboBoxProver->clear();
        ui->listWidgetTypes->clear();
        ui->textEditDescription->clear();
        ui->pushButtonVerify->setEnabled(false);
        ui->pushButtonSaveSmtfile->setEnabled(false);
        logic = QString("ALL");
        fileContent = QString();
        options = QString();
        variables.clear();
        return;
    }
    ui->textEditOutput->append(tr("Ready"));
    ui->pushButtonVerify->setEnabled(true);
    ui->pushButtonSaveSmtfile->setEnabled(true);
}

QString SmtVerifierDialog::includeFile(QString filename, QString parentFile)
{
    QString content = QString();
    QFileInfo fi(parentFile);
    QDir path = fi.dir();
    QString filePathString = path.filePath(filename);
    QFile file(filePathString);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        ui->textEditOutput->append(tr("File %1 could not be opened").arg(filePathString));
        return QString();
    }
    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        content.append(line);
        content.append("\n");
    }
    return content;
}

void SmtVerifierDialog::saveAsSmtlibFile()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save as SMT-LIB File"), QDir::homePath(),
                                             tr("SMT-LIB Files (*.smt2)"));

    if(filename.isEmpty())
        return;

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        ui->textEditOutput->setText(tr("File %1 could not be opened").arg(filename));
        return;
    }
    QTextStream ts(&file);
    QString query = createSmtQuery();
    ts << query << Qt::endl;
    file.close();
}

void SmtVerifierDialog::startVerification()
{
    smtprocess->kill();
    smtprocess->waitForFinished();
    ui->textEditOutput->clear();
    counterexampleList.clear();
    emit counterexamplesFound(); // to clear the result window

    QString smtId = ui->comboBoxProver->currentText();
    Preferences* prefs = Preferences::getInstance();
    QString smtcommand = prefs->getSmtCommand(smtId);
    QString arguments = prefs->getSmtArguments(smtId);
    smtprocess->start(smtcommand, arguments.split(" ", Qt::SkipEmptyParts));
    if(!smtprocess->waitForStarted())
    {
        ui->textEditOutput->setText(tr("Error: SMT process did not start"));
        return;
    }
    QString query = createSmtQuery();
    smtprocess->write(query.toLatin1());
    msecs = QTime::currentTime().msecsSinceStartOfDay();
}

void SmtVerifierDialog::handleSmtOutput()
{
    QString response = smtprocess->readAll();
    int duration = QTime::currentTime().msecsSinceStartOfDay() - msecs;
    if(response.startsWith("sat"))
    {
        ui->textEditOutput->setText(tr("Counterexamples found, see result window for details (Time: %1 msecs)").arg(duration));
        QString querystring = QString("(get-value (");
        for(int i = 0; i < variables.count(); ++i)
        {
            QString variablestring = QString("(id %1) ").arg(variables.at(i));
            querystring.append(variablestring);
        }
        querystring.append("))\n");
        smtprocess->write(querystring.toLatin1());
        return;
    }
    else if(response.startsWith("unsat"))
    {
        ui->textEditOutput->setText(tr("No counterexamples found, plan is valid (Time: %1 msecs)").arg(duration));
        counterexampleList.clear();
        emit counterexamplesFound(); // to clear the result window
        return;
    }
    else if(response.startsWith("unknown"))
    {
        ui->textEditOutput->setText(tr("The validity is unknown (Time: %1 msecs)").arg(duration));
        counterexampleList.clear();
        emit counterexamplesFound(); // to clear the result window
        return;
    }

    counterexampleList = response.simplified().split(")", Qt::SkipEmptyParts);
    counterexampleList.replaceInStrings("(", "");
    counterexampleList.replaceInStrings("id", "");
    counterexampleList.replaceInStrings("ID_", "");
    counterexampleList.replaceInStrings(" ", "");
    emit counterexamplesFound();
}

void SmtVerifierDialog::stopProcess()
{
    smtprocess->kill();
    smtprocess->waitForFinished();
}

bool SmtVerifierDialog::isTypeSelected(QString type)
{
    for(int i = 0; i < ui->listWidgetTypes->count(); ++i)
    {
        QListWidgetItem* item = ui->listWidgetTypes->item(i);
        if(item->text() == type && item->checkState() == Qt::Checked)
        {
            return true;
        }
    }
    return false;
}

QString SmtVerifierDialog::createSmtQuery()
{
    QString querystring("(set-option :print-success false)\n");
    querystring.append("(set-option :produce-models true)\n");
    querystring.append(options);
    querystring.append(QString("(set-logic %1)\n\n").arg(logic));
    querystring.append(QString("; ---- Enum Definitions\n"));
    querystring.append("(declare-datatype ENUMWirkrichtung ((in) (gegen) (beide)))\n");
    querystring.append("(declare-datatype ENUMTOP_Anschluss ((Ende) (Links) (Rechts) (Spitze) (Verbindung) (Ende_Bestdig) (Schnitt) (sonstige)))\n");
    querystring.append("(declare-datatype ENUMSignal_Art ((Hauptsignal) (Hauptsperrsignal) (Mehrabschnittssignal) (Mehrabschnittssperrsignal) (Sperrsignal) (Vorsignal) (Vorsignalwiederholer) (Zugdeckungssignal) (andereSA)))\n");
    querystring.append("(declare-datatype ENUMSignal_Funktion ((Alleinstehendes_Zusatzsignal) (Ausfahr_Signal) (BUE_Ueberwachungssignal) (Vorsignal_Vorsignalwiederholer) (Zug_Ziel_Signal) (Ausfahr_Zwischen_Signal) (Block_Signal) (Deckungs_Signal) (Einfahr_Ausfahr_Signal) (Einfahr_Block_Signal) (Einfahr_Signal) (Gruppenausfahr_Gruppenzwischen_Signal) (Gruppenausfahr_Signal) (Gruppenzwischen_Signal) (Nachrueck_Signal) (Zugdeckungs_Signal) (Zwischen_Signal) (andereSF)))\n");
    querystring.append("(declare-datatype ENUMPZB_Art ((P1000_2000_Hz) (P1000_Hz) (P2000_Hz) (P500_Hz)))\n");


    QString inPlanDefinition = QString("(define-fun inPlan ((ppo PPObjekt)) Bool (or ");
    QSet<QString> idSet;
    QString planProData = QString();
    int objectCount = 1;

    QList<DomItem*> objectlist = document->getObjectList(PlanProDocument::PlanningStateEnd);
    for(int i = 0; i < objectlist.count(); ++i)
    {
        DomItem* item = objectlist.at(i);
        QString id = item->getFirstValueAtPath("Identitaet/Wert");
        id.prepend("ID_");
        QString type = item->getName();
        if(type == "TOP_Knoten")
        {
            if(isTypeSelected(type))
            {
                QString objectDefinition = QString("(define-fun o%1 () PPObjekt (PlanProObjekt %2 TopKnoten))\n").arg(objectCount).arg(id);
                planProData.append(objectDefinition);
                inPlanDefinition.append(QString("(= ppo o%1) ").arg(objectCount));
                idSet.insert(id);
                objectCount++;
            }
        }
        else if(type == "TOP_Kante")
        {
            if(isTypeSelected(type))
            {
                QString idA = item->getFirstValueAtPath("ID_TOP_Knoten_A/Wert");
                idA.prepend("ID_");
                QString idB = item->getFirstValueAtPath("ID_TOP_Knoten_B/Wert");
                idB.prepend("ID_");
                QString laenge = item->getFirstValueAtPath("TOP_Kante_Allg/TOP_Laenge/Wert");
                double doublen = laenge.toDouble();
                doublen *= 1000.0;
                int intlen = (int) doublen;
                QString anschlussA = item->getFirstValueAtPath("TOP_Kante_Allg/TOP_Anschluss_A/Wert");
                QString anschlussB = item->getFirstValueAtPath("TOP_Kante_Allg/TOP_Anschluss_B/Wert");
                QString objectDefinition = QString("(define-fun o%1 () PPObjekt (PlanProObjekt %2 (TopKante %3 %4 %5 %6 %7)))\n").arg(objectCount).arg(id).arg(idA).arg(idB).arg(intlen).arg(anschlussA).arg(anschlussB);
                planProData.append(objectDefinition);
                inPlanDefinition.append(QString("(= ppo o%1) ").arg(objectCount));
                idSet.insert(id);
                idSet.insert(idA);
                idSet.insert(idB);
                objectCount++;
            }
        }
        else if(type == "ZUB_Streckeneigenschaft")
        {
            if(isTypeSelected(type))
            {
                QString objectDefinition = QString("(define-fun o%1 () PPObjekt (PlanProObjekt %2 ZUBStreckeneigenschaft))\n").arg(objectCount).arg(id);
                planProData.append(objectDefinition);
                inPlanDefinition.append(QString("(= ppo o%1) ").arg(objectCount));
                idSet.insert(id);
                objectCount++;
            }
        }
        else if(type == "Proxy_Objekt")
        {
            if(isTypeSelected(type))
            {
                QString objectDefinition = QString("(define-fun o%1 () PPObjekt (PlanProObjekt %2 ProxyObjekt))\n").arg(objectCount).arg(id);
                planProData.append(objectDefinition);
                inPlanDefinition.append(QString("(= ppo o%1) ").arg(objectCount));
                idSet.insert(id);
                objectCount++;
            }
        }
        else if(item->getFirstChildItem("Punkt_Objekt_TOP_Kante") != NULL)
        {
            DomItem* poitem = item->getFirstChildItem("Punkt_Objekt_TOP_Kante");
            QString idTopKante = poitem->getFirstValueAtPath("ID_TOP_Kante/Wert");
            idTopKante.prepend("ID_");
            QString abstand = poitem->getFirstValueAtPath("Abstand/Wert");
            double doubabst = abstand.toDouble();
            doubabst *= 1000.0;
            int intabst = (int) doubabst;
            QString wirkrichtung = poitem->getFirstValueAtPath("Wirkrichtung/Wert");
            if(wirkrichtung.isEmpty())
            {
                wirkrichtung = QString("beide");
            }
            QString seitabstand = poitem->getFirstValueAtPath("Seitlicher_Abstand/Wert");
            double doubseitabst = seitabstand.toDouble();
            doubseitabst *= 1000.0;
            int intseitabst = (int) doubseitabst;
            QString seitabstString = (intseitabst < 0) ? QString("(- %1)").arg(intseitabst * -1) : QString::number(intseitabst);

            if(type == "Signal")
            {
                if(isTypeSelected(type))
                {
                    QString signalArt = item->getFirstValueAtPath("Signal_Real/Signal_Real_Aktiv_Schirm/Signal_Art/Wert");
                    if(signalArt.isEmpty() || signalArt == "andere")
                    {
                        signalArt = QString("andereSA");
                    }
                    QString signalFunktion = item->getFirstValueAtPath("Signal_Real/Signal_Real_Aktiv/Signal_Funktion/Wert");
                    if(signalFunktion.isEmpty() || signalFunktion == "andere")
                    {
                        signalFunktion = QString("andereSF");
                    }
                    QString objectDefinition = QString("(define-fun o%1 () PPObjekt (PlanProObjekt %2 (PunktObjekt %3 %4 %5 %6 (Signal %7 %8))))\n").arg(objectCount).arg(id).arg(idTopKante).arg(intabst).arg(wirkrichtung).arg(seitabstString).arg(signalArt).arg(signalFunktion);
                    planProData.append(objectDefinition);
                    inPlanDefinition.append(QString("(= ppo o%1) ").arg(objectCount));
                    idSet.insert(id);
                    idSet.insert(idTopKante);
                    objectCount++;
                }
            }
            else if(type == "Datenpunkt")
            {
                if(isTypeSelected(type))
                {
                    QString idBezug = item->getFirstValueAtPath("DP_Bezug_Betrieblich/ID_DP_Bezugspunkt/Wert");
                    idBezug.prepend("ID_");
                    QString laenge = item->getFirstValueAtPath("Datenpunkt_Allg/Datenpunkt_Laenge/Wert");
                    double doublen = laenge.toDouble();
                    doublen *= 1000.0;
                    int intlen = (int) doublen;
                    QString dptyp = item->getFirstValueAtPath("DP_Typ/DP_Typ_GETCS/DP_Typ_ETCS/Wert");
                    int intdptyp = dptyp.toInt();
                    QString objectDefinition = QString("(define-fun o%1 () PPObjekt (PlanProObjekt %2 (PunktObjekt %3 %4 %5 %6 (Datenpunkt %7 %8 %9))))\n").arg(objectCount).arg(id).arg(idTopKante).arg(intabst).arg(wirkrichtung).arg(seitabstString).arg(idBezug).arg(intlen).arg(intdptyp);
                    planProData.append(objectDefinition);
                    inPlanDefinition.append(QString("(= ppo o%1) ").arg(objectCount));
                    idSet.insert(id);
                    idSet.insert(idTopKante);
                    idSet.insert(idBezug);
                    objectCount++;
                }
            }
            else if(type == "BUE_Anlage")
            {
                if(isTypeSelected(type))
                {
                    QString objectDefinition = QString("(define-fun o%1 () PPObjekt (PlanProObjekt %2 (PunktObjekt %3 %4 %5 %6 BUEAnlage)))\n").arg(objectCount).arg(id).arg(idTopKante).arg(intabst).arg(wirkrichtung).arg(seitabstString);
                    planProData.append(objectDefinition);
                    inPlanDefinition.append(QString("(= ppo o%1) ").arg(objectCount));
                    idSet.insert(id);
                    idSet.insert(idTopKante);
                    objectCount++;
                }
            }
            else if(type == "W_Kr_Gsp_Komponente")
            {
                if(isTypeSelected(type))
                {
                    QString speedL = item->getFirstValueAtPath("Zungenpaar/Geschwindigkeit_L/Wert");
                    int intspeedL = speedL.toInt();
                    QString speedR = item->getFirstValueAtPath("Zungenpaar/Geschwindigkeit_R/Wert");
                    int intspeedR = speedR.toInt();
                    QString objectDefinition = QString("(define-fun o%1 () PPObjekt (PlanProObjekt %2 (PunktObjekt %3 %4 %5 %6 (WKrGspKomponente %7 %8))))\n").arg(objectCount).arg(id).arg(idTopKante).arg(intabst).arg(wirkrichtung).arg(seitabstString).arg(intspeedL).arg(intspeedR);
                    planProData.append(objectDefinition);
                    inPlanDefinition.append(QString("(= ppo o%1) ").arg(objectCount));
                    idSet.insert(id);
                    idSet.insert(idTopKante);
                    objectCount++;
                }
            }
            else if(type == "PZB_Element")
            {
                if(isTypeSelected(type))
                {
                    QString pzbart = item->getFirstValueAtPath("PZB_Art/Wert");
                    pzbart.prepend("P");
                    QString objectDefinition = QString("(define-fun o%1 () PPObjekt (PlanProObjekt %2 (PunktObjekt %3 %4 %5 %6 (PZBElement %7))))\n").arg(objectCount).arg(id).arg(idTopKante).arg(intabst).arg(wirkrichtung).arg(seitabstString).arg(pzbart);
                    planProData.append(objectDefinition);
                    inPlanDefinition.append(QString("(= ppo o%1) ").arg(objectCount));
                    idSet.insert(id);
                    idSet.insert(idTopKante);
                    objectCount++;
                }
            }
            else
            {
                if(isTypeSelected("Punkt_Objekt"))
                {
                    QString objectDefinition = QString("(define-fun o%1 () PPObjekt (PlanProObjekt %2 (PunktObjekt %3 %4 %5 %6 anderesPO)))\n").arg(objectCount).arg(id).arg(idTopKante).arg(intabst).arg(wirkrichtung).arg(seitabstString);
                    planProData.append(objectDefinition);
                    inPlanDefinition.append(QString("(= ppo o%1) ").arg(objectCount));
                    idSet.insert(id);
                    idSet.insert(idTopKante);
                    objectCount++;
                }
            }
        }
        else
        {
            if(isTypeSelected("Basis_Objekt"))
            {
                QString objectDefinition = QString("(define-fun o%1 () PPObjekt (PlanProObjekt %2 BasisObjekt))\n").arg(objectCount).arg(id);
                planProData.append(objectDefinition);
                inPlanDefinition.append(QString("(= ppo o%1) ").arg(objectCount));
                idSet.insert(id);
                objectCount++;
            }
        }
    }
    inPlanDefinition.append("))\n");

    QString dataTypeString = QString("(declare-datatype Guid (");
    QSetIterator<QString> i(idSet);
    while (i.hasNext())
    {
        QString id = i.next();
        dataTypeString.append(QString("(%1) ").arg(id));
    }
    dataTypeString.append(QString("))\n"));

    querystring.append(QString("; ---- Definition of the GUID Enum\n"));
    querystring.append(dataTypeString);
    querystring.append(QString("; ---- Definition of the PlanPro Model\n"));
    querystring.append("(declare-datatype PPPunktObjektExt ((Signal (signalArt ENUMSignal_Art) (signalFunktion ENUMSignal_Funktion)) (Datenpunkt (idBezug Guid) (dpLaenge Int) (dpTyp Int)) (WKrGspKomponente (geschwindigkeitL Int) (geschwindigkeitR Int)) (PZBElement (pzbArt ENUMPZB_Art)) (BUEAnlage) (anderesPO)))\n");
    querystring.append("(declare-datatype PPObjektExt ((BasisObjekt) (ProxyObjekt) (ZUBStreckeneigenschaftObjekt) (TopKnoten) (TopKante (idKnotenA Guid) (idKnotenB Guid) (laenge Int) (anschlussA ENUMTOP_Anschluss) (anschlussB ENUMTOP_Anschluss)) (PunktObjekt (idTopKante Guid) (abstand Int) (wirkrichtung ENUMWirkrichtung) (seitlAbstand Int) (poext PPPunktObjektExt))))\n");
    querystring.append("(declare-datatype PPObjekt ((PlanProObjekt (id Guid) (objext PPObjektExt))))\n");

    querystring.append(QString("; ---- Expansion of the PlanPro Object Data\n"));
    querystring.append(planProData);
    querystring.append(QString("; ---- Definition of the inPlan Function\n"));
    querystring.append(inPlanDefinition);
    querystring.append(QString("; ---- Content of the Test File\n"));
    querystring.append(fileContent);
    querystring.append(QString("; ---- check-sat Statement\n"));
    querystring.append("(check-sat)\n");
    return querystring;
}
