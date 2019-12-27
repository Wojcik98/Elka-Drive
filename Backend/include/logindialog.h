#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QComboBox>
#include <QGridLayout>
#include <QStringList>
#include <QDebug>

class LoginDialog : public QDialog {
 Q_OBJECT

private:
    QLabel *labelUsername;
    QLabel *labelPassword;
    QLabel *labelError;
    QLabel *spinnerLabel;
    QMovie *spinnerMovie;
    QComboBox *comboUsername;
    QLineEdit *editPassword;
    QDialogButtonBox *buttons;

    void setUpGUI();

public:
    explicit LoginDialog(QWidget *parent=nullptr);
    ~LoginDialog();

    void setUsername(QString &username);
    void setPassword(QString &password);
    void setUsernamesList(const QStringList &usernames);

signals:
    void tryLogin(QString &username, QString &password);

public slots:
    void slotTryLogin();
    void slotLoginResponse(bool succes);
};

#endif // LOGINDIALOG_H
