#include "withdraw.h"
#include "ui_withdraw.h"
#include "environment.h"

#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <QCloseEvent>
#include <QApplication>
#include <QInputDialog>

WithdrawWindow::WithdrawWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WithdrawWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("ATM");
    networkManager = new QNetworkAccessManager(this);
}

WithdrawWindow::~WithdrawWindow()
{
    delete ui;
}

void WithdrawWindow::setIdcard(const QString &idcard)
{
    mIdcard = idcard;
}

void WithdrawWindow::setCardMode(const QString &mode)
{
    mCardMode = mode;
}

void WithdrawWindow::setMyToken(const QByteArray &token)
{
    mToken = token;
}

void WithdrawWindow::setLanguage(const QString &newLanguage)
{
    selectedLanguage = newLanguage;
    updateLanguage();
}

void WithdrawWindow::updateLanguage()
{
    if (selectedLanguage == "SWE") {
        ui->btnBack->setText("Tillbaka");
        ui->txtOut_x->setText("Annan summa");
    }
    else if (selectedLanguage == "ENG") {
        ui->btnBack->setText("Back");
        ui->txtOut_x->setText("Other amount");
    }
    else {
        ui->btnBack->setText("Takaisin");
        ui->txtOut_x->setText("Muu summa");
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
    QApplication::quit();
}

void WithdrawWindow::on_buttonOut_20_clicked()
{
    fetchAccountDetails(20);
}

void WithdrawWindow::on_buttonOut_40_clicked()
{
    fetchAccountDetails(40);
}

void WithdrawWindow::on_buttonOut_50_clicked()
{
    fetchAccountDetails(50);
}

void WithdrawWindow::on_buttonOut_80_clicked()
{
    fetchAccountDetails(80);
}

void WithdrawWindow::on_buttonOut_100_clicked()
{
    fetchAccountDetails(100);
}

void WithdrawWindow::on_buttonOut_x_clicked()
{
    bool ok = false;
    QString dialogTitle;
    QString dialogLabel;

    if (selectedLanguage == "FI") {
        dialogTitle = "Anna nostosumma";
        dialogLabel = "Syötä summa (5 € välein)";
    } else if (selectedLanguage == "SWE") {
        dialogTitle = "Ange uttagsbelopp";
        dialogLabel = "Ange belopp (5 € intervall)";
    } else if (selectedLanguage == "ENG") {
        dialogTitle = "Enter withdrawal amount";
        dialogLabel = "Enter amount (in 5 € increments)";
    } else {
        dialogTitle = "Anna nostosumma";
        dialogLabel = "Syötä summa (5 € välein)";
    }

    int customAmount = QInputDialog::getInt(
        this,
        dialogTitle,
        dialogLabel,
        5,
        5,
        1000,
        5,
        &ok);

    if (ok) {
        if (customAmount % 5 == 0) {
            fetchAccountDetails(customAmount);
        } else {
            QString errorTitle;
            QString errorMessage;

            if (selectedLanguage == "FI") {
                errorTitle = "Virhe";
                errorMessage = "Vain summat 5 € välein.";
            } else if (selectedLanguage == "SWE") {
                errorTitle = "Fel";
                errorMessage = "Endast belopp i 5 € intervall.";
            } else if (selectedLanguage == "ENG") {
                errorTitle = "Error";
                errorMessage = "Only amounts in 5 € increments.";
            } else {
                errorTitle = "Virhe";
                errorMessage = "Vain summat 5 € välein.";
            }

            QMessageBox::warning(this, errorTitle, errorMessage);
        }
    }
}

void WithdrawWindow::fetchAccountDetails(int amount)
{
    QString url = Environment::base_url() + "/card_account?idcard=" + mIdcard;
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", mToken);

    QNetworkReply *reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() {
        onFetchAccountDetailsFinished(reply, amount);
    });
}

