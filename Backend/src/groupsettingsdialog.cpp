#include "include/groupsettingsdialog.h"
#include "include/model.h"

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

    // place components into the dialog
    formGridLayout->addLayout(hbox, 0, 0, 1, 2);
    formGridLayout->addWidget(usersList, 1, 0, 1, 2);
    formGridLayout->addWidget(advancedSettingsCheckbox, 2, 0, 1, 2);
    formGridLayout->addWidget(advancedSettingsWidget, 3, 0, 1, 2);

    setLayout(formGridLayout);
}

void GroupSettingsDialog::groupUsersReceived(QList<User> users) {
    QList<QStandardItem*> column;
    for (User user : users) {
        auto item = new QStandardItem(user.getName());
        item->setData(QVariant(user.getId()), Model::ID_ROLE);
        column.append(item);
    }

    usersModel.clear();
    usersModel.appendColumn(column);
}

void GroupSettingsDialog::enableAdvanced(int state) {
    if (state == Qt::Unchecked) {
        advancedSettingsWidget->hide();
    } else if (state == Qt::Checked) {
        advancedSettingsWidget->show();
    }
}

void GroupSettingsDialog::confirmGroupDelete() {
    // TODO show dialog
    emit requestGroupDelete(groupId);
}
