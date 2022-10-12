#include "websoc.h"
#include "printmsgstation.h"
#include <json/json.h>
#include <json/value.h>
#include <json/writer.h>
#include <memory>

Websoc::Websoc(PrintMsgStation *printmsgstation)
    : _print_msg_station(printmsgstation) {

  _webSoc.ignoreSslErrors();
  connect(&_webSoc, &QWebSocket::connected, this, &Websoc::SlotOnConnect);
  connect(&_webSoc, &QWebSocket::disconnected, this, &Websoc::SlotDisConnect);
  connect(&_webSoc,
          QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), this,
          &Websoc::SlotErr);
  connect(&_webSoc, &QWebSocket::textMessageReceived, this,
          &Websoc::SlotReceiveMsg);
}

void Websoc::OpenUrl(const QUrl &url) {
  _webSoc.close(QWebSocketProtocol::CloseCodeNormal);

  // if(_webSoc.is)
}

void Websoc::SlotReceiveMsg(const QString &message) {

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
          fw.write(_print_msg_station->GetPrintInfo(true))));

    } else if (msgtype_str == "AddOnePrintConfig") {

      if (jsob["Data"].isObject()) {
         auto json = std::make_shared<Json::Value>();
        *json = jsob["Data"];
        _webSoc.sendTextMessage(QString::fromStdString(
            fw.write(_print_msg_station->AddOnePrintConfig(json))));
      }

    } else if (msgtype_str == "DelOnePrintConfig") {

      if (jsob["Data"].isInt()) {
        _webSoc.sendTextMessage(QString::fromStdString(fw.write(
            _print_msg_station->DelOnePrintConfig(jsob["Data"].asInt()))));
      }

    } else if (msgtype_str == "UpdateOnePrintConfig") {

      if (jsob["Data"].isObject()) {
        auto json = std::make_shared<Json::Value>();
        *json = jsob["Data"];
        _webSoc.sendTextMessage(QString::fromStdString(
            fw.write(_print_msg_station->UpdateOnePrintConfig(json))));
      }

    } else if (msgtype_str == "GetPrintConfigs") {
      _webSoc.sendTextMessage(QString::fromStdString(
          fw.write(_print_msg_station->GetPrintConfigs())));

    } else if (msgtype_str == "ToPrint") {

      if (jsob["Data"].isObject()) {
        auto f = [this](const Json::Value &v) {
          Json::FastWriter fw;
          this->_webSoc.sendTextMessage(QString::fromStdString(fw.write(v)));
        };
        auto json = std::make_shared<Json::Value>();
        *json = jsob["Data"];
        _print_msg_station->ToPrint(json,
                                    f);
      }
    }
  }
}

void Websoc::SlotOnConnect() {}

void Websoc::SlotDisConnect() {

  // _webSoc.open(url);
}

void Websoc::SlotErr(QAbstractSocket::SocketError error) {}

void Websoc::SlotSSLErr(const QList<QSslError> &errors) {}
