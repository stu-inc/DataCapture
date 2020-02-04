#include "TcpRecorder.hpp"
#include <QDataStream>
#include <QDebug>
#include <QElapsedTimer>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkDatagram>

TcpRecorder::TcpRecorder(QObject *parent) : QThread(parent) {}

TcpRecorder::~TcpRecorder() { stop(); }

void TcpRecorder::start() { QThread::start(); }

void TcpRecorder::stop() {
  quit();
  wait();
}

void TcpRecorder::restart() {
  stop();
  start();
}

qint64 TcpRecorder::getCurrentTime() const {
  QReadLocker locker(&mLock);
  return mTimer.isNull() || !mTimer->isValid() ? 0 : mTimer->elapsed();
}

void TcpRecorder::setPort(int port) {
  QWriteLocker locker(&mLock);
  mPort = port;
}

void TcpRecorder::setHostAddress(const QHostAddress &address) {
  QWriteLocker locker(&mLock);
  mHostAddress = address;
}

void TcpRecorder::setFileName(const QString &fileName) {
  QWriteLocker locker(&mLock);
  mFileName = fileName;
}

void TcpRecorder::readData() {

  auto socket = qobject_cast<QTcpSocket *>(sender());

  if (mTimer->isValid()) {
    *mDataStream << mTimer->elapsed();
    *mDataStream << socket->readAll();
  }
}

void TcpRecorder::showError(QAbstractSocket::SocketError error) {
  qWarning() << error;
}

void TcpRecorder::run() {

  mFile = QSharedPointer<QFile>::create();
  mDataStream = QSharedPointer<QDataStream>::create();
  mTcpSocket = QSharedPointer<QTcpSocket>::create();
  mTimer = QSharedPointer<QElapsedTimer>::create();

  {
    QReadLocker locker(&mLock);
    mFile->setFileName(mFileName);
  }

  if (!mFile->open(QFile::WriteOnly)) {
    QReadLocker locker(&mLock);
    qWarning() << "Can not open file: " + mFileName;
    return;
  }

  // FIXME: why can not connect this signal?
  // connect(mUdpSocket.data(), &QUdpSocket::error, this,
  // &UdpRecorder::showError);

  {
    QReadLocker locker(&mLock);
    if (!mTcpSocket->bind(mHostAddress, quint16(mPort))) {
      qWarning() << "Can not bind udp server: " + QString::number(mPort)
                 << mTcpSocket->error();
      return;
    }
  }

  connect(mTcpSocket.data(), &QTcpSocket::readyRead, this,
          &TcpRecorder::readData, Qt::BlockingQueuedConnection);

  // Fill up data info segments with zero
  mFile->write(QByteArray(1000, '0'));

  mDataStream->setDevice(mFile.data());

  mTimer->start();

  exec();

  auto time = mTimer->elapsed();
  mTimer->invalidate();

  // Data info
  QJsonObject info;

  info["FileSize"] = mFile->size();
  info["TimeLength"] = time;

  mFile->seek(0);
  mFile->write(QJsonDocument(info).toBinaryData());

  mTcpSocket->close();
  mFile->close();

  mTcpSocket.clear();
  mDataStream.clear();
  mFile.clear();
  mTimer.clear();
}
