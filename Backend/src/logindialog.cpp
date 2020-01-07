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
    QGridLayout* formGridLayout = new QGridLayout(this);

    comboUsername = new QLineEdit(this);
    editPassword = new QLineEdit(this);
    editPassword->setEchoMode(QLineEdit::Password);

    // initialize the labels
    labelUsername = new QLabel(this);
    labelPassword = new QLabel(this);
    labelError = new QLabel(this);
    labelUsername->setText(tr("Username"));
    labelUsername->setBuddy(comboUsername);
    labelPassword->setText(tr("Password"));
    labelPassword->setBuddy(editPassword);
    labelError->setText("");
    labelError->setStyleSheet("color: red;");

    // initialize buttons
    loginButton = new QPushButton(QIcon(":/icons/login.svg"), "Login", this);
    registerButton = new QPushButton(QIcon(":/icons/register.svg"), "Register", this);
    cancelButton = new QPushButton(QIcon(":/icons/cancel.svg"), "Close", this);
    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addWidget(cancelButton);
    hbox->addWidget(registerButton);
    hbox->addWidget(loginButton);

    // spinner
    spinnerLabel = new QLabel(this);
    spinnerMovie = new QMovie(":/gifs/spinner.gif");
    spinnerMovie->start();
    spinnerLabel->setMovie(spinnerMovie);
    hbox->addWidget(spinnerLabel);

    // connects slots
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

    // place components into the dialog
    formGridLayout->addWidget(labelUsername, 0, 0);
    formGridLayout->addWidget(comboUsername, 0, 1);
    formGridLayout->addWidget(labelPassword, 1, 0);
    formGridLayout->addWidget(editPassword, 1, 1);
    formGridLayout->addWidget(editPassword, 1, 1);
    formGridLayout->addWidget(labelError, 2, 0, 1, 2);
    formGridLayout->addLayout(hbox, 3, 0, 1, 2);

    setLayout(formGridLayout);
    spinnerLabel->hide();
}

void LoginDialog::slotTryLogin() {
    spinnerLabel->show();
    QString username = comboUsername->text();
    QString password = editPassword->text();

    emit tryLogin(
        username,
        password
    );
}

void LoginDialog::slotLoginResponse() {
    close();
}

void LoginDialog::loginFailed() {
    labelError->setText("Cannot login!");
    spinnerLabel->hide();
}
