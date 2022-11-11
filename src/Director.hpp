#pragma once

#include <QHash>
#include <QObject>
#include <QReadWriteLock>
#include <QSharedPointer>

class QRunnable;

class Director final : public QObject {
  Q_OBJECT
public:
  /* Instance */
  static Director *getInstance();

  /* Object */
  template <class T> QSharedPointer<T> getObject() {
    QReadLocker locker(&mLock);
    return qSharedPointerCast<T>(mObjects[QString(typeid(T).name())]);
  }

  template <class T> void loadObject() {
    QWriteLocker locker(&mLock);
    mObjects[QString(typeid(T).name())] = QSharedPointer<T>::create();
  }

  template <class T> void unloadObject() {
    QWriteLocker locker(&mLock);
    mObjects.remove(QString(typeid(T).name()));
  }

  /* Run */
  void run(QRunnable *task, bool multiThread = false);

  /* Wait for done */
  bool waitForDone(int msecs = -1);

private:
  /* Constructor and destructor */
  Director() = default;
  ~Director() = default;
  Director(const Director &) = delete;
  Director(const Director &&) = delete;
  Director &operator=(const Director &) = delete;
  Director &operator=(const Director &&) = delete;

private:
  /* Lock */
  mutable QReadWriteLock mLock;

  /* Objects */
  QHash<QString, QSharedPointer<QObject>> mObjects;
};
