#include <QStandardItem>
#include <QStringListModel>
#include <QUrl>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->fileList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->fileList->setModel(&fileListModel);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setController(Controller *controller) {
    this->controller = controller;
    QObject::connect(ui->fileList, &QAbstractItemView::doubleClicked, controller, &Controller::fileDoubleClicked);
}

void MainWindow::setFileList(QList<QStandardItem*> files) {
    fileListModel.clear();
    fileListModel.appendColumn(files);
}
