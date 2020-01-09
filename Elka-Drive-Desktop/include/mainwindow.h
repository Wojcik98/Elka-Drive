#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "include/controller.h"
#include "include/view.h"

class Controller;
class View;

namespace Ui {
class MainWindow;
}

class MainWindow : public View {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void setController(Controller *controller) override;
    void setFileList(QList<QStandardItem*> files) override;
    void clearMsg() override;

private:
    Ui::MainWindow *ui;

    void settingsButtonClicked() override;
    static bool filesSort(QStandardItem *a, QStandardItem *b);
    void sendButtonClicked();
};

#endif // MAINWINDOW_H
