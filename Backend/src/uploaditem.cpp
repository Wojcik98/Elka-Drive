#include "include/uploaditem.h"

UploadItem::UploadItem(QString rootLocal, QString rootServer, QString path) : rootLocal(rootLocal), rootServer(rootServer), relativePath(path) {

}

QString UploadItem::getRootLocal() {
    return rootLocal;
}

QString UploadItem::getRootServer() {
    return rootServer;
}

QString UploadItem::getRelativePath() {
    return relativePath;
}

QString UploadItem::getFilename() {
    auto separation = relativePath.lastIndexOf("/") + 1;
    return relativePath.right(relativePath.size() - separation);
}
