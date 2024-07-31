#include "widget.h"
#include "ui_widget.h"
#include "../../src/planproxmldocument.h"

#include <QInputDialog>
#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>
#include <QTime>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    smtprocess = new QProcess();
    msecs = 0;
    connect(smtprocess, SIGNAL(readyRead()), this, SLOT(handleSmtOutput()));
}

Widget::~Widget()
{
    smtprocess->kill();
    smtprocess->waitForFinished();
    delete ui;
    delete smtprocess;
}

void Widget::createDatatypeDeclarations()
{
    ui->textEditInput->append("(declare-datatype ENUMWirkrichtung ((in) (gegen) (beide)))");
    ui->textEditInput->append("(declare-datatype ENUMTOP_Anschluss ((Ende) (Links) (Rechts) (Spitze) (Verbindung) (Ende_Bestdig) (Schnitt) (sonstige)))");
    ui->textEditInput->append("(declare-datatype ENUMSignal_Art ((Hauptsignal) (Hauptsperrsignal) (Mehrabschnittssignal) (Mehrabschnittssperrsignal) (Sperrsignal) (Vorsignal) (Vorsignalwiederholer) (Zugdeckungssignal) (andereSA)))");
    ui->textEditInput->append("(declare-datatype ENUMSignal_Funktion ((Alleinstehendes_Zusatzsignal) (Ausfahr_Signal) (BUE_Ueberwachungssignal) (Vorsignal_Vorsignalwiederholer) (Zug_Ziel_Signal) (Ausfahr_Zwischen_Signal) (Block_Signal) (Deckungs_Signal) (Einfahr_Ausfahr_Signal) (Einfahr_Block_Signal) (Einfahr_Signal) (Gruppenausfahr_Gruppenzwischen_Signal) (Gruppenausfahr_Signal) (Gruppenzwischen_Signal) (Nachrueck_Signal) (Zugdeckungs_Signal) (Zwischen_Signal) (andereSF)))");
    ui->textEditInput->append("(declare-datatype ENUMPZB_Art ((P1000_2000_Hz) (P1000_Hz) (P2000_Hz) (P500_Hz)))");
    ui->textEditInput->append("(define-sort Guid () (_ BitVec 128))");
    ui->textEditInput->append("(declare-datatype PPPunktObjektExt ((Signal (signalArt ENUMSignal_Art) (signalFunktion ENUMSignal_Funktion)) (Datenpunkt (idBezug Guid) (dpLaenge Int) (dpTyp Int)) (WKrGspKomponente (geschwindigkeitL Int) (geschwindigkeitR Int)) (PZBElement (pzbArt ENUMPZB_Art)) (BUEAnlage) (anderesPO)))");
    ui->textEditInput->append("(declare-datatype PPObjektExt ((BasisObjekt) (ProxyObjekt) (ZUBStreckeneigenschaftObjekt) (TopKnoten) (TopKante (idKnotenA Guid) (idKnotenB Guid) (laenge Int) (anschlussA ENUMTOP_Anschluss) (anschlussB ENUMTOP_Anschluss)) (PunktObjekt (idTopKante Guid) (abstand Int) (wirkrichtung ENUMWirkrichtung) (seitlAbstand Int) (poext PPPunktObjektExt))))");
    ui->textEditInput->append("(declare-datatype PPObjekt ((PlanProObjekt (id Guid) (objext PPObjektExt))))");
}

void Widget::on_pushButtonAddHeader_clicked()
{
    QString logic = QInputDialog::getText(this, "Set Logic", "Enter the logic that should be used", QLineEdit::Normal, "ALL");
    if (!logic.isEmpty())
    {
        ui->textEditInput->clear();
        ui->textEditInput->append("(set-option :print-success false)");
        ui->textEditInput->append("(set-option :produce-models true)");
        ui->textEditInput->append(QString("(set-logic %1)").arg(logic));

        createDatatypeDeclarations();
    }
}


