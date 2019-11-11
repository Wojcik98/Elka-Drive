#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "controller.h"
#include "model.h"

class Controller;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setController(Controller *controller);
    void setFileList(QStringList files);

private:
    Ui::MainWindow *ui;
    QAbstractItemModel *fileListModel;
    Controller *controller;
};

#endif // MAINWINDOW_H
