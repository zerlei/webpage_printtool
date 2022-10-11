#include "websoc.h"
#include "printmsgstation.h"

Websoc::Websoc(PrintMsgStation* printmsgstation)
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
  if(reader.parse(message.str(),jsob)) {

    auto msgtype = jsob["msgtype"]
  if(msgtype.isNull()) {
    return
  }

  auto msgtype_str = msgtype.toString();

  if(msgtype_str == "GetPrintInfo") {

  } else if(msgtype_str == "AddOnePrintConfig") {

  } else if(msgtype_str == "DelOnePrintConfig") {

  } else if(msgtype_str == "UpdateOnePrintConfig") {

  } else if(msgtype_str == "GetPrintConfigs") {

  } else if (msgtype_str == "ToPrint") {
    
  }

  }


  QJsonObject jsob = QJsonDocument::fromJson(Msg.toUtf8()).object();

  
  



}

void Websoc::SlotOnConnect() {}

void Websoc::SlotDisConnect() {

  // _webSoc.open(url);
}

void Websoc::SlotErr(QAbstractSocket::SocketError error) {}

void Websoc::SlotSSLErr(const QList<QSslError> &errors) {}
