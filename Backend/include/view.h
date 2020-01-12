#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include <QStandardItem>
#include <QLayout>
#include <QProgressBar>
#include <QTimer>

#include "include/controller.h"

class Controller;

class View : public QMainWindow {
    Q_OBJECT

public:
    explicit View(QWidget *parent = nullptr);
    virtual ~View();

    virtual void setController(Controller *controller) = 0;
    virtual void setFileList(const QList<QStandardItem*> &files, const QString &path) = 0;
    virtual void clearMsg() = 0;
    virtual void setChatModel(QStandardItemModel *model) = 0;
    void setGroupsView();
    void setFilesView();
    void setSettingsButtonEnabled(bool enabled);
    void showLogoutMsg();
    void showForbiddenMsg();
    void showDirectoryNotFound();
    void showUnknownErrorMsg();
    void showFileOpenError(const QString &filename);
    QString getNewGroupName(bool *ok);
    QString getNewFolderName(bool *ok);
    QStringList getUploadFileNames();
    QString getUploadFolderName();
    QString getSaveFilename(const QString &suggested);

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
    QPushButton *backButton;

    QWidget *uploadWidget;
    QProgressBar *uploadProgress;
    QLabel *uploadFinishedLabel;
    QTimer uploadTimer;
    QWidget *downloadWidget;
    QProgressBar *downloadProgress;
    QLabel *downloadFinishedLabel;
    QTimer downloadTimer;

private:
    void setupBottomButtons();
    void setupUpload();
    void setupDownload();
    void connectSignals();

signals:
    void createNewGroup();
    void openGroupSettings(const QModelIndex &index);
    void createNewFolder();
    void uploadFile();
    void uploadFolder();
    void sendMsg(const QString &msg);

public slots:
    void setUploadProgress(const qint64 current, const qint64 total);
    void setDownloadProgress(const qint64 current, const qint64 total);
    void resetUploadProgress();
    void resetDownloadProgress();
};

#endif // VIEW_H
