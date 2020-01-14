#include <QStandardItem>
#include <QStringListModel>
#include <QUrl>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <algorithm>

#include "include/messagedelegate.h"
#include "include/mainwindow.h"
#include "include/view.h"
#include "include/controller.h"
#include "include/model.h"
#include "ui_mainwindow.h"

/*!
 * \brief Konstruktor.
 * \param parent Rodzic w strukturze Qt
 */
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

/*!
 * \brief Destruktor.
 */
MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::showEvent(QShowEvent *ev) {
    QMainWindow::showEvent(ev);
    controller->tryLogin();
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

/*!
 * \brief Obsługuje naciśnięcie przycisku ustawień grupy.
 */
void MainWindow::settingsButtonClicked() {
    auto selectionModel = ui->fileList->selectionModel();
    auto list = selectionModel->selectedIndexes();

    if (list.length() > 0) {
        auto index = list.first();
        emit openGroupSettings(index);
    }
}

/*!
 * \brief Obsługuje naciśnięcie przycisku pobierania.
 */
void MainWindow::downloadButtonClicked() {
    auto selectionModel = ui->fileList->selectionModel();
    auto list = selectionModel->selectedIndexes();

    if (list.length() > 0) {
        auto index = list.first();
        emit requestDownload(index);
    }
}

/*!
 * \brief Obsługuje naciśnięcie przycisku usuwania.
 */
void MainWindow::deleteButtonClicked() {
    auto selectionModel = ui->fileList->selectionModel();
    auto list = selectionModel->selectedIndexes();

    if (list.length() > 0) {
        auto index = list.first();
        emit requestDelete(index);
    }
}

/*!
 * \brief Ustawia kontroler i łączy sygnały.
 * \param newController Kontroler
 */
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
        &Controller::fileSelected
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

/*!
 * \brief Wyświetla ścieżkę i jej zawartość.
 * \param files Zawartość ścieżki
 * \param path Ścieżka
 */
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

/*!
 * \brief Czyści linię edycji wiadomości.
 */
void MainWindow::clearMsg() {
    ui->messageEdit->clear();
}

/*!
 * \brief Ustawia nowy model wiadomości w czacie.
 * \param model Nowy model
 */
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

/*!
 * \brief Wyświetla komunikat o wylogowaniu.
 */
void MainWindow::showLogoutMsg() {
    QMessageBox msgBox(this);
    msgBox.setText("You have been logged out!");
    msgBox.exec();
}

/*!
 * \brief Wyświetla komunikat o braku dostępu do grupy.
 */
void MainWindow::showForbiddenMsg() {
    QMessageBox msgBox(this);
    msgBox.setText("You don't have access to this group!");
    msgBox.exec();
}

/*!
 * \brief Wyświetla komunikat o przebywaniu w nieistniejącym folderze.
 */
void MainWindow::showDirectoryNotFound() {
    QMessageBox msgBox(this);
    msgBox.setText("Directory does not exist!");
    msgBox.setInformativeText("It could be deleted. You will be moved up in directory structure.");
    msgBox.exec();
}

/*!
 * \brief Wyświetla komunikat, że żądana nazwa grupy już istnieje.
 */
void MainWindow::showGroupExist() {
    QMessageBox msgBox(this);
    msgBox.setText("Group with this name already exists!");
    msgBox.setInformativeText("Choose another name");
    msgBox.exec();
}

/*!
 * \brief Wyświetla komunikat o nieznanym błędzie.
 */
void MainWindow::showUnknownErrorMsg() {
    QMessageBox msgBox(this);
    msgBox.setText("Error while connecting to the server!");
    msgBox.setInformativeText("Try again. If error persists contact developer.");
    msgBox.exec();
}

/*!
 * \brief Wyświetla komunikat o błędzie otwierania pliku.
 * \param filename Nazwa pliku
 */
void MainWindow::showFileOpenError(const QString &filename) {
    QMessageBox msgBox(this);
    msgBox.setText("Could not open file!");
    msgBox.setInformativeText(filename);
    msgBox.exec();
}

/*!
 * \brief Wyświetla okno do wpisania nazwy nowej grupy.
 * \param ok Parametr, w którym zostanie zapisane powodzenie operacji
 * \return Nazwa nowej grupy
 */
QString MainWindow::getNewGroupName(bool *ok) {
    return QInputDialog::getText(
        this, "Create new group", "Group name:",
        QLineEdit::Normal, "", ok
    );
}

/*!
 * \brief Wyświetla okno do wpisania nazwy nowego folderu.
 * \param ok Parametr, w którym zostanie zapisane powodzenie operacji
 * \return Nazwa nowego folderu
 */
QString MainWindow::getNewFolderName(bool *ok) {
    return QInputDialog::getText(
        this, "New folder", "Name:",
        QLineEdit::Normal, "", ok
    );
}

/*!
 * \brief Wyświetla okno do wybrania plików do wysłania.
 * \return Lista ścieżek do plików
 */
QStringList MainWindow::getUploadFileNames() {
    return QFileDialog::getOpenFileNames(
        this, "Select one or more files to upload", QDir::homePath()
    );
}

/*!
 * \brief Wyświetla okno do wybrania folderu do wysłania.
 * \return Ścieżka folderu
 */
QString MainWindow::getUploadFolderName() {
    return QFileDialog::getExistingDirectory(
        this, "Select directory to upload", QDir::homePath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );
}

/*!
 * \brief Wyświetla okno do wybrania nazwy, gdzie zostanie zapisany plik.
 * \param suggested Sugerowana nazwa pliku
 * \return Ścieżka pliku
 */
QString MainWindow::getSaveFilename(const QString &suggested) {
    return QFileDialog::getSaveFileName(this, "Save file", suggested);
}
