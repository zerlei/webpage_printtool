#ifndef SLWEBSOC_H
#define SLWEBSOC_H
#include <QObject>
#include <QWebSocket>
#include <functional>
#include <QWidget>

class SLWebsoc  : public QObject
{
    Q_OBJECT
public:
    SLWebsoc();

    void OpenUrl(const  QUrl& url);
private:
    QWebSocket _webSoc;
    QUrl _url;

private slots:
    void SlotReceiveMsg(const QString& message);
    void SlotOnConnect();
    void SlotDisConnect();
    void SlotErr(QAbstractSocket::SocketError error);
    void SlotSSLErr(const QList<QSslError>& errors);
};

#endif // SLWEBSOC_H
