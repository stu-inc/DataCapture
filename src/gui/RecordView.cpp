#include "RecordView.hpp"
#include "serial/SerialRecorder.hpp"
#include "ui_recordview.h"

RecordView::RecordView(QWidget *parent)
    : QWidget(parent), ui(new Ui::RecordView) {
  ui->setupUi(this);
  ui->pushButtonStopRecord->setEnabled(false);
  mRecorder = QSharedPointer<SerialRecorder>::create();
}

RecordView::~RecordView() { delete ui; }

void RecordView::startRecord() {

  ui->pushButtonStartRecord->setEnabled(false);
  ui->groupBoxDevice->setEnabled(false);
  ui->groupBoxFile->setEnabled(false);
  ui->pushButtonStopRecord->setEnabled(true);

  mRecorder->setPortName(ui->widgetSerialDevice->getCurrentDevice());
  mRecorder->setFileName(ui->widgetRecordFile->getFileName());

  mRecorder->setBaundRate(ui->widgetSerialDevice->getBaundRate());
  mRecorder->setDataBits(ui->widgetSerialDevice->getDataBits());
  mRecorder->setParity(ui->widgetSerialDevice->getParity());
  mRecorder->setStopBits(ui->widgetSerialDevice->getStopBits());

  mRecorder->start();
}

void RecordView::stopRecord() {

  ui->pushButtonStopRecord->setEnabled(false);
  ui->pushButtonStartRecord->setEnabled(true);
  ui->groupBoxDevice->setEnabled(true);
  ui->groupBoxFile->setEnabled(true);

  mRecorder->stop();
}
