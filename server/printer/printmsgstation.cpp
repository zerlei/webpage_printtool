#include "printmsgstation.h"
#include "base.h"
#include <QGuiApplication>
#include <QScreen>
#include <chrono>
#include <drogon/HttpRequest.h>
#include <format>
#include <iomanip>
#include <json/value.h>
#include <memory>
#include <time.h>

PrintMsgStation::PrintMsgStation() {}

const Json::Value PrintMsgStation::getPrintInfo(bool isUpdate) {
  if (isUpdate) {
    _print.updatePrinterInfo();
  }
  Json::Value PrinterInfoS;
  for (auto &x : _print.getAvaliablePrinterInfo()) {
    Json::Value PrinterInfo;
    PrinterInfo["PrinterName"] =
        QString(x.printerName()).toUtf8().toStdString();

    // qDebug()<<QString(x.printerName()).toUtf8();
    Json::Value PrinterInfoPages;
    for (auto &y : x.supportedPageSizes()) {
      Json::Value PrinterInfoPage;
      PrinterInfoPage["PaperName"] = y.name().toUtf8().toStdString();
      PrinterInfoPage["Key"] = y.key().toUtf8().toStdString();
      PrinterInfoPage["Id"] = y.id();
      auto mmSize = y.size(QPageSize::Millimeter);
      QString size =
          QString("%1_%2mm").arg(mmSize.width()).arg(mmSize.height());
      PrinterInfoPage["PaperSize"] = size.toStdString();
      PrinterInfoPages.append(PrinterInfoPage);
    }
    PrinterInfo["Papers"] = PrinterInfoPages;
    PrinterInfoS.append(PrinterInfo);
  }

  return PrinterInfoS;
}

const Json::Value
PrintMsgStation::addOnePrintConfig(std::shared_ptr<Json::Value> json) {
  Json::Value respValue;
  if (json) {
    auto [isSuccess, message] = _db.printerConfigInsert(*json);
    respValue["isSuccess"] = isSuccess;
    respValue["message"] = message;
  } else {
    respValue["isSuccess"] = false;
    respValue["message"] = "需要可被解析的值!";
  }
  return respValue;
}

const Json::Value PrintMsgStation::delOnePrintConfig(int Id) {
  Json::Value respValue;
  auto [isSuccess, message] = _db.printerConfigDel(Id);
  respValue["isSuccess"] = isSuccess;
  respValue["message"] = message;
  return respValue;
}

const Json::Value
PrintMsgStation::updateOnePrintConfig(std::shared_ptr<Json::Value> json) {
  Json::Value respValue;
  if (json) {
    auto [isSuccess, message] = _db.printerConfigUpdate(*json);
    respValue["isSuccess"] = isSuccess;
    respValue["message"] = message;
  } else {
    respValue["isSuccess"] = false;
    respValue["message"] = "需要可被解析的值!";
  }
  return respValue;
}

const Json::Value PrintMsgStation::getPrintConfigs() {
  Json::Value vs;
  for (PrinterConfig &x : _db.printerConfigQueryById()) {
    vs.append(x.getReflectionJson());
  }
  return vs;
}

void PrintMsgStation::toPrint(
    std::shared_ptr<Json::Value> json, const std::string &ipinfo_,
    std::function<void(const Json::Value &)> callback) {
  auto list = std::make_shared<Json::Value>((*json)["WebPages"]);

  auto size = list->size();
  if (size == 0) {
    Json::Value v;
    Json::Value errValue;
    errValue["isSuccess"] = false;
    errValue["message"] = "错误的数据格式";
    v.append(errValue);
    callback(v);
    return;
  }
  auto i = std::make_shared<int>();
  *i = 0;
  auto respValue = std::make_shared<Json::Value>();
  auto f = [callback, size, i, respValue, list, ipinfo_,
            this](bool isSuccess, const QString &message) {
    Json::Value subValue;
    subValue["isSuccess"] = isSuccess;
    subValue["message"] = message.toStdString();
    respValue->append(subValue);
    PrintedPage page;
    page.IsSuccess = isSuccess;
    page.PrintTime =
        std::format("{0:%F} {0:%T}", std::chrono::system_clock::now());
    page.FromIp = ipinfo_;
    page.FromType = "HttpServer";
    page.PageName = (*list)[*i]["Url"].asString();
    page.ConfigId = (*list)[*i]["Id"].asInt();
    page.PrintMode = (*list)[*i]["PrintMode"].asString();
    _db.printedPageInsert(page);

    *i = *i + 1;
    if (*i == size) {
      callback(*respValue);
    }
  };

  for (auto &x : *list) {

    const auto url = QString::fromStdString(x["Url"].asString());
    auto mode = x["PrintMode"].asString() == "LoadAchieve"
                    ? PrintModel::LoadAchieve
                    : PrintModel::JsPrintRequest;
    auto Id = x["Id"].asInt();

    auto printer_config = _db.printerConfigQueryById(Id);

    if (printer_config.empty()) {
      f(false, "找不到对应的打印机配置~");
    } else {
      auto PrinterInfoName =
          QString::fromStdString(printer_config[0].PrinterName);

      auto PageName = QString::fromStdString(printer_config[0].PaperName);

      auto TopMargin = printer_config[0].TopMargin;
      auto BottomMargin = printer_config[0].BottomMargin;
      auto LeftMargin = printer_config[0].LeftMargin;
      auto RightMargin = printer_config[0].RightMargin;

      QMarginsF margins(LeftMargin, TopMargin, RightMargin, BottomMargin);

      QPageLayout::Orientation Orientation_;

      auto Orientation = printer_config[0].Orientation;
      if (Orientation == "横向") {
        Orientation_ = QPageLayout::Portrait;
      } else {
        Orientation_ = QPageLayout::Landscape;
      }

      _print.addPrintWebPageToQueue(QUrl(url), PrinterInfoName, mode, PageName,
                                    margins, Orientation_, f);
      emit _print.signalGuiThreadToWork();
    }
  }
}

const Json::Value PrintMsgStation::getPrintedPage(int size_, int page_) {
  Json::Value vs;
  for (auto &x : _db.printedPageQuery(size_, page_)) {
    vs.append(x.getReflectionJson());
  }
  return vs;
}

const Json::Value PrintMsgStation::getScreenInfo() {
  Json::Value v;
  for (auto &x : QGuiApplication::screens()) {
    Json::Value subV;
    subV["physicalDotsPerInchX"] = x->physicalDotsPerInchX();
    subV["physicalDotsPerInchY"] = x->physicalDotsPerInchY();
    const auto size = x->size();
    subV["width"] = size.width();
    subV["height"] = size.height();
    v.append(subV);
  }
  return v;
}
std::string PrintMsgStation::getWebsocketUrl() { return _db.getWebsocketUrl(); }
bool PrintMsgStation::insertOrUpdateWebsocketUrl(
    const std::string &websoc_url_) {
  return _db.insertOrUpdateWebsocketUrl(websoc_url_);
}

void PrintMsgStation::setClientWebSockState(bool is_) {
  _client_websoc_connected = is_;
  Json::Value v;
  v["MsgTyle"] = "WebSoc";
  v["WebsocConnected"] = is_;
  auto msg = _print_websoc->JsonValueToString(v);
  for (auto &_ : _print_websoc->_webconnections) {
    _->send(msg);
  }
}