void Widget::on_pushButtonParsePlanPro_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    QDir::homePath(),
                                                    tr("PlanPro XML files (*.ppxml);;All files (*.*)"));

    if(fileName.isEmpty())
        return;

    PlanProXmlDocument* document = new PlanProXmlDocument();
    bool success = document->loadFile(fileName);
    if(!success)
    {
        QMessageBox::critical(this, "File Reading Error",
                              QString("File\n%1\ncould not be opened").arg(fileName));
        delete document;
        return;
    }

    QString inPlanDefinition = QString("(define-fun inPlan ((ppo PPObjekt)) Bool (or ");

    QList<DomItem*> objectlist = document->getObjectList(PlanProDocument::PlanningStateEnd);
    for(int i = 0; i < objectlist.count(); ++i)
    {
        DomItem* item = objectlist.at(i);
        QString id = item->getFirstValueAtPath("Identitaet/Wert");
        id.remove("-");
        id.prepend("#x");
        QString type = item->getName();
        QString objectDefinition;
        if(type == "TOP_Knoten")
        {
            if(!ui->cbTopKnoten->isChecked())
            {
                continue;
            }
            objectDefinition = QString("(define-fun o%1 () PPObjekt (PlanProObjekt %2 TopKnoten))").arg(i).arg(id);
            inPlanDefinition.append(QString("(= ppo o%1) ").arg(i));
        }
        else if(type == "TOP_Kante")
        {
            if(!ui->cbTopKante->isChecked())
            {
                continue;
            }
            QString idA = item->getFirstValueAtPath("ID_TOP_Knoten_A/Wert");
            idA.remove("-");
            idA.prepend("#x");
            QString idB = item->getFirstValueAtPath("ID_TOP_Knoten_B/Wert");
            idB.remove("-");
            idB.prepend("#x");
            QString laenge = item->getFirstValueAtPath("TOP_Kante_Allg/TOP_Laenge/Wert");
            double doublen = laenge.toDouble();
            doublen *= 1000.0;
            int intlen = (int) doublen;
            QString anschlussA = item->getFirstValueAtPath("TOP_Kante_Allg/TOP_Anschluss_A/Wert");
            QString anschlussB = item->getFirstValueAtPath("TOP_Kante_Allg/TOP_Anschluss_B/Wert");
            objectDefinition = QString("(define-fun o%1 () PPObjekt (PlanProObjekt %2 (TopKante %3 %4 %5 %6 %7)))").arg(i).arg(id).arg(idA).arg(idB).arg(intlen).arg(anschlussA).arg(anschlussB);
            inPlanDefinition.append(QString("(= ppo o%1) ").arg(i));
        }
        else if(type == "ZUB_Streckeneigenschaft")
        {
            if(!ui->cbZubStreckeneigenschaft->isChecked())
            {
                continue;
            }
            objectDefinition = QString("(define-fun o%1 () PPObjekt (PlanProObjekt %2 ZUBStreckeneigenschaft))").arg(i).arg(id);
            inPlanDefinition.append(QString("(= ppo o%1) ").arg(i));
        }
        else if(type == "Proxy_Objekt")
        {
            if(!ui->cbProxyObjekt->isChecked())
            {
                continue;
            }
            objectDefinition = QString("(define-fun o%1 () PPObjekt (PlanProObjekt %2 ProxyObjekt))").arg(i).arg(id);
            inPlanDefinition.append(QString("(= ppo o%1) ").arg(i));
        }
        else if(item->getFirstChildItem("Punkt_Objekt_TOP_Kante") != NULL)
        {
            DomItem* poitem = item->getFirstChildItem("Punkt_Objekt_TOP_Kante");
            QString idTopKante = poitem->getFirstValueAtPath("ID_TOP_Kante/Wert");
            idTopKante.remove("-");
            idTopKante.prepend("#x");
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
                if(!ui->cbSignal->isChecked())
                {
                    continue;
                }
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
                objectDefinition = QString("(define-fun o%1 () PPObjekt (PlanProObjekt %2 (PunktObjekt %3 %4 %5 %6 (Signal %7 %8))))").arg(i).arg(id).arg(idTopKante).arg(intabst).arg(wirkrichtung).arg(seitabstString).arg(signalArt).arg(signalFunktion);
                inPlanDefinition.append(QString("(= ppo o%1) ").arg(i));
            }
            else if(type == "Datenpunkt")
            {
                if(!ui->cbDatenpunkt->isChecked())
                {
                    continue;
                }
                QString idBezug = item->getFirstValueAtPath("DP_Bezug_Betrieblich/ID_DP_Bezugspunkt/Wert");
                idBezug.remove("-");
                idBezug.prepend("#x");
                QString laenge = item->getFirstValueAtPath("Datenpunkt_Allg/Datenpunkt_Laenge/Wert");
                double doublen = laenge.toDouble();
                doublen *= 1000.0;
                int intlen = (int) doublen;
                QString dptyp = item->getFirstValueAtPath("DP_Typ/DP_Typ_GETCS/DP_Typ_ETCS/Wert");
                int intdptyp = dptyp.toInt();
                objectDefinition = QString("(define-fun o%1 () PPObjekt (PlanProObjekt %2 (PunktObjekt %3 %4 %5 %6 (Datenpunkt %7 %8 %9))))").arg(i).arg(id).arg(idTopKante).arg(intabst).arg(wirkrichtung).arg(seitabstString).arg(idBezug).arg(intlen).arg(intdptyp);
                inPlanDefinition.append(QString("(= ppo o%1) ").arg(i));
            }
            else if(type == "BUE_Anlage")
            {
                if(!ui->cbBueAnlage->isChecked())
                {
                    continue;
                }
                objectDefinition = QString("(define-fun o%1 () PPObjekt (PlanProObjekt %2 (PunktObjekt %3 %4 %5 %6 BUEAnlage)))").arg(i).arg(id).arg(idTopKante).arg(intabst).arg(wirkrichtung).arg(seitabstString);
                inPlanDefinition.append(QString("(= ppo o%1) ").arg(i));
            }
            else if(type == "W_Kr_Gsp_Komponente")
            {
                if(!ui->cbWKrGspKomponente->isChecked())
                {
                    continue;
                }
                QString speedL = item->getFirstValueAtPath("Zungenpaar/Geschwindigkeit_L/Wert");
                int intspeedL = speedL.toInt();
                QString speedR = item->getFirstValueAtPath("Zungenpaar/Geschwindigkeit_R/Wert");
                int intspeedR = speedR.toInt();
                objectDefinition = QString("(define-fun o%1 () PPObjekt (PlanProObjekt %2 (PunktObjekt %3 %4 %5 %6 (WKrGspKomponente %7 %8))))").arg(i).arg(id).arg(idTopKante).arg(intabst).arg(wirkrichtung).arg(seitabstString).arg(intspeedL).arg(intspeedR);
                inPlanDefinition.append(QString("(= ppo o%1) ").arg(i));
            }
            else if(type == "PZB_Element")
            {
                if(!ui->cbPzbElement->isChecked())
                {
                    continue;
                }
                QString pzbart = item->getFirstValueAtPath("PZB_Art/Wert");
                pzbart.prepend("P");
                objectDefinition = QString("(define-fun o%1 () PPObjekt (PlanProObjekt %2 (PunktObjekt %3 %4 %5 %6 (PZBElement %7))))").arg(i).arg(id).arg(idTopKante).arg(intabst).arg(wirkrichtung).arg(seitabstString).arg(pzbart);
                inPlanDefinition.append(QString("(= ppo o%1) ").arg(i));
            }
            else
            {
                if(!ui->cbOtherPunktObjekt->isChecked())
                {
                    continue;
                }
                objectDefinition = QString("(define-fun o%1 () PPObjekt (PlanProObjekt %2 (PunktObjekt %3 %4 %5 %6 anderesPO)))").arg(i).arg(id).arg(idTopKante).arg(intabst).arg(wirkrichtung).arg(seitabstString);
                inPlanDefinition.append(QString("(= ppo o%1) ").arg(i));
            }

        }
        else
        {
            if(!ui->cbOtherBasisObjekt->isChecked())
            {
                continue;
            }
            objectDefinition = QString("(define-fun o%1 () PPObjekt (PlanProObjekt %2 BasisObjekt))").arg(i).arg(id);
            inPlanDefinition.append(QString("(= ppo o%1) ").arg(i));
        }

        if(!objectDefinition.isEmpty())
        {
            ui->textEditInput->append(objectDefinition);
        }

    }

    inPlanDefinition.append("))");
    ui->textEditInput->append(inPlanDefinition);


    delete document;
}

