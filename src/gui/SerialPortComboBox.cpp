#include "SerialPortComboBox.hpp"
#include <QContextMenuEvent>
#include <QMenu>
#include <QSerialPortInfo>

SerialPortComboBox::SerialPortComboBox(QWidget *parent) : QComboBox(parent) {
  updateDeviceList();
}

SerialPortComboBox::~SerialPortComboBox() {}

void SerialPortComboBox::contextMenuEvent(QContextMenuEvent *e) {

  QMenu menu(this);

  auto updateAction = new QAction("Update Device List", this);
  connect(updateAction, &QAction::triggered, this,
          &SerialPortComboBox::updateDeviceList);
  menu.addAction(updateAction);

  menu.exec(mapToGlobal(e->pos()));
}

void SerialPortComboBox::updateDeviceList() {

  auto ports = QSerialPortInfo::availablePorts();

  QStringList serialPorts;

  for (int i = 0; i < ports.size(); ++i)
    serialPorts.append(ports[i].portName());

  insertItems(0, serialPorts);

  while (serialPorts.size() < count())
    removeItem(count() - 1);
}
