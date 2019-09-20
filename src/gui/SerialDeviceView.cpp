#include "SerialDeviceView.hpp"
#include "ui_serialdeviceview.h"
#include <QSerialPortInfo>

SerialDeviceView::SerialDeviceView(QWidget *parent)
    : QWidget(parent), ui(new Ui::SerialDeviceView) {
  ui->setupUi(this);
}

SerialDeviceView::~SerialDeviceView() { delete ui; }

QString SerialDeviceView::getCurrentDevice() const {
  return ui->comboBoxPort->currentText();
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
