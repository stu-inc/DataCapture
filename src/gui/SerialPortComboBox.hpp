#pragma once

#include <QComboBox>

class SerialPortComboBox : public QComboBox {
  Q_OBJECT
public:
  explicit SerialPortComboBox(QWidget *parent = nullptr);
  virtual ~SerialPortComboBox() override;

protected slots:

protected:
  virtual void focusInEvent(QFocusEvent *e) override;
  void updateDeviceList();
};
