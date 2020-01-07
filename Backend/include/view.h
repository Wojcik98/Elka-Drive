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
    virtual void clearMsg() = 0;
    void setGroupsView();
    void setFilesView();
    void setSettingsButtonEnabled(bool enabled);
    void showLogoutMsg();
    void showForbiddenMsg();
    void showUnknownErrorMsg();
    void showFileOpenError(const QString &filename);
    QString getNewGroupName(bool *ok);
    QString getNewFolderName(bool *ok);
    QStringList getUploadFileNames();
    QString getUploadFolderName();
    QString getSaveFilename(QString suggested);

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
    QPushButton *sendButton;

    QWidget *uploadWidget;
    QProgressBar *uploadProgress;
    QLabel *uploadFinishedLabel;
    QWidget *downloadWidget;
    QProgressBar *downloadProgress;
    QLabel *downloadFinishedLabel;

signals:
    void createNewGroup();
    void openGroupSettings(QModelIndex index);
    void createNewFolder();
    void uploadFile();
    void uploadFolder();
    void sendMsg(const QString &msg);

public slots:
    void setUploadProgress(qint64 current, qint64 total);
    void setDownloadProgress(qint64 current, qint64 total);
};

#endif // VIEW_H
