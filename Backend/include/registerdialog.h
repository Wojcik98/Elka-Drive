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

public:
   explicit RegisterDialog(QWidget *parent=nullptr);
   ~RegisterDialog();

   void userExists();
   void enableButton(bool enabled);

private:
   QLabel *labelUsername;
   QLabel *labelPassword;
   QLabel *labelError;
   QLabel *spinnerLabel;
   QMovie *spinnerMovie;
   QLineEdit *editUsername;
   QLineEdit *editPassword;
   QPushButton *cancelButton;
   QPushButton *registerButton;
   QHBoxLayout *buttonsLayout;
   QGridLayout *mainLayout;

   void setUpGUI();
   void initInputs();
   void initLabels();
   void initButtons();
   void initSpinner();
   void connectSlots();
   void placeComponents();

signals:
   void tryRegister(const QString &username, const QString &password);

public slots:
   void slotTryRegister();
   void slotRegisterResponse();
};

#endif // REGISTERDIALOG_H
