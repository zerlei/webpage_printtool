#include "printcontroller.h"
#include <string>
#include <QTextCodec>
PrintController::PrintController(WebPrinter& print):_print(print)
{

}

void PrintController::Test(const HttpRequestPtr &, std::function<void (const HttpResponsePtr &)> &&callback)
{


  Json::Value t;
  t["chi"] = u8"吃";
  auto response = HttpResponse::newHttpJsonResponse(t);
  response->addHeader("Access-Control-Allow-Origin","*");

  callback(response);

}

void PrintController::GetPrintInfo(const HttpRequestPtr &request,
                                   std::function<void (const HttpResponsePtr &)> &&callback,
                                   bool&& isUpdate
                                   )
{

  if(isUpdate) {
      _print.UpdatePrinterInfo();
    }

    Json::Value PrinterInfoS;
  for(auto& x:_print.GetAvaliablePrinterInfo()) {
       Json::Value PrinterInfo;

//       QTextCodec* Codec =QTextCodec::codecForName("UTF-8");


       PrinterInfo["name"] = QString(x.printerName()).toUtf8().toStdString();

       //qDebug()<<QString(x.printerName()).toUtf8();
       Json::Value PrinterInfoPages;
       for(auto& y:x.supportedPageSizes()) {

           Json::Value PrinterInfoPage;
           PrinterInfoPage["name"] = y.name().toUtf8().toStdString();
           PrinterInfoPage["key"] = y.key().toUtf8().toStdString();
           PrinterInfoPage["id"] = y.id();
           auto mmSize = y.size(QPageSize::Millimeter);
           QString size = QString("%1x%2mm").arg(mmSize.width()).arg(mmSize.height());
           PrinterInfoPage["size"] = size.toStdString();
           PrinterInfoPages.append(PrinterInfoPage);
        }
       PrinterInfo["papers"] = PrinterInfoPages;
       PrinterInfoS.append(PrinterInfo);
    }
  auto resp = HttpResponse::newHttpJsonResponse(PrinterInfoS);
  resp->addHeader("Access-Control-Allow-Origin","*");
  resp->addHeader("Access-Control-Allow-Methods", "GET, POST");
  resp->addHeader("Access-Control-Allow-Private-Network","true");

  resp->addHeader("Access-Control-Allow-Credentials","true");


  callback(resp);
}

void PrintController::AddOnePrintConfig(const HttpRequestPtr &, std::function<void (const HttpResponsePtr &)> &&callback)
{

}

void PrintController::DelOnePrintConfig(const HttpRequestPtr &, std::function<void (const HttpResponsePtr &)> &&callback)
{

}

void PrintController::UpdateOnePrintConfig(const HttpRequestPtr &, std::function<void (const HttpResponsePtr &)> &&callback)
{

}

void PrintController::GetPrintConfigs(const HttpRequestPtr &, std::function<void (const HttpResponsePtr &)> &&callback)
{

}

void PrintController::ToPrint(const HttpRequestPtr &, std::function<void (const HttpResponsePtr &)> &&callback)
{

}
