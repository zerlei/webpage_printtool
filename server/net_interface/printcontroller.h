#ifndef PRINTCONTROLLER_H
#define PRINTCONTROLLER_H

#include "../printer/printmsgstation.h"
#include <drogon/HttpController.h>
#include <drogon/HttpTypes.h>
#include <drogon/utils/FunctionTraits.h>
#include <functional>
using namespace drogon;

class PrintController : public HttpController<PrintController, false> {
public:
  PrintController(PrintMsgStation &printmsgstation);

  METHOD_LIST_BEGIN
  METHOD_ADD(PrintController::TestConnect, "/testConnect", Get);
  METHOD_ADD(PrintController::Print, "/Print", Post);

  METHOD_LIST_END

protected:
  void TestConnect(const HttpRequestPtr &,
                   std::function<void(const HttpResponsePtr &)> &&callback);

  void Print(const HttpRequestPtr &,
                   std::function<void(const HttpResponsePtr &)> &&callback);

  

private:
  HttpResponsePtr ConfigResponse(const Json::Value &value);
  PrintMsgStation &_printmsgstation;
};

#endif // PRINTCONTROLLER_H
