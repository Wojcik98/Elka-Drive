#include "include/downloaditem.h"
#include <QDebug>

DownloadItem::DownloadItem(const int id, const QString &path, const QUrl &url) : id(id), path(path), file(path), url(url) {

}

bool DownloadItem::openFile() {
    bool success = file.open(QIODevice::WriteOnly);
    return success;
}

void DownloadItem::closeFile() {
    file.close();
}

void DownloadItem::deleteFile() {
    file.remove();
}

void DownloadItem::appendData(const QByteArray &data) {
    file.write(data);
}

QUrl DownloadItem::getUrl() {
    return url;
}

QString DownloadItem::getFilePath() {
    return path;
}
