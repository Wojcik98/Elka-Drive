#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>

#include "mainwindow.h"
#include "model.h"

class MainWindow;

class Controller : public QObject {
    Q_OBJECT
public:
    explicit Controller();

    void setView(MainWindow *view);
    void setModel(Model *model);

    void showGroups();

private:
    MainWindow *view;
    Model *model;

    QStringList path;

public slots:
    void fileDoubleClicked(const QModelIndex &index);
    void goBack();
};

#endif // CONTROLLER_H
