#ifndef PRINTCONTROLLER_H
#define PRINTCONTROLLER_H


#include "webprinter.h"
#include <drogon/HttpController.h>
#include <drogon/HttpTypes.h>
#include <drogon/utils/FunctionTraits.h>
#include <functional>
using namespace drogon;


class PrintController :public HttpController<PrintController, false>
{
public:
  PrintController(WebPrinter& print);

  METHOD_LIST_BEGIN
  METHOD_ADD(PrintController::Test, "/test", Get);
  METHOD_ADD(PrintController::GetPrintInfo, "/getprintinfo?isUpdate={1}", Get);

  METHOD_ADD(PrintController::AddOnePrintConfig, "/addoneprintconfig", Post);
  METHOD_ADD(PrintController::DelOnePrintConfig, "/deloneprintconfig", Get);
  METHOD_ADD(PrintController::UpdateOnePrintConfig, "/updateoneprintconfig", Post);
  METHOD_ADD(PrintController::GetPrintConfigs, "/getprintconfigs", Get);
  METHOD_ADD(PrintController::ToPrint, "/toprint", Post);
  METHOD_LIST_END

protected:

  void Test(const HttpRequestPtr&,std::function<void(const HttpResponsePtr&)>&& callback);



  void GetPrintInfo(const HttpRequestPtr&,std::function<void(const HttpResponsePtr&)>&& callback,bool&& isUpdate);



  void AddOnePrintConfig(const HttpRequestPtr&,std::function<void(const HttpResponsePtr&)>&& callback);

  void DelOnePrintConfig(const HttpRequestPtr&,std::function<void(const HttpResponsePtr&)>&& callback);

  void UpdateOnePrintConfig(const HttpRequestPtr&,std::function<void(const HttpResponsePtr&)>&& callback);

  void GetPrintConfigs(const HttpRequestPtr&,std::function<void(const HttpResponsePtr&)>&& callback);



  void ToPrint(const HttpRequestPtr&,std::function<void(const HttpResponsePtr&)>&& callback);





private:
  WebPrinter& _print;
};

#endif // PRINTCONTROLLER_H
