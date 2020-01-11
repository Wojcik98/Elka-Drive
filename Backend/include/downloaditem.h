#ifndef DOWNLOADITEM_H
#define DOWNLOADITEM_H

#include <QFile>
#include <QString>
#include <QDataStream>
#include <QUrl>

class DownloadItem {
public:
    DownloadItem(const int id, const QString &path, const QUrl &url);
    bool openFile();
    void closeFile();
    void deleteFile();
    void appendData(const QByteArray &data);
    QUrl getUrl();
    QString getFilePath();

private:
    int id;
    QString path;
    QFile file;
    QUrl url;
};

#endif // DOWNLOADITEM_H
