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
    void showEvent(QShowEvent *ev);

    Controller *controller;
    QWidget groupsWidget;
    QWidget filesWidget;
    QPushButton *newButton;
    QPushButton *settingsButton;
    QPushButton *dirButton;
    QPushButton *uploadButton;

signals:
    void createNewGroup();
};

#endif // VIEW_H
