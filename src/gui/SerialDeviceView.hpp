#pragma once

#include <QWidget>
#include <QtSerialPort>

namespace Ui {
class SerialDeviceView;
}

class SerialDeviceView : public QWidget {
  Q_OBJECT
public:
  explicit SerialDeviceView(QWidget *parent = nullptr);
  ~SerialDeviceView();

  QString getCurrentDevice() const;
  QSerialPort::BaudRate getBaundRate() const;
  QSerialPort::DataBits getDataBits() const;
  QSerialPort::Parity getParity() const;
  QSerialPort::StopBits getStopBits() const;

protected slots:
  void updateDeviceInfo(const QString &name);

private:
  Ui::SerialDeviceView *ui;
};
