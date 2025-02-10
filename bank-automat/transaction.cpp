#include "environment.h"
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

void TransactionWindow::fetchTransactions(const QString &idcard, const QByteArray &token)
{
    QString url = Environment::base_url() + "/transaction/account/" + idcard;
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", token);

    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &TransactionWindow::onTransactionDataReceived);
    networkManager->get(request);
}

void TransactionWindow::onTransactionDataReceived(QNetworkReply *reply)
{
    QByteArray responseData = reply->readAll();
    reply->deleteLater();
    networkManager->deleteLater();

    if (reply->error() == QNetworkReply::NoError) {

        qDebug() << "Transaction Data:" << responseData;

        QJsonArray transactions = QJsonDocument::fromJson(responseData).array();
        ui->listWidget->clear();


        for (const QJsonValue &value : transactions) {
            QJsonObject transaction = value.toObject();
            QString idStr = QString::number(transaction["idtransaction"].toVariant().toInt());
            QString type = transaction["type"].toString();
            QString description = transaction["description"].toString();
            QString amount = transaction["amount"].toString();

            QString timestamp = transaction["timestamp"].toString();
            QDateTime dateTime = QDateTime::fromString(timestamp, Qt::ISODate);
            if (dateTime.isValid()) {
                timestamp = dateTime.toString("yyyy-MM-dd hh:mm:ss");
            } else {
                timestamp = "Invalid Date";
            }

            QString itemText = QString("ID: %1 | Type: %2 | Description: %3 | Amount: %4 | Timestamp: %5")
                                   .arg(idStr)
                                   .arg(type)
                                   .arg(description)
                                   .arg(amount)
                                   .arg(timestamp);

            ui->listWidget->addItem(itemText);
        }
    } else {
        qDebug() << "Error fetching transactions:" << reply->errorString();
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
