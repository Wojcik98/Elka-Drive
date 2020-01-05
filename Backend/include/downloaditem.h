#ifndef DOWNLOADITEM_H
#define DOWNLOADITEM_H

#include <QFile>
#include <QString>
#include <QDataStream>
#include <QUrl>

class DownloadItem {
public:
    DownloadItem(int id, QString path, QUrl url);
    bool openFile();
    void closeFile();
    void appendData(QByteArray data);
    QUrl getUrl();

private:
    int id;
    QString path;
    QFile file;
    QUrl url;
};

#endif // DOWNLOADITEM_H
