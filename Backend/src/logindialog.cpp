#include <QMovie>
#include "include/logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent) {
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    setUpGUI();
    setWindowTitle(tr("User Login"));
    setModal(true);
}

LoginDialog::~LoginDialog() {
    spinnerMovie->deleteLater();
}

void LoginDialog::setUpGUI() {
    mainLayout = new QGridLayout(this);

    initInputs();
    initLabels();
    initButtons();
    initSpinner();
    connectSlots();
    placeComponents();
}

void LoginDialog::initInputs() {
    comboUsername = new QLineEdit(this);
    editPassword = new QLineEdit(this);
    editPassword->setEchoMode(QLineEdit::Password);
}

void LoginDialog::initLabels() {
    labelUsername = new QLabel(this);
    labelPassword = new QLabel(this);
    labelError = new QLabel(this);
    labelUsername->setText(tr("Username"));
    labelUsername->setBuddy(comboUsername);
    labelPassword->setText(tr("Password"));
    labelPassword->setBuddy(editPassword);
    labelError->setText("");
    labelError->setStyleSheet("color: red;");
}

void LoginDialog::initButtons() {
    loginButton = new QPushButton(QIcon(":/icons/login.svg"), "Login", this);
    registerButton = new QPushButton(QIcon(":/icons/register.svg"), "Register", this);
    cancelButton = new QPushButton(QIcon(":/icons/cancel.svg"), "Close", this);
    buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(cancelButton);
    buttonsLayout->addWidget(registerButton);
    buttonsLayout->addWidget(loginButton);
}

void LoginDialog::initSpinner() {
    spinnerLabel = new QLabel(this);
    spinnerMovie = new QMovie(":/gifs/spinner.gif");
    spinnerMovie->start();
    spinnerLabel->setMovie(spinnerMovie);
    buttonsLayout->addWidget(spinnerLabel);

    spinnerLabel->hide();
}

void LoginDialog::connectSlots() {
    connect(
        cancelButton,
        &QPushButton::clicked,
        this,
        &LoginDialog::reject
    );
    connect(
        loginButton,
        &QPushButton::clicked,
        this,
        &LoginDialog::slotTryLogin
    );
    connect(
        registerButton,
        &QPushButton::clicked,
        this,
        &LoginDialog::openRegister
    );
}

void LoginDialog::placeComponents() {
    mainLayout->addWidget(labelUsername, 0, 0);
    mainLayout->addWidget(comboUsername, 0, 1);
    mainLayout->addWidget(labelPassword, 1, 0);
    mainLayout->addWidget(editPassword, 1, 1);
    mainLayout->addWidget(editPassword, 1, 1);
    mainLayout->addWidget(labelError, 2, 0, 1, 2);
    mainLayout->addLayout(buttonsLayout, 3, 0, 1, 2);
}

void LoginDialog::slotTryLogin() {
    enableButton(false);
    spinnerLabel->show();
    QString username = comboUsername->text();
    QString password = editPassword->text();

    emit tryLogin(username, password);
}

void LoginDialog::loginCorrect() {
    close();
}

void LoginDialog::loginFailed() {
    labelError->setText("Cannot login!");
    spinnerLabel->hide();
    enableButton(true);
}

void LoginDialog::enableButton(bool enabled) {
    loginButton->setEnabled(enabled);
    registerButton->setEnabled(enabled);
}
