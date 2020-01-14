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

/*!
 * \brief LoginDialog wyświetla okno logowania.
 */
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
    QHBoxLayout *buttonsLayout;
    QGridLayout* mainLayout;

    void setUpGUI();
    void initInputs();
    void initLabels();
    void initButtons();
    void initSpinner();
    void connectSlots();
    void placeComponents();

signals:
    /*!
     * \brief Żądanie logowania.
     * \param username Nazwa użytkownika
     * \param password Hasło
     */
    void tryLogin(QString &username, QString &password);

    /*!
     * \brief Żądanie otworzeina okna rejestracji.
     */
    void openRegister();

public slots:
    void slotTryLogin();
    void loginCorrect();
};

#endif // LOGINDIALOG_H
