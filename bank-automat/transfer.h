#ifndef TRANSFERWINDOW_H
#define TRANSFERWINDOW_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Ui {
class TransferWindow;
}

class TransferWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TransferWindow(QWidget *parent = nullptr);
    ~TransferWindow();

    void setIdcard(const QString &idcard);
    void setCardMode(const QString &mode);
    void setMyToken(const QByteArray &token);
    void setLanguage(const QString &newLanguage);
    void showCenteredInformation();

private slots:
    void onTransferResponse(QNetworkReply *reply, double amount, int receiverAccountId);
    void on_btnBack_clicked();
    void on_buttonTransfer_clicked();

    void on_pushButton_clicked();

protected:
    void closeEvent(QCloseEvent *) override;

private:
    Ui::TransferWindow *ui;

    void fetchAccountDetails(double amount, int receiverAccountId);
    void onFetchAccountDetailsFinished(QNetworkReply *reply, double amount, int receiverAccountId);
    void processTransfer(int senderAccountId, const QString &accountType, double amount, int receiverAccountId);

    QNetworkAccessManager *networkManager;
    QString mIdcard;
    QString mCardMode;
    QByteArray mToken;
    QString selectedLanguage;
    void updateLanguage();
};

#endif // TRANSFERWINDOW_H
