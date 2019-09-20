#pragma once

#include <QRunnable>

class LoadManagersTask final : public QRunnable {
public:
  LoadManagersTask() {}
  virtual ~LoadManagersTask() override {}

  void run() override;
};
