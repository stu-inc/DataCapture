#pragma once

#include <QRunnable>

class UnloadManagersTask final : public QRunnable {
public:
  UnloadManagersTask() {}
  virtual ~UnloadManagersTask() override {}

  void run() override;
};
