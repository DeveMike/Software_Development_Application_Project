#include "withdraw.h"
#include "ui_withdraw.h"

WithdrawWindow::WithdrawWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WithdrawWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("ATM");
}

WithdrawWindow::~WithdrawWindow()
{
    delete ui;
}

void WithdrawWindow::setLanguage(const QString &newLanguage)
{
    selectedLanguage = newLanguage;
    updateLanguage();
}

void WithdrawWindow::updateLanguage()
{
    if (selectedLanguage == "FI") {
        ui->btnBack->setText("Takaisin");
    }
    else if (selectedLanguage == "SWE") {
        ui->btnBack->setText("Tillbaka");
    }
    else if (selectedLanguage == "ENG") {
        ui->btnBack->setText("Back");
    }
}

void WithdrawWindow::on_btnBack_clicked()
{
    this->hide();
    QWidget *mainMenu = parentWidget();

    if (mainMenu) {
        mainMenu->show();
    }
}

void WithdrawWindow::closeEvent(QCloseEvent *)
{
    QApplication::quit(); // Sammuttaa koko sovelluksen
}
