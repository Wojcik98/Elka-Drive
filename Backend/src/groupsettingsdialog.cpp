#include "include/groupsettingsdialog.h"
#include "include/model.h"

#include <QMessageBox>
#include <QLineEdit>
#include <QInputDialog>

/*!
 * \brief Konstruktor.
 * \param groupName Nazwa grupy
 * \param groupID ID grupy
 * \param parent Rodzic w strukturze Qt
 */
GroupSettingsDialog::GroupSettingsDialog(const QString &groupName, const int groupID, QWidget *parent)
    : QDialog(parent), groupName(groupName), groupId(groupID) {
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    setUpGUI();
    setWindowTitle("Group settings");
    setModal(true);
    usersList->setModel(&usersModel);
}

/*!
 * \brief Destruktor.
 */
GroupSettingsDialog::~GroupSettingsDialog() {
}

void GroupSettingsDialog::setUpGUI() {
    mainLayout = new QGridLayout(this);

    initTopBar();
    initList();
    initAdvanced();
    connectSlots();
    placeComponents();
}

void GroupSettingsDialog::initTopBar() {
    labelUsers = new QLabel("Users: ", this);
    addUserButton = new QPushButton(QIcon(":/icons/add.svg"), "", this);
    removeUserButton = new QPushButton(QIcon(":/icons/remove.svg"), "", this);

    topBarLayout = new QHBoxLayout();
    topBarLayout->addWidget(labelUsers);
    topBarLayout->addWidget(addUserButton);
    topBarLayout->addWidget(removeUserButton);
}

void GroupSettingsDialog::initList() {
    usersList = new QListView(this);
    usersList->setSelectionMode(QAbstractItemView::SingleSelection);
    usersList->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void GroupSettingsDialog::initAdvanced() {
    advancedSettingsCheckbox = new QCheckBox("Advanced settings", this);
    advancedSettingsWidget = new QWidget(this);
    auto advancedLayout = new QHBoxLayout();
    deleteGroupButton = new QPushButton(QIcon(":/icons/delete.svg"), "Delete group", advancedSettingsWidget);

    advancedLayout->addWidget(deleteGroupButton);
    advancedSettingsWidget->setLayout(advancedLayout);
    advancedSettingsWidget->hide();
}

void GroupSettingsDialog::connectSlots() {
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
}

void GroupSettingsDialog::placeComponents() {
    mainLayout->addLayout(topBarLayout, 0, 0, 1, 2);
    mainLayout->addWidget(usersList, 1, 0, 1, 2);
    mainLayout->addWidget(advancedSettingsCheckbox, 2, 0, 1, 2);
    mainLayout->addWidget(advancedSettingsWidget, 3, 0, 1, 2);
}

/*!
 * \brief Wyświetla użytkowników.
 * \param users Lista użytkowników
 */
void GroupSettingsDialog::groupUsersReceived(const QList<User> &users) {
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

/*!
 * \brief Obsługuje sygnał o dodaniu użytkownika do grupy.
 */
void GroupSettingsDialog::groupAddUserReceived() {
    emit requestGroupUsers(groupId);
}

void GroupSettingsDialog::removeUser() {
    int totalUsers = usersModel.rowCount();
    if (totalUsers == 1) {
        QMessageBox msgBox(this);
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

/*!
 * \brief Obsługuje sygnał o usunięciu użytkonika z grupy.
 */
void GroupSettingsDialog::groupRemoveUserReceived() {
    emit requestGroupUsers(groupId);
}

/*!
 * \brief Wyświetla zaawansowane ustawienia.
 * \param state Czy wyświetlać
 */
void GroupSettingsDialog::enableAdvanced(int state) {
    if (state == Qt::Unchecked) {
        advancedSettingsWidget->hide();
    } else if (state == Qt::Checked) {
        advancedSettingsWidget->show();
    }
}

void GroupSettingsDialog::confirmGroupDelete() {
    QMessageBox msgBox(this);
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

/*!
 * \brief Obsługuje sygnał o usunięciu grupy.
 */
void GroupSettingsDialog::groupDeletedReceived() {
    close();
}

void GroupSettingsDialog::permissionDenied() {
    QMessageBox msgBox(this);
    msgBox.setText("You were removed from this group!");
    msgBox.exec();
    close();
}
