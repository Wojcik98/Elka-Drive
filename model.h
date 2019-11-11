#ifndef MODEL_H
#define MODEL_H

#include <QStandardItemModel>

class Model : public QObject {
    Q_OBJECT

public:
    Model();

    QList<QStandardItem*> getGroups();
    QList<QStandardItem*> getPath(QString path);
};

#endif // MODEL_H
