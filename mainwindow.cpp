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
    connect(ui->fileList, &QAbstractItemView::doubleClicked, controller, &Controller::fileDoubleClicked);
    connect(ui->backButton, &QPushButton::clicked, controller, &Controller::goBack);
}

void MainWindow::setFileList(QList<QStandardItem*> files) {
    fileListModel.clear();
    fileListModel.appendColumn(files);
}
