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
    void setButtonsLayouts(QLayout *groupslayout, QLayout *filesLayout);
    void setGroupsButtonsVisible();
    void setFilesButtonsVisible();

protected:
    void showEvent(QShowEvent *ev);

    Controller *controller;
    QWidget groupsWidget;
    QWidget filesWidget;
};

#endif // VIEW_H
