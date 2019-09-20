#pragma once

#include <QObject>

class MainWindow;

class GuiManager : public QObject {
  Q_OBJECT
public:
  explicit GuiManager(QObject *parent = nullptr);
  ~GuiManager();

signals:

public slots:

private:
  MainWindow *mMainWindow = nullptr;
};
