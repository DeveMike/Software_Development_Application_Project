#include "customerdata.h"
#include "environment.h"
#include "ui_customerdata.h"
#include <QDebug>

CustomerData::CustomerData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomerData)
{
    ui->setupUi(this);
}

CustomerData::~CustomerData()
{
    delete ui;
}

void CustomerData::setMyToken(const QByteArray &newMyToken) {
    myToken = newMyToken;
}

void CustomerData::setIdcard(const QString &newIdcard) {
    idcard = newIdcard;
}


void CustomerData::showEvent(QShowEvent *event)
{
    QString site_url = Environment::base_url() + "/card/" + idcard;
    QNetworkRequest request(site_url);

    //WEBTOKEN ALKU
    request.setRawHeader(QByteArray("Authorization"), myToken);
    //WEBTOKEN LOPPU
    dataManager = new QNetworkAccessManager(this);

    connect(dataManager, &QNetworkAccessManager::finished, this, &CustomerData::showDataSlot);

    reply = dataManager->get(request);

    QDialog::showEvent(event);
}


void CustomerData::showDataSlot(QNetworkReply *reply)
{

    response_data = reply->readAll();
    qDebug() << response_data;

    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    QJsonObject json_obj = json_doc.object();

    // QString myData;
    // // idcustomer muutetaan kokonaisluvuksi ja sitten merkkijonoksi
    // myData += QString::number(json_obj["idcustomer"].toInt()) + " " + json_obj["fname"].toString() + " " + json_obj["lname"].toString();
    // ui->labelData->setText(myData);

    ui->labelID->setText(QString::number(json_obj["idcustomer"].toInt()));
    ui->labelFname->setText(json_obj["fname"].toString());
    ui->labelLname->setText(json_obj["lname"].toString());



    reply->deleteLater();
    dataManager->deleteLater();
}



