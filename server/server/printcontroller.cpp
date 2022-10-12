#include "printcontroller.h"
#include <string>
PrintController::PrintController(PrintMsgStation &printstation)
    : _printmsgstation(printstation) {}

void PrintController::Test(
    const HttpRequestPtr &,
    std::function<void(const HttpResponsePtr &)> &&callback) {

  Json::Value t;
  t["hi"] = "hi";
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

  callback(ConfigResponse(_printmsgstation.GetPrintInfo(isUpdate)));
}

void PrintController::AddOnePrintConfig(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) {
  auto value = std::make_shared<Json::Value>();
  Json::Reader reader;
  reader.parse(std::string(req->body()), *value);
  callback(ConfigResponse(_printmsgstation.AddOnePrintConfig(value)));
}

void PrintController::DelOnePrintConfig(
    const HttpRequestPtr &,
    std::function<void(const HttpResponsePtr &)> &&callback, int &&Id) {
  callback(ConfigResponse(_printmsgstation.DelOnePrintConfig(Id)));
}

void PrintController::UpdateOnePrintConfig(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) {

  auto value = std::make_shared<Json::Value>();
  Json::Reader reader;
  reader.parse(std::string(req->body()), *value);
  callback(ConfigResponse(_printmsgstation.UpdateOnePrintConfig(value)));
}

void PrintController::GetPrintConfigs(
    const HttpRequestPtr &,
    std::function<void(const HttpResponsePtr &)> &&callback) {

  callback(ConfigResponse(_printmsgstation.GetPrintConfigs()));
}

void PrintController::ToPrint(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) {
  auto json = req->getJsonObject();

  auto f = [callback, this](const Json::Value &value) {
    callback(this->ConfigResponse(value));
  };
  auto value = std::make_shared<Json::Value>();
  Json::Reader reader;
  reader.parse(std::string(req->body()), *value);

  _printmsgstation.ToPrint(value, f);
}

void PrintController::GetScreenInfo(
    const HttpRequestPtr &,
    std::function<void(const HttpResponsePtr &)> &&callback) {
  callback(ConfigResponse(_printmsgstation.GetScreenInfo()));
}

HttpResponsePtr PrintController::ConfigResponse(const Json::Value &value) {
  auto resp = HttpResponse::newHttpJsonResponse(value);
  resp->addHeader("Access-Control-Allow-Origin", "*");
  resp->addHeader("Access-Control-Allow-Methods", "GET, POST");
  resp->addHeader("Access-Control-Allow-Private-Network", "true");
  resp->addHeader("Access-Control-Allow-Credentials", "true");
  return resp;
}
