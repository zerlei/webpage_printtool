#ifndef PRINTMSGSTATION_H
#define PRINTMSGSTATION_H
#include "../data_sqlite/printdatabase.h"
#include "webprinter.h"
#include <QObject>
#include <functional>
#include <json/value.h>
#include <memory>
#include <string>
#include <tuple>
class PrintMsgStation : QObject {
  Q_OBJECT
public:
  PrintMsgStation();

public:
  void workWithStringAsync(std::string str_,std::string& ipinfo,std::string & from_type, std::function<void(const Json::Value &)>);
  void workWithJsonAsync(Json::Value _v,std::function<void(const Json::Value &)>);

  const Json::Value getPrintInfo(bool isUpdate);
  const Json::Value addOnePrintConfig(Json::Value& json);
  const Json::Value delOnePrintConfig(int Id);
  const Json::Value updateOnePrintConfig(Json::Value& json);
  const Json::Value getPrintConfigs();

  void toPrint(Json::Value& json, const std::string &,const std::string&,
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


  std::function<void(const Json::Value&)> _websoc_msg_push{nullptr};

  std::function<void(const std::string& )> _set_websoc_url{nullptr};

  std::function<bool()> _get_websoc_state{nullptr};
};

#endif // PRINTMSGSTATION_H
