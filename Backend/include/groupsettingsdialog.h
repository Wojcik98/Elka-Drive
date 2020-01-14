#ifndef GROUPSETTINGSDIALOG_H
#define GROUPSETTINGSDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QGroupBox>
#include <QListView>
#include <QGridLayout>
#include <QDebug>
#include <QStandardItemModel>

#include "include/user.h"

class GroupSettingsDialog: public QDialog {
 Q_OBJECT

public:
    explicit GroupSettingsDialog(const QString &groupName, const int groupID, QWidget *parent=nullptr);
    ~GroupSettingsDialog();

private:
    QLabel *labelUsers;
    QPushButton *addUserButton;
    QPushButton *removeUserButton;
    QStandardItemModel usersModel;
    QListView *usersList;
    QCheckBox *advancedSettingsCheckbox;
    QWidget *advancedSettingsWidget;
    QPushButton *deleteGroupButton;
    QHBoxLayout *topBarLayout;
    QGridLayout* mainLayout;

    QString groupName;
    int groupId;

    void setUpGUI();
    void initTopBar();
    void initList();
    void initAdvanced();
    void connectSlots();
    void placeComponents();

    void confirmGroupDelete();
    void addUser();
    void removeUser();
    void permissionDenied();

signals:
    void requestGroupUsers(const int groupId);
    void requestAddUserToGroup(const QString &username, const int groupId);
    void requestRemoveUserFromGroup(const QString &username, const int groupId);
    void requestGroupDelete(int groupId);
    void openRegister();

public slots:
    void enableAdvanced(const int state);
    void groupUsersReceived(const QList<User> &users);
    void groupDeletedReceived();
    void groupAddUserReceived();
    void groupRemoveUserReceived();
};

#endif // GROUPSETTINGSDIALOG_H
