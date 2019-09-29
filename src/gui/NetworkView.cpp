#include "NetworkView.hpp"
#include "ui_networkview.h"
#include <QHostAddress>
#include <QMenu>

NetworkView::NetworkView(QWidget *parent)
    : QWidget(parent), ui(new Ui::NetworkView) {

  ui->setupUi(this);

  auto menu = new QMenu();

  menu->addAction("Localhost", this, &NetworkView::setIpToLocalhost);
  menu->addAction("Localhost IPv6", this, &NetworkView::setIpToLocalhostIPv6);
  menu->addAction("Broadcast", this, &NetworkView::setIpToBroadcast);
  menu->addAction("Any IPv4", this, &NetworkView::setIpToAnyIPv4);
  menu->addAction("Any IPv6", this, &NetworkView::setIpToAnyIPv6);
  menu->addAction("Any", this, &NetworkView::setIpToAny);

  ui->toolButtonIp->setMenu(menu);
}

NetworkView::~NetworkView() { delete ui; }

QHostAddress NetworkView::getHostAddress() const {
  return QHostAddress(ui->lineEditIpAddress->text());
}

int NetworkView::getPort() const { return ui->spinBoxPort->value(); }

void NetworkView::setIpToLocalhost() {
  ui->lineEditIpAddress->setText(
      QHostAddress(QHostAddress::LocalHost).toString());
}

void NetworkView::setIpToLocalhostIPv6() {
  ui->lineEditIpAddress->setText(
      QHostAddress(QHostAddress::LocalHostIPv6).toString());
}

void NetworkView::setIpToBroadcast() {
  ui->lineEditIpAddress->setText(
      QHostAddress(QHostAddress::Broadcast).toString());
}

void NetworkView::setIpToAnyIPv4() {
  ui->lineEditIpAddress->setText(
      QHostAddress(QHostAddress::AnyIPv4).toString());
}

void NetworkView::setIpToAnyIPv6() {
  ui->lineEditIpAddress->setText(
      QHostAddress(QHostAddress::AnyIPv6).toString());
}

void NetworkView::setIpToAny() {
  ui->lineEditIpAddress->setText(QHostAddress(QHostAddress::Any).toString());
}
