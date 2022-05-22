#include "printwebsocket.h"

PrintWebSocket::PrintWebSocket()
{

}

void PrintWebSocket::handleNewMessage(const WebSocketConnectionPtr &webconnection, std::string &&str, const WebSocketMessageType &)
{

  webconnection->send("😀"+str+"😀");

}

void PrintWebSocket::handleNewConnection(const HttpRequestPtr &, const WebSocketConnectionPtr &webconnection)
{
  _webconnections.push_back(webconnection);
//  Json::Value value;
//  value['msg']
  webconnection->send("Hello!");

}

void PrintWebSocket::handleConnectionClosed(const WebSocketConnectionPtr &webconnection)
{
  _webconnections.remove(webconnection);


}

std::string PrintWebSocket::JsonValueToString(const Json::Value &value)
{
  Json::StreamWriterBuilder writer_builder;
  std::unique_ptr<Json::StreamWriter> json_write(
      writer_builder.newStreamWriter());
  std::ostringstream stream;
  json_write->write(value, &stream);
  return stream.str();

}
