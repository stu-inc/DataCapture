#include "Director.hpp"
#include <QThreadPool>

Director *Director::getInstance() {
  static Director instance;
  return &instance;
}

void Director::run(QRunnable *task, bool multiThread) {

  if (multiThread)
    return QThreadPool::globalInstance()->start(task);

  task->run();

  if (task->autoDelete())
    delete task;
}

bool Director::waitForDone(int msecs) {
  return QThreadPool::globalInstance()->waitForDone(msecs);
}
