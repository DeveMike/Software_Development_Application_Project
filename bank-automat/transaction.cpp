#include "transaction.h"
#include "ui_transaction.h"

TransactionWindow::TransactionWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TransactionWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("ATM");
}

TransactionWindow::~TransactionWindow()
{
    delete ui;
}

void TransactionWindow::setLanguage(const QString &newLanguage)
{
    selectedLanguage = newLanguage;
    updateLanguage();
}

void TransactionWindow::updateLanguage()
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

void TransactionWindow::on_btnBack_clicked()
{
    this->hide();
    QWidget *mainMenu = parentWidget();

    if (mainMenu) {
        mainMenu->show();
    }
}

void TransactionWindow::closeEvent(QCloseEvent *)
{
    QApplication::quit(); // Sammuttaa koko sovelluksen
}
