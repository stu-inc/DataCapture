#include "MainWindow.hpp"
#include "autodef.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setWindowTitle(QString(APPLICATION_NAME) + " v" +
                 QString(APPLICATION_VERSION));
}

MainWindow::~MainWindow() { delete ui; }
