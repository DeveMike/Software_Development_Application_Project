#ifndef CARDMODE_H
#define CARDMODE_H

#include <QDialog>

namespace Ui {
class CardMode;
}

class CardMode : public QDialog
{
    Q_OBJECT

public:
    explicit CardMode(QWidget *parent = nullptr);
    ~CardMode();

    void setLanguage(const QString &lang);
    void setUsername(const QString &username);
    void setMyToken(const QString &token);

private slots:
    void on_btnDebit_clicked();
    void on_btnCredit_clicked();
    void on_btnBack_clicked();

protected:
    void closeEvent(QCloseEvent *) override;

private:
    Ui::CardMode *ui;
    QString mLanguage;
    QString mUsername;
    QString mToken;
};

#endif // CARDMODE_H
