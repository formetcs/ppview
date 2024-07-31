#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE

class QProcess;
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButtonAddHeader_clicked();

    void on_pushButtonParsePlanPro_clicked();

    void on_pushButtonVerify_clicked();

    void on_pushButtonAddTextFile_clicked();

    void handleSmtOutput();

    void on_pushButtonEnterCommand_clicked();

private:
    void createDatatypeDeclarations();
    Ui::Widget *ui;
    QProcess* smtprocess;
    QString variables;
    int msecs;
};
#endif // WIDGET_H
