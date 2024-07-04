#include "widget.h"
#include "ui_widget.h"
#include "../../src/planproxmldocument.h"

#include <QInputDialog>
#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    smtprocess = new QProcess();
    connect(smtprocess, SIGNAL(readyRead()), this, SLOT(handleSmtOutput()));
}

Widget::~Widget()
{
    smtprocess->kill();
    smtprocess->waitForFinished();
    delete ui;
    delete smtprocess;
}

void Widget::on_pushButtonAddHeader_clicked()
{
    QString logic = QInputDialog::getText(this, "Set Logic", "Enter the logic that should be used", QLineEdit::Normal, "UFLIA");
    if (!logic.isEmpty())
    {
        ui->textEditInput->append("(set-option :print-success false)");
        ui->textEditInput->append("(set-option :produce-models true)");
        ui->textEditInput->append(QString("(set-logic %1)").arg(logic));
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

    QList<DomItem*> objectlist = document->getObjectList(PlanProDocument::PlanningStateEnd);
    for(int i = 0; i < objectlist.count(); ++i)
    {
        DomItem* item = objectlist.at(i);
        QString id = item->getFirstValueAtPath("Identitaet/Wert");
        QString type = item->getName();
        ui->textEditInput->append(QString("%1 (%2)").arg(type).arg(id));
    }


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
    QString cmdline = QInputDialog::getText(this, "Enter Command", "Enter the command of the SMT process (incl. arguments)", QLineEdit::Normal, "/Users/stefan/tools/z3/bin/z3 -in");
    if(cmdline.isEmpty())
        return;

    QStringList cmdlist = cmdline.split(" ", Qt::SkipEmptyParts);
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
}

void Widget::handleSmtOutput()
{
    QString response = smtprocess->readAll();
    if(response.startsWith("sat"))
    {
        smtprocess->write(QString("(get-model)\n").toLatin1());
        return;
    }
    ui->textEditOutput->append(response);
}



