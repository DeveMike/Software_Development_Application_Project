#include "environment.h"
#include "login.h"
#include "mainmenu.h"
#include "cardmode.h"
#include "ui_login.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QCloseEvent>
#include <QDebug>

Login::Login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    this->setWindowTitle("ATM");
    updateLanguage();
}

Login::~Login()
{
    delete ui;
}

void Login::on_btnLogin_clicked()
{
    QJsonObject jsonObj;
    jsonObj.insert("idcard", ui->numberIdcard->text());
    jsonObj.insert("pin", ui->numberPin->text());

    QString site_url = Environment::base_url() + "/login";
    QNetworkRequest request(site_url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    postManager = new QNetworkAccessManager(this);
    connect(postManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(LoginSlot(QNetworkReply*)));

    reply = postManager->post(request, QJsonDocument(jsonObj).toJson());
}

void Login::LoginSlot(QNetworkReply *reply)
{
    response_data = reply->readAll();
    qDebug() << "LoginSlot response_data:" << response_data;

    if (response_data.length() < 2) {
        qDebug() << "Server is not responding!";
        if (selectedLanguage == "FI") {
            ui->labelInfo->setText("Palvelin ei vastaa!");
        } else if (selectedLanguage == "SWE") {
            ui->labelInfo->setText("Servern svarar inte!");
        } else {
            ui->labelInfo->setText("Server is not responding!");
        }
    }
    else if (response_data == "!404!") {
        if (selectedLanguage == "FI") {
            ui->labelInfo->setText("Tietokantavirhe!");
        } else if (selectedLanguage == "SWE") {
            ui->labelInfo->setText("Databasfel!");
        } else {
            ui->labelInfo->setText("Database error!");
        }
    }
    else {
        if (response_data != "false" && response_data.length() > 20) {
            if (selectedLanguage == "FI") {
                ui->labelInfo->setText("Kirjautuminen onnistui!");
            } else if (selectedLanguage == "SWE") {
                ui->labelInfo->setText("Inloggning lyckades!");
            } else {
                ui->labelInfo->setText("Login OK!");
            }

            myToken = "Bearer " + response_data;

            QString card_url = Environment::base_url() + "/card/" + ui->numberIdcard->text();
            QNetworkRequest request(card_url);
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
            request.setRawHeader("Authorization", myToken.toUtf8());

            getCardManager = new QNetworkAccessManager(this);
            connect(getCardManager, SIGNAL(finished(QNetworkReply*)),
                    this, SLOT(cardSlot(QNetworkReply*)));
            getCardManager->get(request);
        } else {
            if (selectedLanguage == "FI") {
                ui->labelInfo->setText("Väärä ID/PIN!");
            } else if (selectedLanguage == "SWE") {
                ui->labelInfo->setText("Fel ID/PIN!");
            } else {
                ui->labelInfo->setText("Incorrect ID/PIN!");
            }
        }
    }

    reply->deleteLater();
    postManager->deleteLater();
}

void Login::cardSlot(QNetworkReply *reply)
{
    QByteArray cardResponse = reply->readAll();
    qDebug() << "cardSlot response_data:" << cardResponse;

    QJsonDocument doc = QJsonDocument::fromJson(cardResponse);
    if (!doc.isObject()) {
        if (selectedLanguage == "FI") {
            ui->labelInfo->setText("Virheellinen JSON (kortti) -vastaus!");
        } else if (selectedLanguage == "SWE") {
            ui->labelInfo->setText("Ogiltigt JSON-svar (kort)!");
        } else {
            ui->labelInfo->setText("Invalid JSON (card) response!");
        }
        qDebug() << "Card response is not a JSON object";
    }
    else {
        QJsonObject json = doc.object();
        QString cardType = json.value("cardtype").toString();
        qDebug() << "Card type:" << cardType;

        this->hide();

        if (cardType == "dual") {
            CardMode *objCardMode = new CardMode(this);
            objCardMode->setGeometry(this->geometry());
            objCardMode->setUsername(ui->numberIdcard->text());
            objCardMode->setMyToken(myToken);
            objCardMode->setLanguage(selectedLanguage);
            objCardMode->show();
            connect(objCardMode, &CardMode::finished, this, &Login::show);

        } else {
            MainMenu *objMainMenu = new MainMenu(this);
            objMainMenu->setGeometry(this->geometry());
            objMainMenu->setMyToken(myToken.toUtf8());
            objMainMenu->setUsername(ui->numberIdcard->text());
            objMainMenu->setLanguage(selectedLanguage);
            objMainMenu->setCardMode("debit");
            objMainMenu->show();
            connect(objMainMenu, &MainMenu::finished, this, &Login::show);
        }
    }

    reply->deleteLater();
    getCardManager->deleteLater();
}

void Login::on_btnLangFI_clicked()
{
    selectedLanguage = "FI";
    qDebug() << "Language set: Suomi";
    updateLanguage();
}

void Login::on_btnLangSWE_clicked()
{
    selectedLanguage = "SWE";
    qDebug() << "Language set: Svenska";
    updateLanguage();
}

void Login::on_btnLangENG_clicked()
{
    selectedLanguage = "ENG";
    qDebug() << "Language set: English";
    updateLanguage();
}

void Login::updateLanguage()
{
    if (selectedLanguage == "FI") {
        ui->label->setText("Kortin tunnus:");
        ui->label_2->setText("Kortin PIN:");
        ui->btnLogin->setText("Kirjaudu");
    }
    else if (selectedLanguage == "SWE") {
        ui->label->setText("Kortnummer:");
        ui->label_2->setText("Kort-PIN:");
        ui->btnLogin->setText("Logga in");
    }
    else {
        ui->label->setText("Card ID:");
        ui->label_2->setText("Card PIN:");
        ui->btnLogin->setText("Login");
    }
}

void Login::closeEvent(QCloseEvent *)
{
    QApplication::quit();
}
