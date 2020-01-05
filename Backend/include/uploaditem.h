#ifndef UPLOADITEM_H
#define UPLOADITEM_H

#include <QObject>
#include <QQueue>

class UploadItem : public QObject {
public:
    UploadItem(QString rootLocal, QString rootServer, QString relativePath);

    QString getRootLocal();
    QString getRootServer();
    QString getRelativePath();
    QString getFilename();

private:
    QString rootLocal;
    QString rootServer;
    QString relativePath;
};

#endif // UPLOADITEM_H
