#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QLineEdit>
#include <QComboBox>
#include <QGridLayout>
#include <QStringList>
#include <QDebug>

class LoginDialog : public QDialog {
 Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent=nullptr);
    ~LoginDialog();
    void loginFailed();
    void enableButton(bool enabled);

private:
    QLabel *labelUsername;
    QLabel *labelPassword;
    QLabel *labelError;
    QLabel *spinnerLabel;
    QMovie *spinnerMovie;
    QLineEdit *comboUsername;
    QLineEdit *editPassword;
    QPushButton *loginButton;
    QPushButton *cancelButton;
    QPushButton *registerButton;

    void setUpGUI();

signals:
    void tryLogin(QString &username, QString &password);
    void openRegister();

public slots:
    void slotTryLogin();
    void loginCorrect();
};

#endif // LOGINDIALOG_H
