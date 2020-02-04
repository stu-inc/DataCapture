#include "UdpPlayer.hpp"
#include <QDataStream>
#include <QDebug>
#include <QElapsedTimer>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

UdpPlayer::UdpPlayer(QObject *parent) : QThread(parent) {}

UdpPlayer::~UdpPlayer() { stop(); }

void UdpPlayer::start() { QThread::start(); }

void UdpPlayer::stop() {
  requestInterruption();
  wait();
}

void UdpPlayer::restart() {
  stop();
  start();
}

qint64 UdpPlayer::getCurrentTime() const {
  QReadLocker locker(&mLock);
  return mTimer.isNull() || !mTimer->isValid() ? 0 : mTimer->elapsed();
}

void UdpPlayer::setPort(int port) {
  QWriteLocker locker(&mLock);
  mPort = port;
}

void UdpPlayer::setHostAddress(const QHostAddress &address) {
  QWriteLocker locker(&mLock);
  mHostAddress = address;
}

void UdpPlayer::setFileName(const QString &fileName) {
  QWriteLocker locker(&mLock);
  mFileName = fileName;
}

void UdpPlayer::run() {

  mFile = QSharedPointer<QFile>::create();
  mDataStream = QSharedPointer<QDataStream>::create();
  mUdpSocket = QSharedPointer<QUdpSocket>::create();
  mTimer = QSharedPointer<QElapsedTimer>::create();
  quint16 port = 0;
  QHostAddress hostAddress;

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
    port = quint16(mPort);
    hostAddress = mHostAddress;
  }

  // Unnecessary?
  if (!mUdpSocket->open(QIODevice::WriteOnly)) {
    QReadLocker locker(&mLock);
    qWarning() << "Can not open udp port: " + QString::number(mPort);
    return;
  }

  mDataStream->setDevice(mFile.data());

  auto info = QJsonDocument::fromBinaryData(mFile->read(1000)).object();

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
        mUdpSocket->writeDatagram(datagram, hostAddress, port);
        break;
      }
      msleep(10);
    }

    if (mDataStream->atEnd())
      break;
  }

  mTimer->invalidate();

  mUdpSocket->close();
  mFile->close();

  mUdpSocket.clear();
  mDataStream.clear();
  mFile.clear();
  mTimer.clear();
}
