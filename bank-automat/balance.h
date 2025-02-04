#ifndef BALANCEWINDOW_H
#define BALANCEWINDOW_H

#include <QDialog>

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

private slots:
    void on_btnBack_clicked();

protected:
    void closeEvent(QCloseEvent *) override;

private:
    Ui::BalanceWindow *ui;
    QString selectedLanguage;
    void updateLanguage();
};

#endif // BALANCEWINDOW_H
