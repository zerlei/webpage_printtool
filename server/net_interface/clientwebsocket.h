#ifndef SLWEBSOC_H
#define SLWEBSOC_H
#include "../printer/printmsgstation.h"
#include <QObject>
#include <QTimer>
#include <QWebSocket>
#include <QWidget>
#include <functional>
#include <qtimer.h>
#include <qtmetamacros.h>
#include <qurl.h>


class ClientWebsoc : public QObject {
  Q_OBJECT
public:
  ClientWebsoc(PrintMsgStation &printmsgstation);

  void openUrl(const QUrl &url);

private:
  QTimer _timer;
  QWebSocket _webSoc;
  QUrl _url;
  PrintMsgStation &_print_msg_station;

private:
std::string JsonValueToString(const Json::Value &);

Q_SIGNALS:
void signalsetWebSocUrl(const QString url);
private slots:
  void slotTimerReConnect();
  void slotOpenUrl(const QString url_str);

  void slotReceiveMsg(const QString &message);
  void slotOnConnect();
  void slotDisConnect();
  void slotErr(QAbstractSocket::SocketError error);
  void slotSSLErr(const QList<QSslError> &errors);
};

#endif // SLWEBSOC_H
