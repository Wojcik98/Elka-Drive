#include "include/logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent) {
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    setUpGUI();
    setWindowTitle(tr("User Login"));
    setModal(true);
}

void LoginDialog::setUpGUI() {
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
    labelError->setStyleSheet("color: red;");

    // initialize buttons
    buttons = new QDialogButtonBox(this);
    buttons->addButton(QDialogButtonBox::Ok);
    buttons->addButton(QDialogButtonBox::Cancel);
    buttons->button(QDialogButtonBox::Ok)->setText(tr("Login"));
    buttons->button(QDialogButtonBox::Cancel)->setText(tr("Close"));

    // connects slots
    connect(
        buttons->button(QDialogButtonBox::Cancel),
        &QPushButton::clicked,
        this,
        &LoginDialog::reject
    );

    connect(
        buttons->button(QDialogButtonBox::Ok),
        SIGNAL(clicked()),
        this,
        SLOT(slotTryLogin())
    );

    // place components into the dialog
    formGridLayout->addWidget(labelUsername, 0, 0);
    formGridLayout->addWidget(comboUsername, 0, 1);
    formGridLayout->addWidget(labelPassword, 1, 0);
    formGridLayout->addWidget(editPassword, 1, 1);
    formGridLayout->addWidget(editPassword, 1, 1);
    formGridLayout->addWidget(labelError, 2, 0, 1, 2);
    formGridLayout->addWidget(buttons, 3, 0, 1, 2);

    setLayout(formGridLayout);

}

void LoginDialog::setUsername(QString &username) {
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

void LoginDialog::setPassword(QString &password) {
    editPassword->setText(password);
}

void LoginDialog::slotTryLogin() {
    QString username = comboUsername->currentText();
    QString password = editPassword->text();

    emit tryLogin(
        username,
        password
    );
}

void LoginDialog::slotLoginResponse(bool success) {
    if (success) {
        close();
    } else {
        labelError->setText("Cannot login!");
    }
}

void LoginDialog::setUsernamesList(const QStringList &usernames) {
    comboUsername->addItems(usernames);
}
