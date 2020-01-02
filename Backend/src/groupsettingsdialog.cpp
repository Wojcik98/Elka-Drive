#include "include/groupsettingsdialog.h"
#include "include/model.h"

#include <QMessageBox>
#include <QLineEdit>
#include <QInputDialog>

GroupSettingsDialog::GroupSettingsDialog(QString groupName, int groupID, QWidget *parent) : QDialog(parent), groupName(groupName), groupId(groupID) {
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    setUpGUI();
    setWindowTitle("Group settings");
    setModal(true);
    usersList->setModel(&usersModel);
}

GroupSettingsDialog::~GroupSettingsDialog() {
}

void GroupSettingsDialog::setUpGUI() {
    QGridLayout* formGridLayout = new QGridLayout(this);

    // top bar
    labelUsers = new QLabel("Users: ", this);
    addUserButton = new QPushButton(QIcon(":/icons/add.svg"), "", this);
    removeUserButton = new QPushButton(QIcon(":/icons/remove.svg"), "", this);
    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addWidget(labelUsers);
    hbox->addWidget(addUserButton);
    hbox->addWidget(removeUserButton);

    // list
    usersList = new QListView(this);
    usersList->setSelectionMode(QAbstractItemView::SingleSelection);
    usersList->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // delete
    advancedSettingsCheckbox = new QCheckBox("Advanced settings", this);

    advancedSettingsWidget = new QWidget(this);
    auto advancedLayout = new QHBoxLayout();
    deleteGroupButton = new QPushButton(QIcon(":/icons/delete.svg"), "Delete group", advancedSettingsWidget);
    advancedLayout->addWidget(deleteGroupButton);
    advancedSettingsWidget->setLayout(advancedLayout);
    advancedSettingsWidget->hide();

    // connects slots
    connect(
        advancedSettingsCheckbox,
        &QCheckBox::stateChanged,
        this,
        &GroupSettingsDialog::enableAdvanced
    );
    connect(
        deleteGroupButton,
        &QPushButton::clicked,
        this,
        &GroupSettingsDialog::confirmGroupDelete
    );
    connect(
        addUserButton,
        &QPushButton::clicked,
        this,
        &GroupSettingsDialog::addUser
    );
    connect(
        removeUserButton,
        &QPushButton::clicked,
        this,
        &GroupSettingsDialog::removeUser
    );

    // place components into the dialog
    formGridLayout->addLayout(hbox, 0, 0, 1, 2);
    formGridLayout->addWidget(usersList, 1, 0, 1, 2);
    formGridLayout->addWidget(advancedSettingsCheckbox, 2, 0, 1, 2);
    formGridLayout->addWidget(advancedSettingsWidget, 3, 0, 1, 2);

    setLayout(formGridLayout);
}

void GroupSettingsDialog::groupUsersReceived(QList<User> users, bool forbidden) {
    if (!forbidden) {
        QList<QStandardItem*> column;
        for (User user : users) {
            auto item = new QStandardItem(user.getName());
            item->setData(QVariant(user.getId()), Model::ID_ROLE);
            column.append(item);
        }

        usersModel.clear();
        usersModel.appendColumn(column);

        auto firstIndex = usersModel.index(0, 0);
        usersList->setCurrentIndex(firstIndex);
    } else {
        permissionDenied();
    }
}

void GroupSettingsDialog::addUser() {
    bool ok;

    QString username = QInputDialog::getText(
        this,
        "Add user to group",
        "Username:",
        QLineEdit::Normal,
        "",
        &ok
    );

    if (ok) {
        emit requestAddUserToGroup(username, groupId);
    }
}

void GroupSettingsDialog::groupAddUserReceived(bool success, bool forbidden) {
    if (forbidden) {
        permissionDenied();
    } else {
        if (success) {
            emit requestGroupUsers(groupId);
        } else {
            QMessageBox msgBox;
            msgBox.setText("Could not add user to the group");
            msgBox.setInformativeText("Make sure the username is correct");
            msgBox.exec();
        }
    }
}

void GroupSettingsDialog::removeUser() {
    int totalUsers = usersModel.rowCount();
    if (totalUsers == 1) {
        QMessageBox msgBox;
        msgBox.setText("Cannot remove last user!");
        msgBox.setInformativeText("Perhaps you wanted to delete the group? Check \"Advanced settings\"");
        msgBox.exec();
    } else {
        auto selectionModel = usersList->selectionModel();
        auto list = selectionModel->selectedIndexes();

        if (list.length() > 0) {
            auto index = list.first();
            QString username = index.data(Qt::DisplayRole).toString();
            emit requestRemoveUserFromGroup(username, groupId);
        }
    }
}

void GroupSettingsDialog::groupRemoveUserReceived(bool success, bool forbidden) {
    if (forbidden) {
        permissionDenied();
    } else {
        if (success) {
            emit requestGroupUsers(groupId);
        } else {
            QMessageBox msgBox;
            msgBox.setText("Could not remove user from the group");
            msgBox.exec();
        }
    }
}

void GroupSettingsDialog::enableAdvanced(int state) {
    if (state == Qt::Unchecked) {
        advancedSettingsWidget->hide();
    } else if (state == Qt::Checked) {
        advancedSettingsWidget->show();
    }
}

void GroupSettingsDialog::confirmGroupDelete() {
    QMessageBox msgBox;
    msgBox.setText("Do you want to delete the group?");
    msgBox.setInformativeText("This operation will also permamently delete all files in this group!");
    msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();

    switch (ret) {
        case QMessageBox::Yes:
            emit requestGroupDelete(groupId);
            break;
        case QMessageBox::No:
            break;
    }
}

void GroupSettingsDialog::groupDeletedReceived(bool forbidden) {
    if (forbidden) {
        permissionDenied();
    } else {
        close();
    }
}

void GroupSettingsDialog::permissionDenied() {
    QMessageBox msgBox;
    msgBox.setText("You were removed from this group!");
    msgBox.exec();
    close();
}
