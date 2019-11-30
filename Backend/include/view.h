#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include <QStandardItem>

#include "include/controller.h"

class Controller;

class View : public QMainWindow {
    Q_OBJECT

public:
    explicit View(QWidget *parent = nullptr);
    virtual ~View();

    virtual void setController(Controller *controller) = 0;
    virtual void setFileList(QList<QStandardItem*> files) = 0;
};

#endif // VIEW_H
