#include "TcpPlayer.hpp"
#include <QDataStream>
#include <QDebug>
#include <QElapsedTimer>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

TcpPlayer::TcpPlayer(QObject *parent) : QThread(parent) {}

TcpPlayer::~TcpPlayer() { stop(); }

void TcpPlayer::start() { QThread::start(); }

void TcpPlayer::stop() {
  requestInterruption();
  wait();
}

void TcpPlayer::restart() {
  stop();
  start();
}

qint64 TcpPlayer::getCurrentTime() const {
  QReadLocker locker(&mLock);
  return mTimer.isNull() || !mTimer->isValid() ? 0 : mTimer->elapsed();
}

void TcpPlayer::setPort(int port) {
  QWriteLocker locker(&mLock);
  mPort = port;
}

void TcpPlayer::setHostAddress(const QHostAddress &address) {
  QWriteLocker locker(&mLock);
  mHostAddress = address;
}

void TcpPlayer::setFileName(const QString &fileName) {
  QWriteLocker locker(&mLock);
  mFileName = fileName;
}

void TcpPlayer::run() {

  mFile = QSharedPointer<QFile>::create();
  mDataStream = QSharedPointer<QDataStream>::create();
  mTcpSocket = QSharedPointer<QTcpSocket>::create();
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
    mTcpSocket->connectToHost(mHostAddress, quint16(mPort));
  }

  if (!mTcpSocket->waitForConnected(3000)) {
    QReadLocker locker(&mLock);
    qWarning() << "Can not open udp port: " + QString::number(mPort);
    return;
  }

  mDataStream->setDevice(mFile.data());

  auto info = QJsonDocument::fromJson(mFile->read(1000)).object();

  int fileSize = info["FileSize"].toInt();
  int timeLength = info["TimeLength"].toInt();

  mTimer->start();

  while (!isInterruptionRequested()) {

    qint64 time;
    QByteArray datagram;

    *mDataStream >> time;
    *mDataStream >> datagram;

    while (true) {
      if (mTimer->elapsed() >= time) {
        mTcpSocket->write(datagram);
        break;
      }
      msleep(10);
    }

    if (mDataStream->atEnd())
      break;
  }

  mTimer->invalidate();

  mTcpSocket->close();
  mFile->close();

  mTcpSocket.clear();
  mDataStream.clear();
  mFile.clear();
  mTimer.clear();
}