void Widget::on_pushButtonAddTextFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    QDir::homePath(),
                                                    tr("SMTLIB files (*.smt2);;Text files (*.txt);;All files (*.*)"));

    if(fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        ui->textEditInput->append(line);
    }
}


void Widget::on_pushButtonVerify_clicked()
{
    bool ok;
    variables = QInputDialog::getText(this, "Enter Variables", "Enter the variables which assignment should be displayed (leave empty for complete model)", QLineEdit::Normal, QString(), &ok);
    if(!ok)
    {
        return;
    }

    QStringList cmdlist = ui->comboBoxCmdline->currentText().split(" ", Qt::SkipEmptyParts);
    QString command = cmdlist.first();
    cmdlist.removeFirst();

    smtprocess->kill();
    smtprocess->waitForFinished();
    ui->textEditOutput->clear();

    smtprocess->start(command, cmdlist);
    if(!smtprocess->waitForStarted())
    {
        ui->textEditOutput->append("SMT process did not start");
        return;
    }
    QString query = ui->textEditInput->toPlainText() + QString("\n(check-sat)\n");
    smtprocess->write(query.toLatin1());
    msecs = QTime::currentTime().msecsSinceStartOfDay();
}

void Widget::handleSmtOutput()
{
    QString response = smtprocess->readAll();
    int duration = QTime::currentTime().msecsSinceStartOfDay() - msecs;
    if(response.startsWith("sat"))
    {
        ui->textEditOutput->append(QString("sat (Duration: %1 msecs)").arg(duration));
        if(variables.isEmpty())
        {
            smtprocess->write(QString("(get-model)\n").toLatin1());
        }
        else
        {
            QStringList variablelist = variables.split(" ", Qt::SkipEmptyParts);
            QString querystring = QString("(get-value (");
            for(int i = 0; i < variablelist.count(); ++i)
            {
                QString variablestring = QString("(id %1) ").arg(variablelist.at(i));
                querystring.append(variablestring);
            }
            querystring.append("))\n");
            ui->textEditOutput->append(querystring);
            smtprocess->write(querystring.toLatin1());
        }
        return;
    }
    else if(response.startsWith("unsat"))
    {
        ui->textEditOutput->append(QString("unsat (Duration: %1 msecs)").arg(duration));
        return;
    }
    else if(response.startsWith("unknown"))
    {
        ui->textEditOutput->append(QString("unknown (Duration: %1 msecs)").arg(duration));
        return;
    }
    ui->textEditOutput->append(response);
}




void Widget::on_pushButtonEnterCommand_clicked()
{
    if(smtprocess->state() != QProcess::Running)
    {
        QMessageBox::warning(this, "Error", "SMT process is not running");
        return;
    }
    bool ok;
    QString command = QInputDialog::getText(this, "Enter Command", "Enter a command taht should be sent to the SMT process", QLineEdit::Normal, QString(), &ok);
    if(!ok)
    {
        return;
    }
    ui->textEditOutput->append(command);
    smtprocess->write(command.toLatin1());
}

