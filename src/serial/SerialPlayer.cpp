#include "SerialPlayer.hpp"
#include <QDataStream>
#include <QDebug>
#include <QElapsedTimer>
#include <QFile>
#include <QSerialPort>

SerialPlayer::SerialPlayer(QObject *parent) : QThread(parent) {}

SerialPlayer::~SerialPlayer() { stop(); }

void SerialPlayer::start() { QThread::start(); }

void SerialPlayer::stop() {
  requestInterruption();
  wait();
}

void SerialPlayer::restart() {
  stop();
  start();
}

void SerialPlayer::setPortName(const QString &name) {
  QWriteLocker locker(&mLock);
  mPortName = name;
}

void SerialPlayer::setFileName(const QString &fileName) {
  QWriteLocker locker(&mLock);
  mFileName = fileName;
}

void SerialPlayer::run() {

  mFile = QSharedPointer<QFile>::create();
  mDataStream = QSharedPointer<QDataStream>::create();
  mSerialPort = QSharedPointer<QSerialPort>::create();
  mTimer = QSharedPointer<QElapsedTimer>::create();

  {
    QReadLocker locker(&mLock);
    mFile->setFileName(mFileName);
  }

  if (!mFile->open(QFile::ReadOnly)) {
    QReadLocker locker(&mLock);
    qWarning() << "Can not open file: " + mFileName;
    return;
  }

  {
    QReadLocker locker(&mLock);
    mSerialPort->setPortName(mPortName);
  }

  if (!mSerialPort->open(QIODevice::WriteOnly)) {
    QReadLocker locker(&mLock);
    qWarning() << "Can not open serial port: " + mPortName;
    return;
  }

  mDataStream->setDevice(mFile.data());

  mTimer->start();

  while (!isInterruptionRequested()) {

    qint64 time;
    QByteArray bytes;

    *mDataStream >> time;
    *mDataStream >> bytes;

    while (true) {
      if (mTimer->elapsed() >= time) {
        mSerialPort->write(bytes);
        break;
      }
      msleep(10);
    }

    if (mDataStream->atEnd())
      break;
  }

  mTimer->invalidate();

  mSerialPort->close();
  mFile->close();

  mSerialPort.clear();
  mDataStream.clear();
  mFile.clear();
  mTimer.clear();
}
