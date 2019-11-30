#include <QStandardItem>
#include <QStringListModel>
#include <QUrl>

#include "include/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->fileList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->fileList->setModel(&fileListModel);

    ui->fileList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->fileList, &QListView::customContextMenuRequested, [this](QPoint pos) {
        QModelIndex *index = new QModelIndex(ui->fileList->indexAt(pos));

        QMenu *menu = new QMenu(this);

        auto download = new QAction(QIcon(":/icons/save.png"), "Download", this);
        connect(download, &QAction::triggered, controller, [this, index]() {
            controller->requestDownload(*index);
            delete index;
        });
        menu->addAction(download);

        auto del = new QAction(QIcon(":/icons/delete.png"), "Delete", this);
        connect(del, &QAction::triggered, controller, [this, index]() {
            controller->requestDelete(*index);
            delete index;
        });
        menu->addAction(del);

        menu->popup(ui->fileList->viewport()->mapToGlobal(pos));
    });
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
