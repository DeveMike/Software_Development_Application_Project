#include "transfer.h"
#include "ui_transfer.h"
#include "environment.h"

#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <QCloseEvent>
#include <QApplication>
#include <QInputDialog>

TransferWindow::TransferWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TransferWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("ATM");
    networkManager = new QNetworkAccessManager(this);
}

TransferWindow::~TransferWindow()
{
    delete ui;
}

void TransferWindow::setIdcard(const QString &idcard)
{
    mIdcard = idcard;
}

void TransferWindow::setCardMode(const QString &mode)
{
    mCardMode = mode;
}

void TransferWindow::setMyToken(const QByteArray &token)
{
    mToken = token;
}

void TransferWindow::setLanguage(const QString &newLanguage)
{
    selectedLanguage = newLanguage;
    updateLanguage();
}

void TransferWindow::updateLanguage()
{
    if (selectedLanguage == "SWE") {
        ui->txtBack->setText("Tillbaka");
        ui->buttonTransfer->setText("Överför");
    }
    else if (selectedLanguage == "ENG") {
        ui->txtBack->setText("Back");
        ui->buttonTransfer->setText("Transfer");
    }
    else {
        ui->txtBack->setText("Takaisin");
        ui->buttonTransfer->setText("Siirrä");
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
    QApplication::quit();
}

void TransferWindow::on_buttonTransfer_clicked()
{
    bool ok;
    double transferAmount = QInputDialog::getDouble(
        this,
        selectedLanguage == "SWE" ? "Ange överföringsbelopp" :
            selectedLanguage == "ENG" ? "Enter transfer amount" : "Anna siirrettävä summa",
        selectedLanguage == "SWE" ? "Ange belopp (€)" :
            selectedLanguage == "ENG" ? "Enter amount (€)" : "Syötä summa (€)",
        0, 0, 10000, 2, &ok);

    if (!ok) return;

    int receiverAccountId = QInputDialog::getInt(
        this,
        selectedLanguage == "SWE" ? "Ange mottagarkonto" :
            selectedLanguage == "ENG" ? "Enter recipient account ID" : "Anna vastaanottajan tilinumero",
        selectedLanguage == "SWE" ? "Ange kontonummer" :
            selectedLanguage == "ENG" ? "Enter account number" : "Syötä tilinumero",
        0, 0, 999999, 1, &ok);

    if (!ok) return;

    fetchAccountDetails(transferAmount, receiverAccountId);
}

void TransferWindow::fetchAccountDetails(double amount, int receiverAccountId)
{
    QString url = Environment::base_url() + "/card_account?idcard=" + mIdcard;
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", mToken);

    QNetworkReply *reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() {
        onFetchAccountDetailsFinished(reply, amount, receiverAccountId);
    });
}

void TransferWindow::onFetchAccountDetailsFinished(QNetworkReply *reply, double amount, int receiverAccountId)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

        if (!jsonDoc.isArray()) {
            QMessageBox::critical(this, "Error", "Unexpected JSON format from /card_account!");
            reply->deleteLater();
            return;
        }

        QJsonArray arr = jsonDoc.array();
        if (arr.isEmpty()) {
            QMessageBox::critical(this, "Error", "No account found for this card ID!");
            reply->deleteLater();
            return;
        }

        int senderAccountId = -1;
        for (auto val : arr) {
            QJsonObject obj = val.toObject();
            if (obj["type"].toString() == mCardMode) {
                senderAccountId = obj["idaccount"].toInt();
                break;
            }
        }

        if (senderAccountId == -1) {
            QMessageBox::warning(this, "Error", "No matching account type found (" + mCardMode + ")!");
        } else {
            processTransfer(senderAccountId, mCardMode, amount, receiverAccountId);
        }
    } else {
        QMessageBox::critical(this, "Error", "Failed to retrieve card_account: " + reply->errorString());
    }
    reply->deleteLater();
}

void TransferWindow::processTransfer(int senderAccountId, const QString &accountType, double amount, int receiverAccountId)
{
    QJsonObject transferData;
    transferData["sender_id"] = senderAccountId;
    transferData["receiver_id"] = receiverAccountId;
    transferData["amount"] = amount;

    QJsonDocument jsonDoc(transferData);
    QByteArray jsonData = jsonDoc.toJson();

    QString url = Environment::base_url() + "/transfer";
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", mToken);

    QNetworkReply *reply = networkManager->post(request, jsonData);
    connect(reply, &QNetworkReply::finished, this, [=]() {
        onTransferResponse(reply, amount, receiverAccountId);
    });
}

void TransferWindow::onTransferResponse(QNetworkReply *reply, double amount, int receiverAccountId)
{
    if (reply->error() == QNetworkReply::NoError) {
        QMessageBox::information(this, "Success", QString("Transfer of %1 € to account %2 successful.").arg(amount).arg(receiverAccountId));
    } else {
        QMessageBox::critical(this, "Error", "Transfer failed: " + reply->errorString());
    }

    reply->deleteLater();
}

void TransferWindow::on_pushButton_clicked()
{
    bool ok;
    double transferAmount = QInputDialog::getDouble(this, "Anna siirrettävä summa",
                                                    "Syötä summa (€)", 0, 0, 10000, 2, &ok);
    if (!ok) return;

    int receiverAccountId = QInputDialog::getInt(this, "Anna vastaanottajan tilinumero",
                                                 "Syötä tilinumero", 0, 0, 999999, 1, &ok);
    if (!ok) return;

    fetchAccountDetails(transferAmount, receiverAccountId);
}

