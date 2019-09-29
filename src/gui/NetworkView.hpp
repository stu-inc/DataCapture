#pragma once

#include <QHostAddress>
#include <QWidget>

namespace Ui {
class NetworkView;
}

class NetworkView : public QWidget {
  Q_OBJECT
public:
  explicit NetworkView(QWidget *parent = nullptr);
  ~NetworkView();

  QHostAddress getHostAddress() const;
  int getPort() const;

protected slots:
  void setIpToLocalhost();
  void setIpToLocalhostIPv6();
  void setIpToBroadcast();
  void setIpToAnyIPv4();
  void setIpToAnyIPv6();
  void setIpToAny();

private:
  Ui::NetworkView *ui;
};
