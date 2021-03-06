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
#include <QTimer>
#include <QMovie>

/*!
 * \brief RegisterDialog wyświetla okno rejestracji.
 */
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
   QTimer closeTimer;

   void setUpGUI();
   void initInputs();
   void initLabels();
   void initButtons();
   void initSpinner();
   void connectSlots();
   void placeComponents();

signals:
   /*!
    * \brief Żądanie rejestracji.
    * \param username Nazwa użytkownika
    * \param password Hasło
    */
   void tryRegister(const QString &username, const QString &password);

public slots:
   void slotTryRegister();
   void slotRegisterResponse();
};

#endif // REGISTERDIALOG_H
