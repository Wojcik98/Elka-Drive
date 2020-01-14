#include "include/registerdialog.h"

/*!
 * \brief Konstruktor.
 * \param parent Rodzic w strukturze Qt
 */
RegisterDialog::RegisterDialog(QWidget *parent) : QDialog(parent) {
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    setUpGUI();
    setWindowTitle(tr("New account"));
    setModal(true);

    closeTimer.setInterval(1000);
    closeTimer.setSingleShot(true);
}

/*!
 * \brief Destruktor.
 */
RegisterDialog::~RegisterDialog() {
    spinnerMovie->deleteLater();
}

void RegisterDialog::setUpGUI() {
    mainLayout = new QGridLayout(this);

    initInputs();
    initLabels();
    initButtons();
    initSpinner();
    connectSlots();
    placeComponents();
}

void RegisterDialog::initInputs() {
    editUsername = new QLineEdit(this);
    editPassword = new QLineEdit(this);
    editPassword->setEchoMode(QLineEdit::Password);
}

void RegisterDialog::initLabels() {
    labelUsername = new QLabel(this);
    labelPassword = new QLabel(this);
    labelError = new QLabel(this);
    labelUsername->setText(tr("Username"));
    labelUsername->setBuddy(editUsername);
    labelPassword->setText(tr("Password"));
    labelPassword->setBuddy(editPassword);
    labelError->setText("");
}

void RegisterDialog::initButtons() {
    registerButton = new QPushButton(QIcon(":/icons/register.svg"), "Register", this);
    cancelButton = new QPushButton(QIcon(":/icons/cancel.svg"), "Cancel", this);
    buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(cancelButton);
    buttonsLayout->addWidget(registerButton);
}

void RegisterDialog::initSpinner() {
    spinnerLabel = new QLabel(this);
    spinnerMovie = new QMovie(":/gifs/spinner.gif");
    spinnerMovie->start();
    spinnerLabel->setMovie(spinnerMovie);
    buttonsLayout->addWidget(spinnerLabel);
    spinnerLabel->hide();
}

void RegisterDialog::placeComponents() {
    mainLayout->addWidget(labelUsername, 0, 0);
    mainLayout->addWidget(editUsername, 0, 1, 1, 2);
    mainLayout->addWidget(labelPassword, 1, 0);
    mainLayout->addWidget(editPassword, 1, 1);
    mainLayout->addWidget(editPassword, 1, 1, 1, 2);
    mainLayout->addWidget(labelError, 2, 0, 1, 2);
    mainLayout->addLayout(buttonsLayout, 3, 0, 1, 2);
}

void RegisterDialog::connectSlots() {
    connect(
        cancelButton,
        &QPushButton::clicked,
        this,
        &RegisterDialog::reject
    );
    connect(
        registerButton,
        &QPushButton::clicked,
        this,
        &RegisterDialog::slotTryRegister
    );
    connect(
        &closeTimer,
        &QTimer::timeout,
        this,
        &RegisterDialog::reject
    );
}

/*!
 * \brief Wysyła żądanie rejestracji z podanymi danymi.
 */
void RegisterDialog::slotTryRegister() {
    spinnerLabel->show();
    enableButton(false);
    QString username = editUsername->text();
    QString password = editPassword->text();

    emit tryRegister(username, password);
}

/*!
 * \brief Wyświetla komunikat o udanej rejestracji.
 */
void RegisterDialog::slotRegisterResponse() {
    labelError->setStyleSheet("color: green;");
    labelError->setText("Register successful!");

    closeTimer.start();
    enableButton(true);
    spinnerLabel->hide();
}

/*!
 * \brief Wyświetla komunikat, że użytkownik o podanej nazwie już istnieje.
 */
void RegisterDialog::userExists() {
    labelError->setStyleSheet("color: red;");
    labelError->setText("User exists!");

    enableButton(true);
    spinnerLabel->hide();
}

/*!
 * \brief Aktywuje przycisk rejestracji.
 * \param enabled Czy aktywować
 */
void RegisterDialog::enableButton(bool enabled) {
    registerButton->setEnabled(enabled);
}
