#include "customerdata.h"
#include "mainmenu.h"
#include "environment.h"
#include "ui_mainmenu.h"
#include "withdraw.h"
#include "transfer.h"
#include "balance.h"
#include "transaction.h"
#include "login.h"

MainMenu::MainMenu(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MainMenu)
{
    ui->setupUi(this);
    this->setWindowTitle("ATM");
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::setMyToken(const QByteArray &newMyToken)
{
    myToken = newMyToken;
    qDebug()<<"Main Menu";
    qDebug()<<myToken;
}

void MainMenu::setCardMode(const QString &mode)
{
    mCardMode = mode;  // Debit/Credit
}

void MainMenu::setUsername(const QString &newUsername)
{
    idcard = newUsername;

    QNetworkRequest request(Environment::base_url() + "/card/" + idcard);
    request.setRawHeader("Authorization", myToken);

    dataManager = new QNetworkAccessManager(this);
    connect(dataManager, &QNetworkAccessManager::finished, this, &MainMenu::onCardDataReceived);
    dataManager->get(request);
}

void MainMenu::onCardDataReceived(QNetworkReply *reply)
{
    QByteArray response_data = reply->readAll();
    reply->deleteLater();
    dataManager->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        ui->labelUsername->setText("Error");
        return;
    }

    QJsonObject cardObj = QJsonDocument::fromJson(response_data).object();
    int customerId = cardObj["idcustomer"].toInt();
    if (customerId == 0) {
        ui->labelUsername->setText("Unknown");
        return;
    }

    QNetworkRequest custReq(Environment::base_url() + "/customer/" + QString::number(customerId));
    custReq.setRawHeader("Authorization", myToken);

    QNetworkAccessManager *customerDataManager = new QNetworkAccessManager(this);
    connect(customerDataManager, &QNetworkAccessManager::finished, this,
            [=](QNetworkReply *custReply) {
                QByteArray custData = custReply->readAll();
                custReply->deleteLater();
                customerDataManager->deleteLater();

                if (custReply->error() == QNetworkReply::NoError) {
                    QJsonObject custObj = QJsonDocument::fromJson(custData).object();
                    ui->labelUsername->setText(custObj["fname"].toString()
                                               + " "
                                               + custObj["lname"].toString());
                } else {
                    ui->labelUsername->setText("Error");
                }
            });

    customerDataManager->get(custReq);
}

void MainMenu::setLanguage(const QString &newLanguage)
{
    selectedLanguage = newLanguage;
    qDebug() << "Language set in MainMenu: " << selectedLanguage;

    if (selectedLanguage == "FI") {
        ui->labelWelcome->setText("Tervetuloa");
        ui->btnWithdraw->setText("Nosta");
        ui->btnBalance->setText("Saldo");
        ui->btnTransfer->setText("Siirrä");
        ui->btnTransaction->setText("Tapahtumat");
        ui->btnData->setText("Omat tiedot");
        ui->btnLogout->setText("Kirjaudu ulos");
    }
    else if (selectedLanguage == "SWE") {
        ui->labelWelcome->setText("Välkommen");
        ui->btnWithdraw->setText("Uttag");
        ui->btnBalance->setText("Saldo");
        ui->btnTransfer->setText("Överföring");
        ui->btnTransaction->setText("Transaktioner");
        ui->btnData->setText("Mina uppgifter");
        ui->btnLogout->setText("Logga ut");
    }
    else if (selectedLanguage == "ENG") {
        ui->labelWelcome->setText("Welcome");
        ui->btnWithdraw->setText("Withdraw");
        ui->btnBalance->setText("Balance");
        ui->btnTransfer->setText("Transfer");
        ui->btnTransaction->setText("Transactions");
        ui->btnData->setText("My Data");
        ui->btnLogout->setText("Log out");
    }
}

QString MainMenu::getLanguage() const
{
    return selectedLanguage;
}

void MainMenu::on_btnData_clicked()
{
    this->hide();
    CustomerData *objCustomerData = new CustomerData(this);
    objCustomerData->setGeometry(this->geometry());
    objCustomerData->setIdcard(idcard);
    objCustomerData->setMyToken(myToken);
    objCustomerData->setLanguage(selectedLanguage);
    objCustomerData->show();
    connect(objCustomerData, &CustomerData::finished, this, &MainMenu::show);
}

void MainMenu::on_btnWithdraw_clicked()
{
    this->hide();
    WithdrawWindow *objWithdrawWindow = new WithdrawWindow(this);
    objWithdrawWindow->setGeometry(this->geometry());
    objWithdrawWindow->setMyToken(myToken);
    objWithdrawWindow->setIdcard(idcard);
    objWithdrawWindow->setCardMode(mCardMode);
    objWithdrawWindow->setLanguage(selectedLanguage);
    objWithdrawWindow->show();
    connect(objWithdrawWindow, &WithdrawWindow::finished, this, &MainMenu::show);
}

void MainMenu::on_btnTransfer_clicked()
{
    this->hide();
    TransferWindow *objTransferWindow = new TransferWindow(this);
    objTransferWindow->setGeometry(this->geometry());
    objTransferWindow->setLanguage(selectedLanguage);
    objTransferWindow->show();
    connect(objTransferWindow, &TransferWindow::finished, this, &MainMenu::show);
}

void MainMenu::on_btnBalance_clicked()
{
    this->hide();
    BalanceWindow *objBalanceWindow = new BalanceWindow(this);
    objBalanceWindow->setGeometry(this->geometry());
    objBalanceWindow->setAuthToken(myToken);
    objBalanceWindow->setIdCard(idcard);
    objBalanceWindow->setLanguage(selectedLanguage);
    objBalanceWindow->show();
    connect(objBalanceWindow, &BalanceWindow::finished, this, &MainMenu::show);
}

void MainMenu::on_btnTransaction_clicked()
{
    this->hide();
    TransactionWindow *objTransactionWindow = new TransactionWindow(this);
    objTransactionWindow->setGeometry(this->geometry());
    objTransactionWindow->setLanguage(selectedLanguage);
    objTransactionWindow->fetchTransactions(idcard, myToken);
    objTransactionWindow->show();
    connect(objTransactionWindow, &TransactionWindow::finished, this, &MainMenu::show);
}

void MainMenu::on_btnLogout_clicked()
{
    this->hide();
    MainWindow *objMainWindow = new MainWindow(this);
    objMainWindow->setGeometry(this->geometry());
    objMainWindow->show();
}

void MainMenu::closeEvent(QCloseEvent *)
{
    QApplication::quit();
}

