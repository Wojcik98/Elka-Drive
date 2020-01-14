#include "include/uploaditem.h"

/*!
 * \brief Konstruktor
 * \param rootLocal Folder odniesienia na komputerze użytkownika
 * \param rootServer Folder odniesienia na serwerze
 * \param relativePath Ścieżka względna do pliku względem obu folderów
 *
 * Przykład: rootLocal = /home/user/Desktop, rootServer = 2/path, relativePath = some/file.jpg.
 * Wtedy plik /home/user/Desktop/some/file.jpg zostanie wysłany na ścieżkę 2/path/some/file.jpg.
 */
UploadItem::UploadItem(const QString &rootLocal, const QString &rootServer, const QString &relativePath)
    : rootLocal(rootLocal), rootServer(rootServer), relativePath(relativePath) {
    auto separation = relativePath.lastIndexOf("/") + 1;
    relativeDir = relativePath.left(separation);
    filename = relativePath.right(relativePath.size() - separation);
}

/*!
 * \return Folder odniesienia na komputerze użytkownika
 */
QString UploadItem::getRootLocal() {
    return rootLocal;
}

/*!
 * \return Folder odniesienia na serwerze
 */
QString UploadItem::getRootServer() {
    return rootServer;
}

/*!
 * \return Względna ścieżka do pliku (bez nazwy pliku)
 */
QString UploadItem::getRelativeDir() {
    return relativeDir;
}

/*!
 * \return Nazwa pliku
 */
QString UploadItem::getFilename() {
    return filename;
}

/*!
 * \return Ścieżka względna do pliku względem obu folderów
 */
QString UploadItem::getRelativePath() {
    return relativePath;
}
