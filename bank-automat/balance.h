#ifndef BALANCEWINDOW_H
#define BALANCEWINDOW_H

#include <QDialog>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

namespace Ui {
class BalanceWindow;
}

class BalanceWindow : public QDialog
{
    Q_OBJECT

public:
    explicit BalanceWindow(QWidget *parent = nullptr);
    ~BalanceWindow();

    void setLanguage(const QString &newLanguage);
    void setAuthToken(const QByteArray &token);
    void setIdCard(const QString &id);

private slots:
    void on_btnBack_clicked();

protected:
    void closeEvent(QCloseEvent *) override;

private:
    Ui::BalanceWindow *ui;
    QString selectedLanguage;
    QByteArray authToken;
    QString idCard;
    QNetworkAccessManager *networkManager;

    // Uudet metodit
    void fetchAccountDetails();
    void handleAccountDetails(QNetworkReply *reply);
    void fetchAccountBalance(int accountId, const QString &accountType);
    void handleBalanceResponse(QNetworkReply *reply, const QString &accountType);
};

#endif // BALANCEWINDOW_H
