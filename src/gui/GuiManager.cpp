#include "GuiManager.hpp"
#include "MainWindow.hpp"

GuiManager::GuiManager(QObject *parent) : QObject(parent) {
  mMainWindow = new MainWindow;
  mMainWindow->show();
}

GuiManager::~GuiManager() { delete mMainWindow; }
