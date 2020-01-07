#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include <QStandardItem>
#include <QLayout>
#include <QProgressBar>

#include "include/controller.h"

class Controller;

class View : public QMainWindow {
    Q_OBJECT

public:
    explicit View(QWidget *parent = nullptr);
    virtual ~View();

    virtual void setController(Controller *controller) = 0;
    virtual void setFileList(QList<QStandardItem*> files) = 0;
    void setGroupsButtonsVisible();
    void setFilesButtonsVisible();
    void setSettingsButtonEnabled(bool enabled);

protected:
    virtual void settingsButtonClicked() = 0;
    void showEvent(QShowEvent *ev);

    QStandardItemModel fileListModel;
    Controller *controller;
    QWidget groupsWidget;
    QWidget filesWidget;
    QPushButton *newButton;
    QPushButton *settingsButton;
    QPushButton *dirButton;
    QPushButton *uploadFileButton;
    QPushButton *uploadFolderButton;
    QProgressBar *uploadProgress;
    QProgressBar *downloadProgress;

signals:
    void createNewGroup();
    void openGroupSettings(QModelIndex index);
    void createNewFolder();
    void uploadFile();
    void uploadFolder();

public slots:
    void setUploadProgress(qint64 current, qint64 total);
    void setDownloadProgress(qint64 current, qint64 total);
};

#endif // VIEW_H
