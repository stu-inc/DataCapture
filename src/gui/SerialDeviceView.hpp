#pragma once

#include <QWidget>

namespace Ui {
class SerialDeviceView;
}

class SerialDeviceView : public QWidget {
  Q_OBJECT
public:
  explicit SerialDeviceView(QWidget *parent = nullptr);
  ~SerialDeviceView();

  QString getCurrentDevice() const;

protected slots:
  void updateDeviceInfo(const QString &name);

private:
  Ui::SerialDeviceView *ui;
};
