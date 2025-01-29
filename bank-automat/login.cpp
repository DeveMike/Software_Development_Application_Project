#include "enviroment.h"
#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::on_btnLogin_clicked()
{
    QJsonObject jsonObj;
    jsonObj.insert("idcard",ui->numberIdcard->text());
    jsonObj.insert("pin",ui->numberPin->text());

    QString site_url=Enviroment::base_url()+"/login";
    QNetworkRequest request(site_url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    postManager = new QNetworkAccessManager(this);
    connect(postManager,SIGNAL(finished(QNetworkReply*)), this, SLOT(LoginSlot(QNetworkReply*)));
    reply = postManager->post(request, QJsonDocument(jsonObj).toJson());



}

void Login::LoginSlot(QNetworkReply *reply)
{
    response_data=reply->readAll();
    if(response_data.length()<2){
        qDebug()<<"Palvelin ei vastaa!";
        ui->labelInfo->setText("Palvelin ei vastaa!");
    }
    else{
        if(response_data=="!404!"){
            ui->labelInfo->setText("Tietokanta virhe!");

        }
        else{
            if(response_data!="false" && response_data.length()>20){
                ui->labelInfo->setText("Kirjautuminen OK!");

            }
            else{
                ui->labelInfo->setText("Väärä ID/PIN");

            }
        }
    }
    reply->deleteLater();
    postManager->deleteLater();
}

