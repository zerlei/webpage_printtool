#include "printwebsocket.h"
#include <json/value.h>

PrintWebSocket::PrintWebSocket(PrintMsgStation &print_msg_station)
    : _printmsgstation(print_msg_station) {
      auto f = [this](const Json::Value& v) {
        auto msg = this->JsonValueToString(v);
        for(auto & x : this->_webconnections) {
          x->send(msg);
        }
      };
      _printmsgstation._websoc_msg_push = f;
    }

void PrintWebSocket::handleNewMessage(
    const WebSocketConnectionPtr &webconnection, std::string &&str,
    const WebSocketMessageType &) {
  auto f = [this, webconnection](const Json::Value &value) {
    webconnection->send(this->JsonValueToString(value));
  };
  std::string ip = webconnection->localAddr().toIp().c_str();

  std::string fromtype = "ServerWebsocket";
  _printmsgstation.workWithStringAsync(str,ip,fromtype, f);
}

void PrintWebSocket::handleNewConnection(
    const HttpRequestPtr &, const WebSocketConnectionPtr &webconnection) {
  _webconnections.push_back(webconnection);
}

void PrintWebSocket::handleConnectionClosed(
    const WebSocketConnectionPtr &webconnection) {
  _webconnections.remove(webconnection);
}

std::string PrintWebSocket::JsonValueToString(const Json::Value &value) {
  Json::StreamWriterBuilder writer_builder;
  std::unique_ptr<Json::StreamWriter> json_write(
      writer_builder.newStreamWriter());
  std::ostringstream stream;
  json_write->write(value, &stream);
  return stream.str();
}
