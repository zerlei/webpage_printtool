#ifndef PRINTMSGSTATION_H
#define PRINTMSGSTATION_H
#include "../data_sqlite/printdatabase.h"
#include "../net_interface/printwebsocket.h"
#include "webprinter.h"
#include <QObject>
#include <functional>
#include <json/value.h>
#include <memory>

class PrintMsgStation : QObject {
  Q_OBJECT
public:
  PrintMsgStation();

  const Json::Value getPrintInfo(bool isUpdate);
  const Json::Value addOnePrintConfig(std::shared_ptr<Json::Value> json);
  const Json::Value delOnePrintConfig(int Id);
  const Json::Value updateOnePrintConfig(std::shared_ptr<Json::Value> json);
  const Json::Value getPrintConfigs();

  void toPrint(std::shared_ptr<Json::Value> json, const std::string &,
               std::function<void(const Json::Value &)>);

  const Json::Value getPrintedPage(int size, int page);

  const Json::Value getScreenInfo();
  std::string getWebsocketUrl();
  bool insertOrUpdateWebsocketUrl(const std::string &);

  void setClientWebSockState(bool);

public:
  WebPrinter _print;
  PrintDatabase _db;
  bool _client_websoc_connected{false};

private:
  PrintWebSocket *_print_websoc;
};

#endif // PRINTMSGSTATION_H
