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
#include <string>
#include <time.h>
#include <tuple>
using namespace std::literals;
PrintMsgStation::PrintMsgStation() {}

const Json::Value PrintMsgStation::workWithString(std::string &str_) {
  try {
    Json::Value jsob;
    Json::Reader reader;
    if (reader.parse(str_, jsob)) {

      return workWithJson(jsob);

    } else {
      throw 1;
    }

  } catch (...) {
    Json::Value resp;
    resp["IsSuccess"] = false;
    return resp;
  }
}
const Json::Value PrintMsgStation::workWithJson(Json::Value &jsob) {

  try {
    auto msgtype = jsob["MsgType"];
    if (!msgtype.isString()) {
      throw 1;
    }
    auto msg_str = msgtype.asString();
    Json::Value resp;
    resp["Id"] = jsob["Id"];
    resp["IsSuccess"] = true;
    if (msg_str == "GetPrintInfo") {
      resp["Result"] = getPrintInfo(true);

    } else if (msg_str == "AddOnePrintConfig") {
      resp["Result"] = addOnePrintConfig(jsob["Data"]);

    } else if (msg_str == "DelOnePrintConfig") {
      resp["Result"] = delOnePrintConfig(jsob["Data"].asInt());
    } else if (msg_str == "UpdateOnePrintConfig") {
      resp["Result"] = updateOnePrintConfig(jsob["Data"]);
    } else if (msg_str == "GetPrintConfigs") {
      resp["Result"] = getPrintConfigs();

    } else if (msg_str == "ToPrint") {
      throw 1;

    } else if (msg_str == "GetPrintedPages") {
      resp["Result"] = getPrintedPage(jsob["Data"]["Size"].asInt(),
                                      jsob["Data"]["Page"].asInt());

    } else if (msg_str == "GetWebsocketUrl") {

      auto is_connected = false;
      if (_get_websoc_state) {
        is_connected = _get_websoc_state();
      }
      resp["Result"]["IsConnected"] = is_connected;
      resp["Result"]["WebSocUrl"] = getWebsocketUrl();
      ;

    } else if (msg_str == "InsertOrUpdateWebsocketUrl") {
      resp["IsSuccess"] = insertOrUpdateWebsocketUrl(jsob["Data"].asString());
    }
    return resp;

  } catch (...) {
    Json::Value resp;
    resp["IsSuccess"] = false;
    return resp;
  }
}
void PrintMsgStation::workWithStringAsync(
    std::string str_, std::string &ipinfo, std::string &from_type,
    std::function<void(const Json::Value &)> callback) {

  try {
    Json::Value jsob;
    Json::Reader reader;
    if (reader.parse(str_, jsob)) {
      jsob["IpInfo"] = ipinfo;
      jsob["FromType"] = from_type;

      workWithJsonAsync(jsob, callback);
    } else {
      throw 1;
    }

  } catch (...) {
    Json::Value resp;
    resp["IsSuccess"] = false;
    callback(resp);
  }
}
void PrintMsgStation::workWithJsonAsync(
    Json::Value jsob, std::function<void(const Json::Value &)> callback) {
  try {
    auto msgtype = jsob["MsgType"];
    if (!msgtype.isString()) {
      throw 1;
    }
    auto msg_str = msgtype.asString();
    Json::Value resp;
    resp["Id"] = jsob["Id"].asString();
    resp["IsSuccess"] = true;
    if (msg_str == "GetPrintInfo") {
      resp["Result"] = getPrintInfo(true);
      callback(resp);

    } else if (msg_str == "AddOnePrintConfig") {
      auto result = addOnePrintConfig(jsob["Data"]);
      resp["IsSuccess"] = result["IsSuccess"];
      resp["Message"] = result["Message"];
      callback(resp);

    } else if (msg_str == "DelOnePrintConfig") {
      auto result = delOnePrintConfig(jsob["Data"].asInt());
      resp["IsSuccess"] = result["IsSuccess"];
      resp["Message"] = result["Message"];
      callback(resp);

    } else if (msg_str == "UpdateOnePrintConfig") {
      auto result = updateOnePrintConfig(jsob["Data"]);
      resp["IsSuccess"] = result["IsSuccess"];
      resp["Message"] = result["Message"];
      callback(resp);

    } else if (msg_str == "GetPrintConfigs") {
      resp["Result"] = getPrintConfigs();
      callback(resp);

    } else if (msg_str == "ToPrint") {
      auto f = [resp, callback](const Json::Value &j) mutable {
        resp["Result"] = j;
        callback(resp);
      };
      toPrint(jsob["Data"], jsob["IpInfo"].asString(),
              jsob["FromType"].asString(), f);

    } else if (msg_str == "GetPrintedPages") {
      resp["Result"] = getPrintedPage(jsob["Data"]["Size"].asInt(),
                                      jsob["Data"]["Page"].asInt());
      callback(resp);

    } else if (msg_str == "GetWebsocketUrl") {
      auto is_connected = false;
      if (_get_websoc_state) {
        is_connected = _get_websoc_state();
      }
      resp["Result"]["IsConnected"] = is_connected;
      resp["Result"]["WebSocUrl"] = getWebsocketUrl();
      ;

      callback(resp);

    } else if (msg_str == "InsertOrUpdateWebsocketUrl") {
      resp["IsSuccess"] = insertOrUpdateWebsocketUrl(jsob["Data"].asString());
      callback(resp);
    }

  } catch (...) {
    Json::Value resp;
    resp["IsSuccess"] = false;
    resp["Id"] = jsob["Id"].asString();
    resp["Message"] = "数据格式是否错误？";
    callback(resp);
  }
}

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

