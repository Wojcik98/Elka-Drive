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

/*!
 * \brief GroupSettingsDialog wyświetla i pozwala na zmianę ustawień grupy.
 */
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
    /*!
     * \brief Żądanie listy użytkonwików w grupie.
     * \param groupId ID grupy
     */
    void requestGroupUsers(const int groupId);

    /*!
     * \brief Żądanie dodania użytkownika do grupy.
     * \param username Nazwa użytkownika
     * \param groupId ID grupy
     */
    void requestAddUserToGroup(const QString &username, const int groupId);

    /*!
     * \brief Żądanie usunięcia użytkownika z grupy.
     * \param username Nazwa użytkownika
     * \param groupId ID grupy
     */
    void requestRemoveUserFromGroup(const QString &username, const int groupId);

    /*!
     * \brief Żądanie usunięcia grupy.
     * \param groupId ID grupy
     */
    void requestGroupDelete(int groupId);

public slots:
    void enableAdvanced(const int state);
    void groupUsersReceived(const QList<User> &users);
    void groupDeletedReceived();
    void groupAddUserReceived();
    void groupRemoveUserReceived();
};

#endif // GROUPSETTINGSDIALOG_H
