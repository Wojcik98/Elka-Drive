#include "include/downloaditem.h"
#include <QDebug>

DownloadItem::DownloadItem(int id, QString path, QUrl url) : id(id), path(path), file(path), url(url) {

}

bool DownloadItem::openFile() {
    bool success = file.open(QIODevice::WriteOnly);
    return success;
}

void DownloadItem::closeFile() {
    file.close();
}

void DownloadItem::appendData(QByteArray data) {
    file.write(data);
}

QUrl DownloadItem::getUrl() {
    return url;
}
