#include "SerialPortComboBox.hpp"
#include <QSerialPortInfo>

SerialPortComboBox::SerialPortComboBox(QWidget *parent) : QComboBox(parent) {
  updateDeviceList();
}

SerialPortComboBox::~SerialPortComboBox() {}

void SerialPortComboBox::focusInEvent(QFocusEvent *) { updateDeviceList(); }

void SerialPortComboBox::updateDeviceList() {

  auto ports = QSerialPortInfo::availablePorts();

  QStringList serialPorts;

  for (int i = 0; i < ports.size(); ++i)
    serialPorts.append(ports[i].portName());

  insertItems(0, serialPorts);

  while (serialPorts.size() < count())
    removeItem(count() - 1);
}
