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

void Websoc::SlotReceiveMsg(const QString &message) {}

void Websoc::SlotOnConnect() {}

void Websoc::SlotDisConnect() {

  // _webSoc.open(url);
}

void Websoc::SlotErr(QAbstractSocket::SocketError error) {}

void Websoc::SlotSSLErr(const QList<QSslError> &errors) {}
