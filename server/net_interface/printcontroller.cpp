#include "printcontroller.h"
#include <string>
PrintController::PrintController(PrintMsgStation &printstation)
    : _printmsgstation(printstation) {}

void PrintController::TestConnect(
    const HttpRequestPtr &,
    std::function<void(const HttpResponsePtr &)> &&callback) {

  Json::Value t;
  t["😀"] = "😀";
  auto resp = HttpResponse::newHttpJsonResponse(t);
  resp->addHeader("Access-Control-Allow-Origin", "*");
  resp->addHeader("Access-Control-Allow-Methods", "GET, POST");
  resp->addHeader("Access-Control-Allow-Private-Network", "true");
  resp->addHeader("Access-Control-Allow-Credentials", "true");

  callback(resp);
}

void PrintController::GetPrintInfo(
    const HttpRequestPtr &request,
    std::function<void(const HttpResponsePtr &)> &&callback, bool &&isUpdate) {

  callback(ConfigResponse(_printmsgstation.getPrintInfo(isUpdate)));
}

void PrintController::AddOnePrintConfig(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) {
  auto value = std::make_shared<Json::Value>();
  Json::Reader reader;
  reader.parse(std::string(req->body()), *value);
  callback(ConfigResponse(_printmsgstation.addOnePrintConfig(value)));
}

void PrintController::DelOnePrintConfig(
    const HttpRequestPtr &,
    std::function<void(const HttpResponsePtr &)> &&callback, int &&Id) {
  callback(ConfigResponse(_printmsgstation.delOnePrintConfig(Id)));
}

void PrintController::UpdateOnePrintConfig(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) {

  auto value = std::make_shared<Json::Value>();
  Json::Reader reader;
  reader.parse(std::string(req->body()), *value);
  callback(ConfigResponse(_printmsgstation.updateOnePrintConfig(value)));
}

void PrintController::GetPrintConfigs(
    const HttpRequestPtr &,
    std::function<void(const HttpResponsePtr &)> &&callback) {

  callback(ConfigResponse(_printmsgstation.getPrintConfigs()));
}

void PrintController::ToPrint(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) {
  auto json = req->getJsonObject();
  auto v = req->getLocalAddr();
  //  v.toIp();

  auto f = [callback, this](const Json::Value &value) {
    callback(this->ConfigResponse(value));
  };
  auto value = std::make_shared<Json::Value>();
  Json::Reader reader;
  reader.parse(std::string(req->body()), *value);

  auto sourceip = req->getLocalAddr().toIp();
  _printmsgstation.toPrint(value, sourceip, f);
}
void PrintController::GetPrintedPage(
    const HttpRequestPtr &,
    std::function<void(const HttpResponsePtr &)> &&callback, int &&size,
    int &&page) {
  callback(ConfigResponse(_printmsgstation.getPrintedPage(size, page)));
}
void PrintController::GetScreenInfo(
    const HttpRequestPtr &,
    std::function<void(const HttpResponsePtr &)> &&callback) {
  callback(ConfigResponse(_printmsgstation.getScreenInfo()));
}
HttpResponsePtr PrintController::ConfigResponse(const Json::Value &value) {
  auto resp = HttpResponse::newHttpJsonResponse(value);
  resp->addHeader("Access-Control-Allow-Origin", "*");
  resp->addHeader("Access-Control-Allow-Methods", "GET, POST");
  resp->addHeader("Access-Control-Allow-Private-Network", "true");
  resp->addHeader("Access-Control-Allow-Credentials", "true");
  return resp;
}
