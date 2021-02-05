#include "RecordView.hpp"
#include "NetworkView.hpp"
#include "SerialDeviceView.hpp"
#include "serial/SerialRecorder.hpp"
#include "tcp/TcpRecorder.hpp"
#include "udp/UdpRecorder.hpp"
#include "ui_recordview.h"
#include <QTime>

RecordView::RecordView(QWidget *parent)
    : QWidget(parent), ui(new Ui::RecordView) {

  ui->setupUi(this);

  ui->pushButtonStopRecord->setEnabled(false);

  ui->comboBoxInterface->addItem("Serial Port");
  ui->comboBoxInterface->addItem("UDP");
  ui->comboBoxInterface->addItem("TCP");

  mSerialRecorder = QSharedPointer<SerialRecorder>::create();
  mUdpRecorder = QSharedPointer<UdpRecorder>::create();
  mTcpRecorder = QSharedPointer<TcpRecorder>::create();
}

RecordView::~RecordView() { delete ui; }

void RecordView::startRecord() {

  ui->pushButtonStartRecord->setEnabled(false);
  ui->groupBoxDevice->setEnabled(false);
  ui->groupBoxFile->setEnabled(false);
  ui->pushButtonStopRecord->setEnabled(true);

  if (ui->comboBoxInterface->currentIndex() == 0 /* Serial Port */) {

    auto w = qobject_cast<SerialDeviceView *>(
        ui->gridLayoutInput->itemAt(0)->widget());

    mSerialRecorder->setPortName(w->getCurrentDevice());
    mSerialRecorder->setFileName(ui->widgetRecordFile->getFileName());

    mSerialRecorder->setBaundRate(w->getBaundRate());
    mSerialRecorder->setDataBits(w->getDataBits());
    mSerialRecorder->setParity(w->getParity());
    mSerialRecorder->setStopBits(w->getStopBits());

    mSerialRecorder->start();

  } else if (ui->comboBoxInterface->currentIndex() == 1 /* UDP */) {

    auto w =
        qobject_cast<NetworkView *>(ui->gridLayoutInput->itemAt(0)->widget());

    mUdpRecorder->setHostAddress(w->getHostAddress());
    mUdpRecorder->setPort(w->getPort());
    mUdpRecorder->setFileName(ui->widgetRecordFile->getFileName());

    mUdpRecorder->start();

  } else if (ui->comboBoxInterface->currentIndex() == 2 /* TCP */) {

    auto w =
        qobject_cast<NetworkView *>(ui->gridLayoutInput->itemAt(0)->widget());

    mTcpRecorder->setHostAddress(w->getHostAddress());
    mTcpRecorder->setPort(w->getPort());
    mTcpRecorder->setFileName(ui->widgetRecordFile->getFileName());

    mTcpRecorder->start();
  }

  mTimerId = startTimer(33);
}

void RecordView::stopRecord() {

  ui->pushButtonStopRecord->setEnabled(false);
  ui->pushButtonStartRecord->setEnabled(true);
  ui->groupBoxDevice->setEnabled(true);
  ui->groupBoxFile->setEnabled(true);

  if (ui->comboBoxInterface->currentIndex() == 0 /* Serial Port */) {
    mSerialRecorder->stop();
  } else if (ui->comboBoxInterface->currentIndex() == 1 /* UDP */) {
    mUdpRecorder->stop();
  } else if (ui->comboBoxInterface->currentIndex() == 2 /* TCP */) {
    mTcpRecorder->stop();
  }

  killTimer(mTimerId);
}

void RecordView::changeInterface(int index) {

  if (index == 0 /* Serial Port */) {

    auto w = ui->gridLayoutInput->itemAt(0)->widget();
    ui->gridLayoutInput->replaceWidget(w, new SerialDeviceView);
    delete w;

  } else if (index == 1 /* UDP */ || index == 2 /* TCP */) {

    auto w = ui->gridLayoutInput->itemAt(0)->widget();
    ui->gridLayoutInput->replaceWidget(w, new NetworkView);
    delete w;
  }
}

void RecordView::timerEvent(QTimerEvent *e) {

  int currentTime = 0;
  qint64 fileSize = 0;

  if (ui->comboBoxInterface->currentIndex() == 0 /* Serial Port */) {
    currentTime = int(mSerialRecorder->getCurrentTime());
    fileSize = mSerialRecorder->getCurrentFileSize();
  } else if (ui->comboBoxInterface->currentIndex() == 1 /* UDP */) {
    currentTime = int(mUdpRecorder->getCurrentTime());
    fileSize = mUdpRecorder->getCurrentFileSize();
  } else if (ui->comboBoxInterface->currentIndex() == 2 /* TCP */) {
    currentTime = int(mTcpRecorder->getCurrentTime());
    fileSize = mTcpRecorder->getCurrentFileSize();
  }

  QTime time(0, 0);
  auto current = time.addMSecs(currentTime);

  ui->widgetRecordFile->setTimeText(current.toString("HH:mm:ss:zzz"));
  ui->widgetRecordFile->setSizeText(QString::number(fileSize));

  e->accept();
}
