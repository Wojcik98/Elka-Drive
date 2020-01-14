#ifndef REQUESTTYPE_H
#define REQUESTTYPE_H

/*!
 * \brief Typy żądań
 */
enum RequestType {
    LOGIN,              /*!< Zaloguj */
    REGISTER,           /*!< Zarejestruj */
    GROUPS,             /*!< Pokaż grupy użytkownika */
    NEW_GROUP,          /*!< Stwórz nową grupę */
    PATH,               /*!< Pokaż zawartość ścieżki */
    GROUP_USERS,        /*!< Pokaż użytkowników w grupie */
    GROUP_DELETE,       /*!< Usuń grupę */
    GROUP_ADD_USER,     /*!< Dodaj użytkownika do grupy */
    GROUP_REMOVE_USER,  /*!< Usuń użytkonwika z grupy */
    DELETE,             /*!< Usuń plik lub folder */
    NEW_FOLDER,         /*!< Stwórz nowy folder */
    DOWNLOAD,           /*!< Pobierz plik */
    UPLOAD_NEW_FOLDER,  /*!< Stwórz folder przed wysłaniem pliku */
    UPLOAD_SEND_FILE,   /*!< Wyślij plik */
    SEND_MSG            /*!< Wyślij wiadomość */
};

#endif // REQUESTTYPE_H
