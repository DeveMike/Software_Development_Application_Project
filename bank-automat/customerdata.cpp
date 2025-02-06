#include "customerdata.h"
#include "environment.h"
#include "ui_customerdata.h"
#include <QDebug>
#include <QFileDialog>
#include <QHttpMultiPart>
#include <QFile>
#include <QPixmap>
#include <QFileInfo>
#include <QApplication>

CustomerData::CustomerData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomerData),
    customerId(0),
    thumbnailManager(new QNetworkAccessManager(this)),
    uploadManager(new QNetworkAccessManager(this))
{
    ui->setupUi(this);
    this->setWindowTitle("ATM");

    connect(thumbnailManager, &QNetworkAccessManager::finished, this, &CustomerData::onThumbnailDownloaded);
    connect(uploadManager, &QNetworkAccessManager::finished, this, &CustomerData::onUploadThumbnailFinished);
    connect(ui->btnUploadThumbnail, &QPushButton::clicked, this, &CustomerData::onBtnUploadThumbnailClicked);
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

void CustomerData::setLanguage(const QString &newLanguage) {
    selectedLanguage = newLanguage;
    updateLanguage();
}

void CustomerData::updateLanguage()
{
    if (selectedLanguage == "FI") {
        ui->labelData->setText("Asiakastiedot:");
        ui->labelID->setText("Asiakas ID:");
        ui->labelData_3->setText("Etunimi:");
        ui->labelData_2->setText("Sukunimi:");
        ui->btnBack->setText("Takaisin");
        ui->btnUploadThumbnail->setText("Vaihda kuva");
    }
    else if (selectedLanguage == "SWE") {
        ui->labelData->setText("Kunduppgifter:");
        ui->labelID->setText("Kund ID:");
        ui->labelData_3->setText("Förnamn:");
        ui->labelData_2->setText("Efternamn:");
        ui->btnBack->setText("Tillbaka");
        ui->btnUploadThumbnail->setText("Byt bild");
    }
    else if (selectedLanguage == "ENG") {
        ui->labelData->setText("Customer Data:");
        ui->labelID->setText("Customer ID:");
        ui->labelData_3->setText("First Name:");
        ui->labelData_2->setText("Last Name:");
        ui->btnBack->setText("Back");
        ui->btnUploadThumbnail->setText("Change image");
    }
}

void CustomerData::showEvent(QShowEvent *event)
{
    QString site_url = Environment::base_url() + "/card/" + idcard;
    QNetworkRequest request(site_url);

    //WEBTOKEN START
    request.setRawHeader(QByteArray("Authorization"), myToken);
    //WEBTOKEN END
    dataManager = new QNetworkAccessManager(this);

    connect(dataManager, &QNetworkAccessManager::finished, this, &CustomerData::showDataSlot);

    reply = dataManager->get(request);

    QDialog::showEvent(event);
}

void CustomerData::showDataSlot(QNetworkReply *reply)
{
    response_data = reply->readAll();
    qDebug() << "Received data: " << response_data;

    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    QJsonObject json_obj = json_doc.object();

    int id = json_obj["idcustomer"].toInt();
    customerId = id;
    ui->labelID->setText(QString::number(id));

    loadUserThumbnail(id);

    QString site_url = Environment::base_url() + "/customer/" + QString::number(id);
    QNetworkRequest request(site_url);
    request.setRawHeader(QByteArray("Authorization"), myToken);

    QNetworkAccessManager *customerDataManager = new QNetworkAccessManager(this);
    connect(customerDataManager, &QNetworkAccessManager::finished, this, [=](QNetworkReply *customerReply) {
        QByteArray customerData = customerReply->readAll();
        qDebug() << "Customer information: " << customerData;

        QJsonDocument customerJsonDoc = QJsonDocument::fromJson(customerData);
        QJsonObject customerJsonObj = customerJsonDoc.object();

        ui->labelFname->setText(customerJsonObj["fname"].toString());
        ui->labelLname->setText(customerJsonObj["lname"].toString());

        customerReply->deleteLater();
        customerDataManager->deleteLater();
    });

    customerDataManager->get(request);

    reply->deleteLater();
    dataManager->deleteLater();
}

void CustomerData::loadUserThumbnail(int userId)
{
    QUrl url(Environment::base_url() + "/customer/getThumbnail?userId=" + QString::number(userId));
    QNetworkRequest request(url);
    request.setRawHeader(QByteArray("Authorization"), myToken);
    thumbnailManager->get(request);
}

void CustomerData::onThumbnailDownloaded(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray imageData = reply->readAll();
        QPixmap pixmap;
        if (pixmap.loadFromData(imageData)) {
            ui->lblThumbnail->setPixmap(pixmap.scaled(128, 128, Qt::KeepAspectRatio));
            qDebug() << "Thumbnail downloaded successfully!";
        } else {
            qDebug() << "Failed to load the image.";
        }
    } else {
        qDebug() << "Error downloading the thumbnail:" << reply->errorString();
    }
    reply->deleteLater();
}

void CustomerData::uploadNewThumbnail(int userId, QString filePath)
{
    QUrl apiUrl(Environment::base_url() + "/customer/thumbnail");
    QNetworkRequest request(apiUrl);
    request.setRawHeader(QByteArray("Authorization"), myToken);

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart userIdPart;
    userIdPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"userId\""));
    userIdPart.setBody(QString::number(userId).toUtf8());
    multiPart->append(userIdPart);

    QFile *file = new QFile(filePath);
    if (!file->open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open the file.";
        delete file;
        multiPart->deleteLater();
        return;
    }

    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));
    QFileInfo fileInfo(file->fileName());
    QString fileName = fileInfo.fileName();
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"thumbnail\"; filename=\"" + fileName + "\""));
    imagePart.setBodyDevice(file);
    file->setParent(multiPart);
    multiPart->append(imagePart);

    QNetworkReply *reply = uploadManager->post(request, multiPart);
    multiPart->setParent(reply);
}

void CustomerData::onUploadThumbnailFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "Thumbnail upload successful!";
        if (customerId != 0) {
            loadUserThumbnail(customerId);
        }
    } else {
        qDebug() << "Error uploading thumbnail:" << reply->errorString();
    }
    reply->deleteLater();
}

void CustomerData::onBtnUploadThumbnailClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Select Image", "", "Images (*.png *.jpg *.jpeg)");
    if (!filePath.isEmpty() && customerId != 0) {
        uploadNewThumbnail(customerId, filePath);
    }
}

void CustomerData::on_btnBack_clicked()
{
    this->hide();
    QWidget *mainMenu = parentWidget();

    if (mainMenu) {
        mainMenu->show();
    }
}

void CustomerData::closeEvent(QCloseEvent *)
{
    QApplication::quit();
}
