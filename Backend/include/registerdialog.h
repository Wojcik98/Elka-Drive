#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QLineEdit>
#include <QComboBox>
#include <QGridLayout>
#include <QStringList>
#include <QDebug>
#include <QMovie>

class RegisterDialog : public QDialog {
Q_OBJECT

private:
   QLabel *labelUsername;
   QLabel *labelPassword;
   QLabel *labelError;
   QLabel *spinnerLabel;
   QMovie *spinnerMovie;
   QComboBox *comboUsername;
   QLineEdit *editPassword;
   QPushButton *cancelButton;
   QPushButton *registerButton;

   void setUpGUI();

public:
   explicit RegisterDialog(QWidget *parent=nullptr);
   ~RegisterDialog();

   void setUsername(QString &username); // TODO delete
   void setPassword(QString &password);
   void setUsernamesList(const QStringList &usernames);
   void userExists();

signals:
   void tryLogin(QString &username, QString &password);

public slots:
   void slotTryRegister();
   void slotRegisterResponse();
};

#endif // REGISTERDIALOG_H
