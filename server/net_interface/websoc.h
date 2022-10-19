#ifndef SLWEBSOC_H
#define SLWEBSOC_H
#include <QObject>
#include <QWebSocket>
#include <QWidget>
#include <functional>


class PrintMsgStation;
class Websoc : public QObject {
  Q_OBJECT
public:
  Websoc(PrintMsgStation* printmsgstation);

  void OpenUrl(const QUrl &url);

private:
  QWebSocket _webSoc;
  QUrl _url;
  PrintMsgStation* _print_msg_station;

private slots:
  void SlotReceiveMsg(const QString &message);
  void SlotOnConnect();
  void SlotDisConnect();
  void SlotErr(QAbstractSocket::SocketError error);
  void SlotSSLErr(const QList<QSslError> &errors);
};

#endif // SLWEBSOC_H
