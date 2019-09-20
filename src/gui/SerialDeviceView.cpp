#include "SerialDeviceView.hpp"
#include "ui_serialdeviceview.h"
#include <QSerialPortInfo>

SerialDeviceView::SerialDeviceView(QWidget *parent)
    : QWidget(parent), ui(new Ui::SerialDeviceView) {
  ui->setupUi(this);
  ui->comboBoxPort->setCurrentIndex(-1);

  ui->comboBoxBaudRate->addItem("1200 baud", QSerialPort::Baud1200);
  ui->comboBoxBaudRate->addItem("2400 baud", QSerialPort::Baud2400);
  ui->comboBoxBaudRate->addItem("4800 baud", QSerialPort::Baud4800);
  ui->comboBoxBaudRate->addItem("9600 baud", QSerialPort::Baud9600);
  ui->comboBoxBaudRate->addItem("19200 baud", QSerialPort::Baud19200);
  ui->comboBoxBaudRate->addItem("38400 baud", QSerialPort::Baud38400);
  ui->comboBoxBaudRate->addItem("57600 baud", QSerialPort::Baud57600);
  ui->comboBoxBaudRate->addItem("115200 baud", QSerialPort::Baud115200);

  ui->comboBoxBaudRate->setCurrentIndex(3);

  ui->comboBoxDataBits->addItem("Data5", QSerialPort::Data5);
  ui->comboBoxDataBits->addItem("Data6", QSerialPort::Data6);
  ui->comboBoxDataBits->addItem("Data7", QSerialPort::Data7);
  ui->comboBoxDataBits->addItem("Data8", QSerialPort::Data8);

  ui->comboBoxDataBits->setCurrentIndex(3);

  ui->comboBoxParity->addItem("NoParity", QSerialPort::NoParity);
  ui->comboBoxParity->addItem("EvenParity", QSerialPort::EvenParity);
  ui->comboBoxParity->addItem("OddParity", QSerialPort::OddParity);
  ui->comboBoxParity->addItem("SpaceParity", QSerialPort::SpaceParity);
  ui->comboBoxParity->addItem("MarkParity", QSerialPort::MarkParity);
  ui->comboBoxParity->addItem("NoParity", QSerialPort::NoParity);

  ui->comboBoxDataBits->setCurrentIndex(0);

  ui->comboBoxStopBits->addItem("OneStop", QSerialPort::OneStop);
  ui->comboBoxStopBits->addItem("OneAndHalfStop", QSerialPort::OneAndHalfStop);
  ui->comboBoxStopBits->addItem("TwoStop", QSerialPort::TwoStop);

  ui->comboBoxStopBits->setCurrentIndex(0);
}

SerialDeviceView::~SerialDeviceView() { delete ui; }

QString SerialDeviceView::getCurrentDevice() const {
  return ui->comboBoxPort->currentText();
}

QSerialPort::BaudRate SerialDeviceView::getBaundRate() const {
  return static_cast<QSerialPort::BaudRate>(
      ui->comboBoxBaudRate->currentData().toInt());
}

QSerialPort::DataBits SerialDeviceView::getDataBits() const {
  return static_cast<QSerialPort::DataBits>(
      ui->comboBoxDataBits->currentData().toInt());
}

QSerialPort::Parity SerialDeviceView::getParity() const {
  return static_cast<QSerialPort::Parity>(
      ui->comboBoxParity->currentData().toInt());
}

QSerialPort::StopBits SerialDeviceView::getStopBits() const {
  return static_cast<QSerialPort::StopBits>(
      ui->comboBoxStopBits->currentData().toInt());
}

void SerialDeviceView::updateDeviceInfo(const QString &name) {

  for (auto &port : QSerialPortInfo::availablePorts()) {

    if (port.portName() == name) {

      QString text;

      text += "Port Name:\t\t" + port.portName() + "\n";
      text += "Description:\t\t" + port.description() + "\n";
      text += "Manufacturer:\t\t" + port.manufacturer() + "\n";
      text += "Serial Number:\t\t" + port.serialNumber() + "\n";
      text += "System Location:\t" + port.systemLocation() + "\n";

      if (port.hasVendorIdentifier())
        text += "Vendor Identifier:\t" +
                QString::number(port.vendorIdentifier()) + "\n";

      if (port.hasProductIdentifier())
        text += "Product Identifier:\t" +
                QString::number(port.productIdentifier()) + "\n";

      ui->textBrowserInfo->setText(text);

      return;
    }
  }

  ui->textBrowserInfo->clear();
}
