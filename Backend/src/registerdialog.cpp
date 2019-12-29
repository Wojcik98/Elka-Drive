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

    comboUsername = new QComboBox(this);
    comboUsername->setEditable(true);
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
    formGridLayout->addWidget(comboUsername, 0, 1, 1, 2);
    formGridLayout->addWidget(labelPassword, 1, 0);
    formGridLayout->addWidget(editPassword, 1, 1);
    formGridLayout->addWidget(editPassword, 1, 1, 1, 2);
    formGridLayout->addWidget(labelError, 2, 0, 1, 2);
    formGridLayout->addLayout(hbox, 3, 0, 1, 2);

    setLayout(formGridLayout);
    spinnerLabel->hide();
}

void RegisterDialog::setUsername(QString &username) {
    bool found = false;
    for (int i = 0; i < comboUsername->count() && !found; i++) {
        if (comboUsername->itemText(i) == username) {
            comboUsername->setCurrentIndex(i);
            found = true;
        }
    }

    if (!found) {
        int index = comboUsername->count();
        comboUsername->addItem(username);

        comboUsername->setCurrentIndex(index);
    }

    // place the focus on the password field
    editPassword->setFocus();
}

void RegisterDialog::setPassword(QString &password) {
    editPassword->setText(password);
}

void RegisterDialog::slotTryRegister() {
    spinnerLabel->show();
    registerButton->setEnabled(false);
    QString username = comboUsername->currentText();
    QString password = editPassword->text();

    emit tryLogin(
        username,
        password
    );
}

void RegisterDialog::slotRegisterResponse(bool success) {
    if (success) {
        labelError->setStyleSheet("color: green;");
        labelError->setText("Register successful!");
    } else {
        labelError->setStyleSheet("color: red;");
        labelError->setText("User exists!");
    }
    registerButton->setEnabled(true);
    spinnerLabel->hide();
}

void RegisterDialog::setUsernamesList(const QStringList &usernames) {
    comboUsername->addItems(usernames);
}
