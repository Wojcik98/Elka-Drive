#include "include/uploaditem.h"

UploadItem::UploadItem(QString rootLocal, QString rootServer, QString relativePath) : rootLocal(rootLocal), rootServer(rootServer), relativePath(relativePath) {
    auto separation = relativePath.lastIndexOf("/") + 1;
    relativeDir = relativePath.left(separation);
    filename = relativePath.right(relativePath.size() - separation);
}

QString UploadItem::getRootLocal() {
    return rootLocal;
}

QString UploadItem::getRootServer() {
    return rootServer;
}

QString UploadItem::getRelativeDir() {
    return relativeDir;
}

QString UploadItem::getFilename() {
    return filename;
}

QString UploadItem::getRelativePath() {
    return relativePath;
}
