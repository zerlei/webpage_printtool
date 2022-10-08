#include "slwebsoc.h"

SLWebsoc::SLWebsoc()
{

    _webSoc.ignoreSslErrors();
    connect(&_webSoc, &QWebSocket::connected, this, &SLWebsoc::SlotOnConnect);
    connect(&_webSoc, &QWebSocket::disconnected, this, &SLWebsoc::SlotDisConnect);
    connect(&_webSoc, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), this, &SLWebsoc::SlotErr);
    connect(&_webSoc, &QWebSocket::textMessageReceived, this, &SLWebsoc::SlotReceiveMsg);


}

void SLWebsoc::OpenUrl(const QUrl& url)
{
    _webSoc.close(QWebSocketProtocol::CloseCodeNormal);

    if(_webSoc.is)

}

void SLWebsoc::SlotReceiveMsg(const QString &message)
{

}

void SLWebsoc::SlotOnConnect()
{

}

void SLWebsoc::SlotDisConnect()
{

    _webSoc.open(url);

}

void SLWebsoc::SlotErr(QAbstractSocket::SocketError error)
{

}

void SLWebsoc::SlotSSLErr(const QList<QSslError> &errors)
{

}
