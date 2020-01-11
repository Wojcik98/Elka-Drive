#ifndef UPLOADITEM_H
#define UPLOADITEM_H

#include <QObject>
#include <QQueue>

class UploadItem : public QObject {
public:
    UploadItem(const QString &rootLocal, const QString &rootServer, const QString &relativePath);

    QString getRootLocal();
    QString getRootServer();
    QString getRelativeDir();
    QString getFilename();
    QString getRelativePath();

private:
    QString rootLocal;
    QString rootServer;
    QString relativePath;
    QString relativeDir;
    QString filename;
};

#endif // UPLOADITEM_H
