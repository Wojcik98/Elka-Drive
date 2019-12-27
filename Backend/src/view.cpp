#include "include/view.h"

View::View(QWidget *parent) : QMainWindow(parent) {

}

View::~View() {

}

void View::showEvent(QShowEvent *ev) {
    QMainWindow::showEvent(ev);
    controller->checkLogin();
}
