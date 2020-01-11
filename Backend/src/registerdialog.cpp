#include "include/registerdialog.h"

RegisterDialog::RegisterDialog(QWidget *parent) : QDialog(parent) {
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    setUpGUI();
    setWindowTitle(tr("New account"));
    setModal(true);
}

RegisterDialog::~RegisterDialog() {
    spinnerMovie->deleteLater();
}

void RegisterDialog::setUpGUI() {
    QGridLayout* formGridLayout = new QGridLayout(this);

    editUsername = new QLineEdit(this);
    editPassword = new QLineEdit(this);
    editPassword->setEchoMode(QLineEdit::Password);

    // initialize the labels
    labelUsername = new QLabel(this);
    labelPassword = new QLabel(this);
    labelError = new QLabel(this);
    labelUsername->setText(tr("Username"));
    labelUsername->setBuddy(editUsername);
    labelPassword->setText(tr("Password"));
    labelPassword->setBuddy(editPassword);
    labelError->setText("");

    // initialize buttons
    registerButton = new QPushButton(QIcon(":/icons/register.svg"), "Register", this);
    cancelButton = new QPushButton(QIcon(":/icons/cancel.svg"), "Cancel", this);
    QHBoxLayout *hbox = new QHBoxLayout(this);
    hbox->addWidget(cancelButton);
    hbox->addWidget(registerButton);

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
        &RegisterDialog::reject
    );
    connect(
        registerButton,
        &QPushButton::clicked,
        this,
        &RegisterDialog::slotTryRegister
    );

    // place components into the dialog
    formGridLayout->addWidget(labelUsername, 0, 0);
    formGridLayout->addWidget(editUsername, 0, 1, 1, 2);
    formGridLayout->addWidget(labelPassword, 1, 0);
    formGridLayout->addWidget(editPassword, 1, 1);
    formGridLayout->addWidget(editPassword, 1, 1, 1, 2);
    formGridLayout->addWidget(labelError, 2, 0, 1, 2);
    formGridLayout->addLayout(hbox, 3, 0, 1, 2);

    setLayout(formGridLayout);
    spinnerLabel->hide();
}

void RegisterDialog::slotTryRegister() {
    spinnerLabel->show();
    enableButton(false);
    QString username = editUsername->text();
    QString password = editPassword->text();

    emit tryRegister(username, password);
}

void RegisterDialog::slotRegisterResponse() {
    labelError->setStyleSheet("color: green;");
    labelError->setText("Register successful!");

    enableButton(true);
    spinnerLabel->hide();
}

void RegisterDialog::userExists() {
    labelError->setStyleSheet("color: red;");
    labelError->setText("User exists!");

    enableButton(true);
    spinnerLabel->hide();

}

void RegisterDialog::enableButton(bool enabled) {
    registerButton->setEnabled(enabled);
}
