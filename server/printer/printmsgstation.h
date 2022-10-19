#ifndef PRINTMSGSTATION_H
#define PRINTMSGSTATION_H
#include "printdatabase.h"
#include "webprinter.h"
#include <QObject>
#include <functional>
#include <json/value.h>
#include <memory>
class PrintMsgStation : QObject {
  Q_OBJECT
public:
  PrintMsgStation();

  const Json::Value GetPrintInfo(bool isUpdate);
  const Json::Value AddOnePrintConfig(std::shared_ptr<Json::Value> json);
  const Json::Value DelOnePrintConfig(int Id);
  const Json::Value UpdateOnePrintConfig(std::shared_ptr<Json::Value> json);
  const Json::Value GetPrintConfigs();

  void ToPrint(std::shared_ptr<Json::Value> json,
               std::function<void(const Json::Value &)>);
  const Json::Value GetScreenInfo();

  const Json::Value GetPrintedPage(int size,int page);

public:
  WebPrinter _print;
  PrintDatabase _db;

private:
};

#endif // PRINTMSGSTATION_H
