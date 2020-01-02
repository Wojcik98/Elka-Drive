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
    explicit GroupSettingsDialog(QString groupName, int groupID, QWidget *parent=nullptr);
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

    QString groupName;
    int groupId;

    void setUpGUI();
    void confirmGroupDelete();
    void addUser();
    void removeUser();
    void permissionDenied();

signals:
    void requestGroupUsers(int groupId);
    void requestAddUserToGroup(QString username, int groupId);
    void requestRemoveUserFromGroup(QString username, int groupId);
    void requestGroupDelete(int groupId);
    void openRegister();

public slots:
    void enableAdvanced(int state);
    void groupUsersReceived(QList<User> users, bool forbidden);
    void groupDeletedReceived(bool forbidden);
    void groupAddUserReceived(bool success, bool forbidden);
    void groupRemoveUserReceived(bool success, bool forbidden);
};

#endif // GROUPSETTINGSDIALOG_H