const Json::Value PrintMsgStation::addOnePrintConfig(Json::Value &json) {
  Json::Value respValue;
  if (json) {
    auto [IsSuccess, Message] = _db.printerConfigInsert(json);
    respValue["IsSuccess"] = IsSuccess;
    respValue["Message"] = Message;
  } else {
    respValue["IsSuccess"] = false;
    respValue["Message"] = "需要可被解析的值!";
  }
  return respValue;
}

const Json::Value PrintMsgStation::delOnePrintConfig(int Id) {
  Json::Value respValue;
  auto [IsSuccess, Message] = _db.printerConfigDel(Id);
  respValue["IsSuccess"] = IsSuccess;
  respValue["Message"] = Message;
  return respValue;
}

const Json::Value PrintMsgStation::updateOnePrintConfig(Json::Value &json) {
  Json::Value respValue;
  if (json) {
    auto [IsSuccess, Message] = _db.printerConfigUpdate(json);
    respValue["IsSuccess"] = IsSuccess;
    respValue["Message"] = Message;
  } else {
    respValue["IsSuccess"] = false;
    respValue["Message"] = "需要可被解析的值!";
  }
  return respValue;
}

const Json::Value PrintMsgStation::getPrintConfigs() {
  Json::Value vs;
  auto v = _db.printerConfigQueryById();
  for (PrinterConfig &x : _db.printerConfigQueryById()) {
    vs.append(x.getReflectionJson());
  }
  return vs;
}

void PrintMsgStation::toPrint(
    Json::Value &json, const std::string &ipinfo_,
    const std::string &from_type_,
    std::function<void(const Json::Value &)> callback) {
  auto list = std::make_shared<Json::Value>((json));

  auto size = list->size();
  if (size == 0) {
    Json::Value v;
    Json::Value errValue;
    errValue["IsSuccess"] = false;
    errValue["Message"] = "错误的数据格式";
    v.append(errValue);
    callback(v);
    return;
  }
  auto i = std::make_shared<int>();
  *i = 0;
  auto respValue = std::make_shared<Json::Value>();
  auto f = [callback, size, i, respValue, list, ipinfo_, from_type_,
            this](bool IsSuccess, const QString &Message) {
    Json::Value subValue;
    subValue["IsSuccess"] = IsSuccess;
    subValue["Message"] = Message.toStdString();
    respValue->append(subValue);
    PrintedPage page;
    page.IsSuccess = IsSuccess;
    page.PrintTime =
        std::format("{0:%F} {0:%T}", std::chrono::system_clock::now()+8h);
    page.FromIp = ipinfo_;
    page.FromType = from_type_;
    page.PageName = (*list)[*i]["PageUrl"].asString();
    page.ConfigName = (*list)[*i]["ConfigName"].asString();
    page.PrintMode = (*list)[*i]["PrintMode"].asString();
    _db.printedPageInsert(page);
    if (this->_websoc_msg_push) {
      Json::Value v;
      v["MsgType"] = "PrintPageChanged";
      _websoc_msg_push(v);
    }

    *i = *i + 1;
    if (*i == size) {
      callback(*respValue);
    }
  };

  for (auto &x : *list) {

    try {
      const auto url = QString::fromStdString(x["PageUrl"].asString());
      auto mode = x["PrintMode"].asString() == "LoadAchieve"
                      ? PrintModel::LoadAchieve
                      : PrintModel::JsPrintRequest;
      auto Name = x["ConfigName"].asString();

      auto printer_config =
          _db.printerConfigQueryByName(QString::fromStdString(Name));

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

        _print.addPrintWebPageToQueue(QUrl(url), PrinterInfoName, mode,
                                      PageName, margins, Orientation_, f);
        emit _print.signalGuiThreadToWork();
      }

    } catch (...) {
      f(false, "不合格的数据格式！");
    }
  }
}

const Json::Value PrintMsgStation::getPrintedPage(int size_, int page_) {
  Json::Value vs;

  auto [Count, Rows] = _db.printedPageQuery(size_, page_);
  for (auto &x : Rows) {
    vs["Rows"].append(x.getReflectionJson());
  }
  vs["Count"] = Count;
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

  auto IsSuceess = _db.insertOrUpdateWebsocketUrl(websoc_url_);
  if (IsSuceess) {
    if (_set_websoc_url) {
      _set_websoc_url(websoc_url_);
    }
  }
  return IsSuceess;
}

void PrintMsgStation::setClientWebSockState(bool is_) {

  if (_websoc_msg_push) {
    Json::Value v;
    v["Id"] = "😀";
    v["MsgType"] = "WebSocState";
    v["WebSocUrl"] = _db.getWebsocketUrl();
    v["WebsocConnected"] = is_;
    _websoc_msg_push(v);
  }
}
