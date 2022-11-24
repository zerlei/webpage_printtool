#include "clientwebsocket.h"

#include "net_interface/clientwebsocket.h"
#include <json/json.h>
#include <json/value.h>
#include <json/writer.h>
#include <memory>
#include <qtimer.h>
#include <qtmetamacros.h>
#include <qurl.h>
#include <string>

ClientWebsoc::ClientWebsoc(PrintMsgStation &printmsgstation)
    : _print_msg_station(printmsgstation) {
  auto f = [this](const std::string &url) { 
    
    QString url_str(url.c_str());
    emit signalsetWebSocUrl(url_str);
  };
  _print_msg_station._set_websoc_url = f;

  _print_msg_station._get_websoc_state = [this]() {
    return this->_webSoc.state() == 1;
  };
  _webSoc.ignoreSslErrors();
  connect(&_webSoc, &QWebSocket::connected, this, &ClientWebsoc::slotOnConnect);
  connect(&_webSoc, &QWebSocket::disconnected, this,
          &ClientWebsoc::slotDisConnect);
  connect(&_webSoc,
          QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), this,
          &ClientWebsoc::slotErr);
  connect(&_webSoc, &QWebSocket::textMessageReceived, this,
          &ClientWebsoc::slotReceiveMsg);
  connect(&_timer, &QTimer::timeout, this, &ClientWebsoc::slotTimerReConnect);
  connect(this, &ClientWebsoc::signalsetWebSocUrl,this,&ClientWebsoc::slotOpenUrl, Qt::QueuedConnection);

  auto [is_connected,url] = _print_msg_station.getWebsocketUrAndState();
  this->openUrl(QUrl(url.c_str()));
}

void ClientWebsoc::openUrl(const QUrl &url) {
  if(_url.toString() == url.toString()) {
    return;
  }
  _url = url;
  if (_webSoc.state() != QAbstractSocket::UnconnectedState) {
    _webSoc.close(QWebSocketProtocol::CloseCodeNormal);
  }
  slotTimerReConnect();
}

void ClientWebsoc::slotReceiveMsg(const QString &message) {

  auto f = [this](const Json::Value &v) {
    this->_webSoc.sendTextMessage(
        QString::fromStdString(this->JsonValueToString(v)));
  };
  auto ip = _url.host().toStdString();
  std::string fromtype = "ClientWebsocket";
  _print_msg_station.workWithStringAsync(message.toStdString(), ip,fromtype,f);
}

void ClientWebsoc::slotOnConnect() {
  _timer.stop();
  _print_msg_station.setClientWebSockState(true);
}

void ClientWebsoc::slotDisConnect() {
  _timer.start(100000);
  _print_msg_station.setClientWebSockState(false);
  // _webSoc.open(url);
}
void ClientWebsoc::slotTimerReConnect() { _webSoc.open(_url); }

void ClientWebsoc::slotErr(QAbstractSocket::SocketError error) {

  _print_msg_station.setClientWebSockState(false);

  _timer.start(100000);
}

void ClientWebsoc::slotSSLErr(const QList<QSslError> &errors) {}
std::string ClientWebsoc::JsonValueToString(const Json::Value &value) {
  return Json::FastWriter().write(value);
}
void ClientWebsoc::slotOpenUrl(const QString url_str) {
  QUrl url(url_str);
  this->openUrl(url);
}