#include "transfer.h"
#include "ui_transfer.h"

TransferWindow::TransferWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TransferWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("ATM");
}

TransferWindow::~TransferWindow()
{
    delete ui;
}

void TransferWindow::setLanguage(const QString &newLanguage)
{
    selectedLanguage = newLanguage;
    updateLanguage();
}

void TransferWindow::updateLanguage()
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

void TransferWindow::on_btnBack_clicked()
{
    this->hide();
    QWidget *mainMenu = parentWidget();

    if (mainMenu) {
        mainMenu->show();
    }
}

void TransferWindow::closeEvent(QCloseEvent *)
{
    QApplication::quit(); // Sammuttaa koko sovelluksen
}
