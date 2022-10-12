#include "printmsgstation.h"
#include <QGuiApplication>
#include <QScreen>

PrintMsgStation::PrintMsgStation() {}

const Json::Value PrintMsgStation::GetPrintInfo(bool isUpdate) {
  if (isUpdate) {
    _print.UpdatePrinterInfo();
  }
  Json::Value PrinterInfoS;
  for (auto &x : _print.GetAvaliablePrinterInfo()) {
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
          QString("%1x%2mm").arg(mmSize.width()).arg(mmSize.height());
      PrinterInfoPage["PaperSize"] = size.toStdString();
      PrinterInfoPages.append(PrinterInfoPage);
    }
    PrinterInfo["Papers"] = PrinterInfoPages;
    PrinterInfoS.append(PrinterInfo);
  }

  return PrinterInfoS;
}

const Json::Value
PrintMsgStation::AddOnePrintConfig(std::shared_ptr<Json::Value> json) {
  Json::Value respValue;
  if (json) {
    auto [isSuccess, message] = _db.Insert(*json);
    respValue["isSuccess"] = isSuccess;
    respValue["message"] = message;
  } else {
    respValue["isSuccess"] = false;
    respValue["message"] = "需要可被解析的值!";
  }
  return respValue;
}

const Json::Value PrintMsgStation::DelOnePrintConfig(int Id) {
  Json::Value respValue;
  auto [isSuccess, message] = _db.Del(Id);
  respValue["isSuccess"] = isSuccess;
  respValue["message"] = message;
  return respValue;
}

const Json::Value
PrintMsgStation::UpdateOnePrintConfig(std::shared_ptr<Json::Value> json) {
  Json::Value respValue;
  if (json) {
    auto [isSuccess, message] = _db.Update(*json);
    respValue["isSuccess"] = isSuccess;
    respValue["message"] = message;
  } else {
    respValue["isSuccess"] = false;
    respValue["message"] = "需要可被解析的值!";
  }
  return respValue;
}

const Json::Value PrintMsgStation::GetPrintConfigs() { return _db.Query(); }

void PrintMsgStation::ToPrint(
    std::shared_ptr<Json::Value> json,
    std::function<void(const Json::Value &)> callback) {
  auto list = (*json)["WebPages"];

  auto size = list.size();
  if (size == 0) {
    Json::Value v;
    Json::Value errValue;
    errValue["isSuccess"] = false;
    errValue["message"] = "你传的是什么";
    v.append(errValue);

    callback(v);
    return;
  }
  auto i = std::make_shared<int>();
  *i = 0;
  auto respValue = std::make_shared<Json::Value>();
  auto f = [callback, size, i, respValue](bool isSuccess,
                                          const QString &message) {
    Json::Value subValue;
    subValue["isSuccess"] = isSuccess;
    subValue["message"] = message.toStdString();
    respValue->append(subValue);
    *i = *i + 1;
    if (*i == size) {
      callback(*respValue);
    }
  };

  for (auto &x : list) {

    const auto url = QString::fromStdString(x["Url"].asString());
    auto mode = x["PrintMode"].asString() == "LoadAchieve"
                    ? PrintModel::LoadAchieve
                    : PrintModel::JsPrintRequest;
    auto Name = x["Name"].asString();

    const Json::Value value = _db.QueryByName(QString::fromStdString(Name));

    if (value.empty()) {
      f(false, "找不到对应的打印机配置~");
    } else {
      auto PrinterInfoName =
          QString::fromStdString(value[0]["PrinterName"].asString());

      auto PageName = QString::fromStdString(value[0]["PaperName"].asString());

      auto TopMargin = value[0]["TopMargin"].asDouble();
      auto BottomMargin = value[0]["BottomMargin"].asDouble();
      auto LeftMargin = value[0]["LeftMargin"].asDouble();
      auto RightMargin = value[0]["RightMargin"].asDouble();

      QMarginsF margins(LeftMargin, TopMargin, RightMargin, BottomMargin);

      QPageLayout::Orientation Orientation_;

      auto Orientation = value[0]["Orientation"].asString();
      if (Orientation == "横向") {
        Orientation_ = QPageLayout::Portrait;
      } else {
        Orientation_ = QPageLayout::Landscape;
      }

      _print.AddPrintWebPageToQueue(QUrl(url), PrinterInfoName, mode, PageName,
                                    margins, Orientation_, f);
      emit _print.signalGuiThreadToWork();
    }
  }
}

const Json::Value PrintMsgStation::GetScreenInfo() {
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
  //  const auto size = x->size();
  //  auto X = size.width() / x->physicalDotsPerInchX();
  //  auto Y = size.height() / x->physicalDotsPerInchY();

  //  qDebug() << "显示器的英寸X：" << X;
  //  qDebug() << "显示器的英寸Y：" << Y;
  //  qDebug() << "显示器的英寸对角线：" << std::sqrt(X * X + Y * Y);

  return v;
}
