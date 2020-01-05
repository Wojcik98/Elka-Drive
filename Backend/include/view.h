#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include <QStandardItem>
#include <QLayout>

#include "include/controller.h"

class Controller;

class View : public QMainWindow {
    Q_OBJECT

public:
    explicit View(QWidget *parent = nullptr);
    virtual ~View();

    virtual void setController(Controller *controller) = 0;
    virtual void setFileList(QList<QStandardItem*> files) = 0;
    void setGroupsButtonsVisible();
    void setFilesButtonsVisible();
    void setSettingsButtonEnabled(bool enabled);

protected:
    virtual void settingsButtonClicked() = 0;
    void showEvent(QShowEvent *ev);

    QStandardItemModel fileListModel;
    Controller *controller;
    QWidget groupsWidget;
    QWidget filesWidget;
    QPushButton *newButton;
    QPushButton *settingsButton;
    QPushButton *dirButton;
    QPushButton *uploadFileButton;
    QPushButton *uploadFolderButton;

signals:
    void createNewGroup();
    void openGroupSettings(QModelIndex index);
    void createNewFolder();
    void uploadFile();
    void uploadFolder();
};

#endif // VIEW_H
