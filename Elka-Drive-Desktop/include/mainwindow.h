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

/*!
 * \brief MainWindow wyświetla główne okno aplikacji.
 */
class MainWindow : public View {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void setController(Controller *controller) override;
    void setFileList(const QList<QStandardItem*> &files, const QString &path) override;
    void clearMsg() override;
    void setChatModel(QStandardItemModel *model) override;
    void settingsButtonClicked() override;
    void downloadButtonClicked() override;
    void deleteButtonClicked() override;
    void showLogoutMsg() override;
    void showForbiddenMsg() override;
    void showDirectoryNotFound() override;
    void showGroupExist() override;
    void showUnknownErrorMsg() override;
    void showFileOpenError(const QString &filename) override;
    QString getNewGroupName(bool *ok) override;
    QString getNewFolderName(bool *ok) override;
    QStringList getUploadFileNames() override;
    QString getUploadFolderName() override;
    QString getSaveFilename(const QString &suggested) override;

private:
    Ui::MainWindow *ui;
    void showEvent(QShowEvent *ev) override;

    static bool filesSort(QStandardItem *a, QStandardItem *b);
    void sendButtonClicked();
    void handleContextMenuRequested(const QPoint &pos);

private slots:
    void chatModelRowInserted(const QModelIndex&, int, int);
};

#endif // MAINWINDOW_H
