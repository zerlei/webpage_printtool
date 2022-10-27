#include "clientwebsocket.h"
#include "../printer/printmsgstation.h"
#include "net_interface/clientwebsocket.h"
#include <json/json.h>
#include <json/value.h>
#include <json/writer.h>
#include <memory>
#include <qtimer.h>

Websoc::Websoc(PrintMsgStation *printmsgstation)
    : _print_msg_station(printmsgstation) {

  _webSoc.ignoreSslErrors();
  connect(&_webSoc, &QWebSocket::connected, this, &Websoc::slotOnConnect);
  connect(&_webSoc, &QWebSocket::disconnected, this, &Websoc::slotDisConnect);
  connect(&_webSoc,
          QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), this,
          &Websoc::slotErr);
  connect(&_webSoc, &QWebSocket::textMessageReceived, this,
          &Websoc::slotReceiveMsg);
  connect(&_timer, &QTimer::timeout, this, &Websoc::slotTimerReConnect);
}

void Websoc::openUrl(const QUrl &url) {
  _url = url;
  if (_webSoc.state() != QAbstractSocket::UnconnectedState) {
    _webSoc.close(QWebSocketProtocol::CloseCodeNormal);
  }
  slotTimerReConnect();
}

void Websoc::slotReceiveMsg(const QString &message) {

  Json::Reader reader;
  Json::Value jsob;
  if (reader.parse(message.toStdString(), jsob)) {
    auto msgtype = jsob["msgtype"];
    if (msgtype.isNull()) {
      return;
    }
    Json::FastWriter fw;

    auto msgtype_str = msgtype.asString();

    if (msgtype_str == "GetPrintInfo") {
      _webSoc.sendTextMessage(QString::fromStdString(
          fw.write(_print_msg_station->getPrintInfo(true))));

    } else if (msgtype_str == "AddOnePrintConfig") {

      if (jsob["Data"].isObject()) {
        auto json = std::make_shared<Json::Value>();
        *json = jsob["Data"];
        _webSoc.sendTextMessage(QString::fromStdString(
            fw.write(_print_msg_station->addOnePrintConfig(json))));
      }

    } else if (msgtype_str == "DelOnePrintConfig") {

      if (jsob["Data"].isInt()) {
        _webSoc.sendTextMessage(QString::fromStdString(fw.write(
            _print_msg_station->delOnePrintConfig(jsob["Data"].asInt()))));
      }

    } else if (msgtype_str == "UpdateOnePrintConfig") {

      if (jsob["Data"].isObject()) {
        auto json = std::make_shared<Json::Value>();
        *json = jsob["Data"];
        _webSoc.sendTextMessage(QString::fromStdString(
            fw.write(_print_msg_station->updateOnePrintConfig(json))));
      }

    } else if (msgtype_str == "GetPrintConfigs") {
      _webSoc.sendTextMessage(QString::fromStdString(
          fw.write(_print_msg_station->getPrintConfigs())));

    } else if (msgtype_str == "ToPrint") {
      if (jsob["Data"].isObject()) {
        auto f = [this](const Json::Value &v) {
          Json::FastWriter fw;
          this->_webSoc.sendTextMessage(QString::fromStdString(fw.write(v)));
        };
        auto json = std::make_shared<Json::Value>();
        *json = jsob["Data"];
        _print_msg_station->toPrint(json, _url.toString().toStdString(), f);
      }
    } else if (msgtype_str == "GetPrintedPage") {
      // oh?
      //_webSoc.sendTextMessage(QString::fromStdString(fw.write(_print_msg_station->)))

    } else if (msgtype_str == "GetWebsocketUrl") {
      // oh?

    } else if (msgtype_str == "InsertOrUpdateWebsocketUrl") {
      // oh?
    }
  }
}

void Websoc::slotOnConnect() { _timer.stop(); }

void Websoc::slotDisConnect() {
  _timer.start(100000);
  // _webSoc.open(url);
}
void Websoc::slotTimerReConnect() { _webSoc.open(_url); }

void Websoc::slotErr(QAbstractSocket::SocketError error) {
  _timer.start(100000);
}

void Websoc::slotSSLErr(const QList<QSslError> &errors) {}
