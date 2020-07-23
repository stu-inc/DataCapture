#include "PlaybackView.hpp"
#include "NetworkView.hpp"
#include "SerialDeviceView.hpp"
#include "serial/SerialPlayer.hpp"
#include "tcp/TcpPlayer.hpp"
#include "udp/UdpPlayer.hpp"
#include "ui_playbackview.h"

PlaybackView::PlaybackView(QWidget *parent)
    : QWidget(parent), ui(new Ui::PlaybackView) {

  ui->setupUi(this);

  ui->pushButtonStopPlayback->setEnabled(false);

  ui->comboBoxInterface->addItem("Serial Port");
  ui->comboBoxInterface->addItem("UDP");
  ui->comboBoxInterface->addItem("TCP");

  mSerialPlayer = QSharedPointer<SerialPlayer>::create();
  mUdpPlayer = QSharedPointer<UdpPlayer>::create();
  mTcpPlayer = QSharedPointer<TcpPlayer>::create();
}

PlaybackView::~PlaybackView() { delete ui; }

void PlaybackView::startPlayback() {

  ui->pushButtonStartPlayback->setEnabled(false);
  ui->groupBoxDevice->setEnabled(false);
  ui->groupBoxFile->setEnabled(false);
  ui->pushButtonStopPlayback->setEnabled(true);

  if (ui->comboBoxInterface->currentIndex() == 0 /* Serial Port */) {

    auto w = qobject_cast<SerialDeviceView *>(
        ui->gridLayoutOutput->itemAt(0)->widget());

    mSerialPlayer->setPortName(w->getCurrentDevice());
    mSerialPlayer->setFileName(ui->widgetPlaybackFile->getFileName());

    mSerialPlayer->setBaundRate(w->getBaundRate());
    mSerialPlayer->setDataBits(w->getDataBits());
    mSerialPlayer->setParity(w->getParity());
    mSerialPlayer->setStopBits(w->getStopBits());

    mSerialPlayer->start();

  } else if (ui->comboBoxInterface->currentIndex() == 1 /* UDP */) {

    auto w =
        qobject_cast<NetworkView *>(ui->gridLayoutOutput->itemAt(0)->widget());

    mUdpPlayer->setHostAddress(w->getHostAddress());
    mUdpPlayer->setPort(w->getPort());
    mUdpPlayer->setFileName(ui->widgetPlaybackFile->getFileName());

    mUdpPlayer->start();

  } else if (ui->comboBoxInterface->currentIndex() == 2 /* TCP */) {

    auto w =
        qobject_cast<NetworkView *>(ui->gridLayoutOutput->itemAt(0)->widget());

    mTcpPlayer->setHostAddress(w->getHostAddress());
    mTcpPlayer->setPort(w->getPort());
    mTcpPlayer->setFileName(ui->widgetPlaybackFile->getFileName());

    mTcpPlayer->start();
  }

  mTimerId = startTimer(33);
}

void PlaybackView::stopPlayback() {

  ui->pushButtonStopPlayback->setEnabled(false);
  ui->pushButtonStartPlayback->setEnabled(true);
  ui->groupBoxDevice->setEnabled(true);
  ui->groupBoxFile->setEnabled(true);

  if (ui->comboBoxInterface->currentIndex() == 0 /* Serial Port */) {
    mSerialPlayer->stop();
  } else if (ui->comboBoxInterface->currentIndex() == 1 /* UDP */) {
    mUdpPlayer->stop();
  } else if (ui->comboBoxInterface->currentIndex() == 2 /* TCP */) {
    mTcpPlayer->stop();
  }

  killTimer(mTimerId);
}

void PlaybackView::changeInterface(int index) {

  if (index == 0 /* Serial Port */) {

    auto w = ui->gridLayoutOutput->itemAt(0)->widget();
    ui->gridLayoutOutput->replaceWidget(w, new SerialDeviceView);
    delete w;

  } else if (index == 1 /* UDP */ || index == 2 /* TCP */) {

    auto w = ui->gridLayoutOutput->itemAt(0)->widget();
    ui->gridLayoutOutput->replaceWidget(w, new NetworkView);
    delete w;
  }
}

void PlaybackView::timerEvent(QTimerEvent *e) {

  int currentTime = 0;

  if (ui->comboBoxInterface->currentIndex() == 0 /* Serial Port */) {
    currentTime = int(mSerialPlayer->getCurrentTime());
  } else if (ui->comboBoxInterface->currentIndex() == 1 /* UDP */) {
    currentTime = int(mUdpPlayer->getCurrentTime());
  } else if (ui->comboBoxInterface->currentIndex() == 2 /* TCP */) {
    currentTime = int(mTcpPlayer->getCurrentTime());
  }

  QTime time(0, 0);
  auto current = time.addMSecs(currentTime);

  ui->widgetPlaybackFile->setTimeText(current.toString("HH:mm:ss:zzz"));

  e->accept();
}
