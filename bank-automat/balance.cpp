#include "balance.h"
#include "ui_balance.h"
#include "environment.h"

#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

BalanceWindow::BalanceWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BalanceWindow),
    networkManager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);
    this->setWindowTitle("ATM");
}

BalanceWindow::~BalanceWindow()
{
    delete ui;
}

void BalanceWindow::setAuthToken(const QByteArray &token)
{
    authToken = token;
}

void BalanceWindow::setIdCard(const QString &id)
{
    idCard = id;
    fetchAccountDetails();
}

void BalanceWindow::fetchAccountDetails()
{
    QString url = Environment::base_url() + "/card_account?idcard=" + idCard;
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", authToken);

    QNetworkReply *reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() {
        handleAccountDetails(reply);
    });
}

void BalanceWindow::handleAccountDetails(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

        if (!jsonDoc.isArray()) {
            qDebug() << "Virheellinen JSON-muoto /card_account-reitiltä!";
            reply->deleteLater();
            return;
        }

        QJsonArray arr = jsonDoc.array();
        if (arr.isEmpty()) {
            qDebug() << "Kortille ei löytynyt tilejä!";
            reply->deleteLater();
            return;
        }

        for (const QJsonValue &val : arr) {
            QJsonObject obj = val.toObject();
            int accountId = obj["idaccount"].toInt();
            QString accountType = obj["type"].toString(); // esim. "debit" tai "credit"
            fetchAccountBalance(accountId, accountType);
        }
    } else {
        qDebug() << "Virhe tilitietojen haussa:" << reply->errorString();
    }
    reply->deleteLater();
}

void BalanceWindow::fetchAccountBalance(int accountId, const QString &accountType)
{
    QString url = Environment::base_url() + "/account/" + QString::number(accountId);
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", authToken);

    QNetworkReply *reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() {
        handleBalanceResponse(reply, accountType);
    });
}

void BalanceWindow::handleBalanceResponse(QNetworkReply *reply, const QString &accountType)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        if (!jsonDoc.isObject()) {
            qDebug() << "Virheellinen JSON /account-reitiltä!";
            reply->deleteLater();
            return;
        }
        QJsonObject obj = jsonDoc.object();

        if (accountType == "debit" && obj.contains("debit_balance")) {
            double debitBalance = 0.0;
            QJsonValue val = obj["debit_balance"];
            if (val.isDouble()) {
                debitBalance = val.toDouble();
            } else if (val.isString()) {
                debitBalance = val.toString().toDouble();
            }
            ui->balanceDebit->display(debitBalance);
        } else if (accountType == "credit" && obj.contains("credit_balance")) {
            double creditBalance = 0.0;
            QJsonValue val = obj["credit_balance"];
            if (val.isDouble()) {
                creditBalance = val.toDouble();
            } else if (val.isString()) {
                creditBalance = val.toString().toDouble();
            }
            ui->balanceCredit->display(creditBalance);
        } else {
            qDebug() << "Odotettuja kenttiä ei löytynyt JSON-vastauksesta!";
        }
    } else {
        qDebug() << "Virhe tilin saldon haussa:" << reply->errorString();
    }
    reply->deleteLater();
}

void BalanceWindow::on_btnBack_clicked()
{
    this->hide();
    QWidget *mainMenu = parentWidget();
    if (mainMenu) {
        mainMenu->show();
    }
}

void BalanceWindow::closeEvent(QCloseEvent *)
{
    QApplication::quit();
}

void BalanceWindow::setLanguage(const QString &newLanguage)
{
    selectedLanguage = newLanguage;
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