void WithdrawWindow::onFetchAccountDetailsFinished(QNetworkReply *reply, int amount)
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

        int selectedAccountId = -1;
        for (auto val : arr) {
            QJsonObject obj = val.toObject();
            if (obj["type"].toString() == mCardMode) {
                selectedAccountId = obj["idaccount"].toInt();
                break;
            }
        }

        if (selectedAccountId == -1) {
            QMessageBox::warning(this, "Error", "No matching account type found (" + mCardMode + ")!");
        } else {
            processWithdrawal(selectedAccountId, mCardMode, amount);
        }
    } else {
        QMessageBox::critical(this, "Error", "Failed to retrieve card_account: " + reply->errorString());
    }
    reply->deleteLater();
}

void WithdrawWindow::processWithdrawal(int accountId, const QString &accountType, double amount)
{
    QString url = Environment::base_url() + "/account/" + QString::number(accountId);
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", mToken);

    QNetworkReply *getReply = networkManager->get(request);

    connect(getReply, &QNetworkReply::finished, this, [=]() {
        if (getReply->error() == QNetworkReply::NoError) {
            QByteArray responseData = getReply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(responseData);
            QJsonObject obj = doc.object();

            bool success = false;

            QString errorMessage;

            if (accountType == "debit") {
                double currentBalance = obj["debit_balance"].toString().toDouble();
                if (currentBalance >= amount) {
                    obj["debit_balance"] = QString::number(currentBalance - amount, 'f', 2);
                    success = true;
                } else {
                    if (selectedLanguage == "SWE") {
                        errorMessage = "Otillräckliga medel.";
                    } else if (selectedLanguage == "ENG") {
                        errorMessage = "Insufficient funds.";
                    } else {
                        errorMessage = "Tilillä ei ole katetta.";
                    }
                    QMessageBox::warning(this, "Error", errorMessage);
                }
            }
            else if (accountType == "credit") {
                double currentBalance = obj["credit_balance"].toString().toDouble();
                double creditLimit = obj["credit_limit"].toString().toDouble();
                double newBalance = currentBalance - amount;

                if (newBalance >= -creditLimit) {
                    obj["credit_balance"] = QString::number(newBalance, 'f', 2);
                    success = true;
                } else {
                    if (selectedLanguage == "SWE") {
                        errorMessage = "Kreditgränsen överskriden.";
                    } else if (selectedLanguage == "ENG") {
                        errorMessage = "Credit limit exceeded.";
                    } else {
                        errorMessage = "Luottoraja ylittyy.";
                    }
                    QMessageBox::warning(this, "Error", errorMessage);
                }
            }

            if (success) {
                QJsonDocument updateDoc(obj);
                QByteArray updateData = updateDoc.toJson();

                QNetworkRequest putReq(getReply->url());
                putReq.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
                putReq.setRawHeader("Authorization", mToken);

                QNetworkReply *putReply = networkManager->sendCustomRequest(putReq, "PUT", updateData);
                connect(putReply, &QNetworkReply::finished, this, [=]() {
                    onWithdrawalResponse(putReply, amount);
                });
            }
        }
        getReply->deleteLater();
    });
}

void WithdrawWindow::onWithdrawalResponse(QNetworkReply *reply, int amount)
{
    QString successTitle, successMessage, errorTitle, errorMessage;

    if (selectedLanguage == "SWE") {
        successTitle = "Lyckat";
        successMessage = QString("Uttag lyckades (%1 €).").arg(amount);
        errorTitle = "Fel";
        errorMessage = "Uppdatering av uttaget misslyckades: ";
    } else if (selectedLanguage == "ENG") {
        successTitle = "Success";
        successMessage = QString("Withdrawal successful (%1 €).").arg(amount);
        errorTitle = "Error";
        errorMessage = "Withdrawal update failed: ";
    } else {
        successTitle = "Onnistui";
        successMessage = QString("Nosto onnistui (%1 €).").arg(amount);
        errorTitle = "Virhe";
        errorMessage = "Noston päivitys epäonnistui: ";
    }

    if (reply->error() == QNetworkReply::NoError) {
        QMessageBox::information(this, successTitle, successMessage);
    } else {
        QMessageBox::critical(this, errorTitle, errorMessage + reply->errorString());
    }

    reply->deleteLater();
}
