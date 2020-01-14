#include "include/downloaditem.h"
#include <QDebug>

/*!
 * \brief Konstruktor.
 * \param id ID pliku do pobrania
 * \param path Ścieżka zapisu pliku
 * \param url URL do wysłania żądania pobrania
 */
DownloadItem::DownloadItem(const int id, const QString &path, const QUrl &url)
    : id(id), path(path), file(path), url(url) {

}

/*!
 * \brief Otwiera plik do zapisu.
 * \return `true` jeśli udało się otworzyć plik, `false` w przeciwnym wypadku
 */
bool DownloadItem::openFile() {
    bool success = file.open(QIODevice::WriteOnly);
    return success;
}

/*!
 * \brief Zamyka plik do zapisu.
 */
void DownloadItem::closeFile() {
    file.close();
}

/*!
 * \brief Usuwa plik do zapisu.
 */
void DownloadItem::deleteFile() {
    file.remove();
}

/*!
 * \brief Zapisuje dane do pliku.
 * \param data Dane do zapisu
 */
void DownloadItem::appendData(const QByteArray &data) {
    file.write(data);
}

/*!
 * \brief Zwraca URL żądania.
 * \return URL żądania
 */
QUrl DownloadItem::getUrl() {
    return url;
}

/*!
 * \brief Zwraca ścieżkę pliku.
 * \return Ścieżka pliku
 */
QString DownloadItem::getFilePath() {
    return path;
}
