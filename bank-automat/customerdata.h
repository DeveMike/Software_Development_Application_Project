#ifndef CUSTOMERDATA_H
#define CUSTOMERDATA_H

#include <QDialog>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QFileDialog>       // tarvitaan tiedostonvalintaa varten
#include <QHttpMultiPart>    // tarvitaan multi-part lähetykseen

namespace Ui {
class CustomerData;
}

class CustomerData : public QDialog
{
    Q_OBJECT

public:
    explicit CustomerData(QWidget *parent = nullptr);
    ~CustomerData();
    void setIdcard(const QString &newIdcard);
    void setMyToken(const QByteArray &newMyToken);
    void setLanguage(const QString &newLanguage);

private slots:
    void showDataSlot(QNetworkReply *reply);
    void on_btnBack_clicked();
    void onThumbnailDownloaded(QNetworkReply *reply);       // Uusi slot: käsittelee ladatun kuvan
    void onUploadThumbnailFinished(QNetworkReply *reply);     // Uusi slot: käsittelee lähetyksen vastauksen
    void onBtnUploadThumbnailClicked();                       // Uusi slot: painikkeen klikkaus

protected:
    void showEvent(QShowEvent *) override;
    void closeEvent(QCloseEvent *) override;

private:
    Ui::CustomerData *ui;
    QString idcard;
    QByteArray myToken;
    QString selectedLanguage;

    QNetworkAccessManager *dataManager;
    QNetworkReply *reply;
    QByteArray response_data;
    void updateLanguage();

    // Uudet jäsenmuuttujat profiilikuvan toiminnallisuutta varten
    int customerId; // tallentaa asiakkaan id:n
    QNetworkAccessManager *thumbnailManager;
    QNetworkAccessManager *uploadManager;

    // Uudet funktiot profiilikuvan latausta ja lähettämistä varten
    void loadUserThumbnail(int userId);
    void uploadNewThumbnail(int userId, QString filePath);
};

#endif // CUSTOMERDATA_H
