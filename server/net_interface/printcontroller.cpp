#include "printcontroller.h"
#include "net_interface/printcontroller.h"
#include <json/value.h>
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
void PrintController::Print(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback) {
  auto f = [callback, this](const Json::Value &value) {
    callback(this->ConfigResponse(value));
  };
  auto str = std::string(req->body());
  _printmsgstation.workWithStringAsync(str, f);
}
HttpResponsePtr PrintController::ConfigResponse(const Json::Value &value) {
  auto resp = HttpResponse::newHttpJsonResponse(value);
  resp->addHeader("Access-Control-Allow-Origin", "*");
  resp->addHeader("Access-Control-Allow-Methods", "GET, POST");
  resp->addHeader("Access-Control-Allow-Private-Network", "true");
  resp->addHeader("Access-Control-Allow-Credentials", "true");
  return resp;
}
