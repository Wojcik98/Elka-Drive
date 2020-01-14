#ifndef MODEL_H
#define MODEL_H

#include <QStandardItemModel>
#include <QStandardItem>
#include "include/apibridge.h"
#include "include/messagereceiver.h"
#include "include/user.h"
#include "include/response.h"
#include "include/message.h"

class APIBridge;
class Response;

/*!
 * \brief Model Model z wzorca MVC.
 */
class Model : public QObject {
    Q_OBJECT

public:
    /*!
     * \brief Typy wyświetlanych obiektów
     */
    enum ItemType {
        GROUP,  /*!< Grupa */
        FOLDER, /*!< Folder */
        FILE    /*!< Plik */
    };

    Model(APIBridge *bridge, MessageReceiver *receiver);

    bool isLogged();
    void setLogged(bool logged);
    void clearPath();
    void clearMsgs();

    void requestDelete(const QModelIndex &index);
    void requestDownload(const QModelIndex &index, const QString &path);
    void requestSubpath(const QModelIndex &index);
    void requestGroups();
    void requestNewGroup(const QString &groupName);
    void requestGroupUsers(const int groupId);
    void requestNewFolder(const QString &name);
    void requestFileUpload(const QString &rootLocal, const QString &relativePath);
    void goBack();
    void refresh();
    void sendMsg(const QString &msg);
    QStandardItemModel *getCurrentGroupMessages();
    QString getPath();

    static const int TYPE_ROLE = Qt::UserRole + 1;
    static const int ID_ROLE = Qt::UserRole + 2;

private:
    static const int STATUS_UNAUTHORIZED = 401;
    static const int STATUS_FORBIDDEN = 403;
    static const int STATUS_NOT_FOUND = 404;
    static const int STATUS_FOUND = 302;
    static const int STATUS_OK = 200;

    APIBridge *bridge;
    MessageReceiver *receiver;
    QStringList path;
    bool logged = false;
    QString usernameTrying;
    QString username;
    QMap<int, QStandardItemModel*> messages;
    QMap<int, QString> groupIdToName;
    bool pathRequestInProgress = false;

    void handleLoginResponse(const Response &response);
    void handleRegisterResponse(const Response &response);
    void handleGroupsResponse(const Response &response);
    void handleNewGroupResponse(const Response &response);
    void handlePathResponse(const Response &response);
    void handleGroupUsersResponse(const Response &response);
    void handleGroupDeleteResponse(const Response &response);
    void handleGroupAddUserResponse(const Response &response);
    void handleGroupRemoveUserResponse(const Response &response);
    void handleDeleteResponse(const Response &response);
    void handleNewFolderResponse(const Response &response);
    void handleUploadResponse(const Response &response);
    void handleResponseError(const QNetworkReply::NetworkError &error, const Response &response);

    QList<QStandardItem*> parseDirectory(const QByteArray &json);

signals:
    /*!
     * \brief Sygnalizuje błąd żądania.
     * \param error Błąd
     * \param response Odpowiedź z serwera
     */
    void responseError(const QNetworkReply::NetworkError &error, const Response &response);

    /*!
     * \brief Sygnalizuje pomyślne zalogowanie się użytkonwika.
     */
    void userLogged();

    /*!
     * \brief Sygnalizuje pomyślne zarejestrownaie nowego użytkownika.
     */
    void userRegistered();

    /*!
     * \brief Przekazuje informacje o grupach zalogowanego użytkownika.
     * \param groups Grupy użytkownika
     */
    void groupsReceived(const QList<QStandardItem*> &groups);

    /*!
     * \brief Sygnalizuje pomyślne utworzenie nowej grupy.
     */
    void newGroupCreated();

    /*!
     * \brief Przekazuje zawartość aktualnej ścieżki.
     * \param dir Zawartość aktualnej ścieżki.
     */
    void pathReceived(const QList<QStandardItem*> &dir);

    /*!
     * \brief Przekazuje informacje o użytkownikach należących do grupy.
     * \param users Użytkownicy
     */
    void groupUsersReceived(const QList<User> &users);

    /*!
     * \brief Sygnalizuje usunięcie grupy.
     */
    void groupDeletedReceived();

    /*!
     * \brief Sygnalizuje dodanie użytkownika do grupy.
     */
    void groupAddUserReceived();

    /*!
     * \brief Sygnalizuje usunięcie użytkonwika z grupy.
     */
    void groupRemoveUserReceived();

    /*!
     * \brief Sygnalizuje usunięcie pliku lub folderu.
     */
    void resourceDeleted();

    /*!
     * \brief Sygnalizuje stworzenie nowego folderu.
     */
    void newFolderCreated();

    /*!
     * \brief Informuje o aktualnym postępie pobierania.
     * \param current Liczba zrobionych kroków
     * \param total Liczba wszystkich kroków
     */
    void downloadProgress(const qint64 current, const qint64 total);

    /*!
     * \brief Informuje o aktualnym postępie wysyłania.
     * \param current Liczba zrobionych kroków
     * \param total Liczba wszystkich kroków
     */
    void uploadProgress(const qint64 current, const qint64 total);

    /*!
     * \brief Sygnalizuje zakończone wysyłanie pliku.
     */
    void uploadComplete();

    /*!
     * \brief Informuje o błędzie otwieranie pliku.
     * \param filename Nazwa pliku
     */
    void fileOpenError(const QString &filename);

public slots:
    void gotResponse(const Response &response);
    void gotMessage(const int groupId, const Message &msg);

    void requestRegister(const QString &user, const QString &password);
    void requestLogin(const QString &user, const QString &password);
    void requestGroupDelete(const int groupId);
    void requestAddUserToGroup(const QString &username, const int groupId);
    void requestRemoveUserFromGroup(const QString &username, const int groupId);
};

#endif // MODEL_H
