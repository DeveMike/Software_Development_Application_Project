#ifndef CUSTOMERDATA_H
#define CUSTOMERDATA_H

#include <QDialog>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QFileDialog>
#include <QHttpMultiPart>

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
    void onThumbnailDownloaded(QNetworkReply *reply);
    void onUploadThumbnailFinished(QNetworkReply *reply);
    void onBtnUploadThumbnailClicked();

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

    int customerId;
    QNetworkAccessManager *thumbnailManager;
    QNetworkAccessManager *uploadManager;

    void loadUserThumbnail(int userId);
    void uploadNewThumbnail(int userId, QString filePath);
};

#endif // CUSTOMERDATA_H
