#ifndef WITHDRAWWINDOW_H
#define WITHDRAWWINDOW_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Ui {
class WithdrawWindow;
}

class WithdrawWindow : public QDialog
{
    Q_OBJECT

public:
    explicit WithdrawWindow(QWidget *parent = nullptr);
    ~WithdrawWindow();

    void setIdcard(const QString &idcard);
    void setCardMode(const QString &mode);
    void setMyToken(const QByteArray &token);
    void setLanguage(const QString &newLanguage);
    void showCenteredInformation();

private slots:
    void onWithdrawalResponse(QNetworkReply *reply, int amount);
    void on_btnBack_clicked();
    void on_btnBack_2_clicked();
    void on_buttonOut_20_clicked();
    void on_buttonOut_40_clicked();
    void on_buttonOut_50_clicked();
    void on_buttonOut_80_clicked();
    void on_buttonOut_100_clicked();
    void on_buttonOut_x_clicked();
    void onDigitButtonClicked();
    void on_btnOK_clicked();
    void hideCustomAmountInput();

protected:
    void closeEvent(QCloseEvent *) override;

private:
    Ui::WithdrawWindow *ui;

    void fetchAccountDetails(int amount);
    void onFetchAccountDetailsFinished(QNetworkReply *reply, int amount);
    void processWithdrawal(int accountId, const QString &accountType, double amount);
    void onWithdrawalResponse(QNetworkReply *reply);

    QNetworkAccessManager *networkManager;
    QString mIdcard;
    QString mCardMode;
    QByteArray mToken;
    QString selectedLanguage;
    void updateLanguage();

    QString insufficientFundsMsg() const;
    QString creditLimitExceededMsg() const;
    QString successWithdrawMsg(int amount) const;
    QString errorWithdrawMsg(const QString &errorStr) const;
};

#endif // WITHDRAWWINDOW_H
