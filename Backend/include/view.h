#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include <QStandardItem>
#include <QLayout>
#include <QProgressBar>
#include <QTimer>

#include "include/controller.h"

class Controller;

/*!
 * \brief View Interfejs widoku z wzorca MVC.
 */
class View : public QMainWindow {
    Q_OBJECT

public:
    explicit View(QWidget *parent = nullptr);
    virtual ~View();

    virtual void setController(Controller *controller) = 0;
    virtual void setFileList(const QList<QStandardItem*> &files, const QString &path) = 0;
    virtual void clearMsg() = 0;
    virtual void setChatModel(QStandardItemModel *model) = 0;
    virtual void showLogoutMsg() = 0;
    virtual void showForbiddenMsg() = 0;
    virtual void showDirectoryNotFound() = 0;
    virtual void showGroupExist() = 0;
    virtual void showUnknownErrorMsg() = 0;
    virtual void showFileOpenError(const QString &filename) = 0;
    virtual QString getNewGroupName(bool *ok) = 0;
    virtual QString getNewFolderName(bool *ok) = 0;
    virtual QStringList getUploadFileNames() = 0;
    virtual QString getUploadFolderName() = 0;
    virtual QString getSaveFilename(const QString &suggested) = 0;
    void setGroupsView();
    void setFilesView();
    void anyItemSelected(bool enabled);

protected:
    virtual void settingsButtonClicked() = 0;
    virtual void downloadButtonClicked() = 0;
    virtual void deleteButtonClicked() = 0;

    QStandardItemModel fileListModel;
    Controller *controller;
    QWidget groupsWidget;
    QWidget filesWidget;
    QPushButton *newButton;
    QPushButton *settingsButton;
    QPushButton *dirButton;
    QPushButton *uploadFileButton;
    QPushButton *uploadFolderButton;
    QPushButton *downloadButton;
    QPushButton *deleteButton;
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
    /*!
     * \brief Żądanie stworzenia nowej grupy.
     */
    void createNewGroup();

    /*!
     * \brief Żądanie otworzenia ustawień grupy.
     * \param index Informacje o grupie
     */
    void openGroupSettings(const QModelIndex &index);

    /*!
     * \brief Żądanie pobrania obiektu.
     * \param index Informacje o obiekcie
     */
    void requestDownload(const QModelIndex &index);

    /*!
     * \brief Żądanie usunięcia obiektu.
     * \param index Informacje o obiekcie
     */
    void requestDelete(const QModelIndex &index);

    /*!
     * \brief Żądanie stworzenia nowego folderu.
     */
    void createNewFolder();

    /*!
     * \brief Żądanie wysłania pliku.
     */
    void uploadFile();

    /*!
     * \brief Żądanie wysłania folderu.
     */
    void uploadFolder();

    /*!
     * \brief Żądanie wysłania wiadomości.
     * \param msg Tekst wiadomości do wysłania
     */
    void sendMsg(const QString &msg);

public slots:
    void setUploadProgress(const qint64 current, const qint64 total);
    void setDownloadProgress(const qint64 current, const qint64 total);
    void resetUploadProgress();
    void resetDownloadProgress();
};

#endif // VIEW_H
