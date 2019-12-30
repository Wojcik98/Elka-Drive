#include "include/view.h"

View::View(QWidget *parent) : QMainWindow(parent), groupsWidget(this), filesWidget(this) {

}

View::~View() {

}

void View::showEvent(QShowEvent *ev) {
    QMainWindow::showEvent(ev);
    controller->checkLogin();
}

void View::setButtonsLayouts(QLayout *groupslayout, QLayout *filesLayout) {
    groupsWidget.setLayout(groupslayout);
    groupsWidget.show();
    filesWidget.setLayout(filesLayout);
    filesWidget.hide();
}

void View::setGroupsButtonsVisible() {
    groupsWidget.show();
    filesWidget.hide();
}

void View::setFilesButtonsVisible() {
    groupsWidget.hide();
    filesWidget.show();
}
