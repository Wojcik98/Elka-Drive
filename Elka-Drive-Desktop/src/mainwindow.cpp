#include <QStandardItem>
#include <QStringListModel>
#include <QUrl>
#include <algorithm>

#include "include/messagedelegate.h"
#include "include/mainwindow.h"
#include "include/view.h"
#include "include/controller.h"
#include "include/model.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : View(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->fileList->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->fileList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->fileList->setModel(&fileListModel);

    ui->fileList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(
        ui->fileList, &QListView::customContextMenuRequested,
        this, &MainWindow::handleContextMenuRequested
    );

    ui->buttonsLayout->addWidget(&groupsWidget, 0, 0);
    ui->buttonsLayout->addWidget(&filesWidget, 0, 0);

    auto uploadLabel = new QLabel("Upload:", this);
    auto downloadLabel = new QLabel("Download:", this);
    ui->statusBar->addPermanentWidget(uploadLabel, 1);
    ui->statusBar->addPermanentWidget(uploadWidget, 3);
    ui->statusBar->addPermanentWidget(downloadLabel, 1);
    ui->statusBar->addPermanentWidget(downloadWidget, 3);

    ui->backButton->setIcon(QIcon(":/icons/arrow_back.svg"));
    ui->refreshButton->setIcon(QIcon(":/icons/refresh.svg"));
    ui->sendButton->setIcon(QIcon(":/icons/send.svg"));
    sendButton = ui->sendButton;
    backButton = ui->backButton;

    ui->messagesList->setItemDelegate(new MessageDelegate(this));
    ui->messagesList->setSelectionMode(QAbstractItemView::NoSelection);
    ui->messagesList->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(
        ui->messageEdit,
        &QLineEdit::returnPressed,
        sendButton,
        &QPushButton::click
    );
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::handleContextMenuRequested(const QPoint &pos) {
    QModelIndex index = ui->fileList->indexAt(pos);
    auto type = index.data(Model::TYPE_ROLE);
    if (type == Model::ItemType::GROUP) {
        return;
    }

    QMenu *menu = new QMenu(this);

    auto download = new QAction(QIcon(":/icons/cloud_download.svg"), "Download", this);
    connect(download, &QAction::triggered, controller, [this, index]() {
         emit requestDownload(index);
    });
    menu->addAction(download);

    auto del = new QAction(QIcon(":/icons/delete.svg"), "Delete", this);
    connect(del, &QAction::triggered, controller, [this, index]() {
        emit requestDelete(index);
    });
    menu->addAction(del);

    menu->popup(ui->fileList->viewport()->mapToGlobal(pos));
}

void MainWindow::settingsButtonClicked() {
    auto selectionModel = ui->fileList->selectionModel();
    auto list = selectionModel->selectedIndexes();

    if (list.length() > 0) {
        auto index = list.first();
        emit openGroupSettings(index);
    }
}

void MainWindow::downloadButtonClicked() {
    auto selectionModel = ui->fileList->selectionModel();
    auto list = selectionModel->selectedIndexes();

    if (list.length() > 0) {
        auto index = list.first();
        emit requestDownload(index);
    }
}

void MainWindow::deleteButtonClicked() {
    auto selectionModel = ui->fileList->selectionModel();
    auto list = selectionModel->selectedIndexes();

    if (list.length() > 0) {
        auto index = list.first();
        emit requestDelete(index);
    }
}

void MainWindow::setController(Controller *newController) {
    controller = newController;
    connect(
        ui->fileList,
        &QAbstractItemView::doubleClicked,
        controller,
        &Controller::fileDoubleClicked
    );
    connect(
        ui->fileList,
        &QAbstractItemView::clicked,
        controller,
        &Controller::fileClicked
    );
    connect(
        ui->backButton,
        &QPushButton::clicked,
        controller,
        &Controller::goBack
    );
    connect(
        ui->refreshButton,
        &QPushButton::clicked,
        controller,
        &Controller::refresh
    );
    connect(
        ui->sendButton,
        &QPushButton::clicked,
        this,
        &MainWindow::sendButtonClicked
    );
}

bool MainWindow::filesSort(QStandardItem *a, QStandardItem *b) {
    return a->data(Model::TYPE_ROLE).toInt() < b->data(Model::TYPE_ROLE).toInt();
}

void MainWindow::setFileList(const QList<QStandardItem*> &files, const QString &path) {
    fileListModel.clear();

    QList<QStandardItem*> copy(files);
    std::sort(copy.begin(), copy.end(), filesSort);
    fileListModel.appendColumn(copy);

    ui->pathLabel->setText("Location: " + path);
}

void MainWindow::sendButtonClicked() {
    auto msg = ui->messageEdit->text();

    if (!msg.trimmed().isEmpty()) {
        emit sendMsg(msg);
    }
}

void MainWindow::clearMsg() {
    ui->messageEdit->clear();
}

void MainWindow::setChatModel(QStandardItemModel *model) {
    if (ui->messagesList->model() != nullptr) {
        disconnect(
            ui->messagesList->model(), nullptr,
            this, nullptr
        );
    }

    ui->messagesList->setModel(model);

    if (ui->messagesList->model() != nullptr) {
        connect(
            ui->messagesList->model(), &QStandardItemModel::rowsInserted,
            this, &MainWindow::chatModelRowInserted
        );
    }
}

void MainWindow::chatModelRowInserted(const QModelIndex&, int, int) {
    auto model = ui->messagesList->model();
    auto lastItem = model->index(model->rowCount() - 1, 0);
    ui->messagesList->scrollTo(lastItem);
}
