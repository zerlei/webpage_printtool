#ifndef PRINTWEBSOCKET_H
#define PRINTWEBSOCKET_H

#include <drogon/HttpTypes.h>
#include <drogon/WebSocketConnection.h>
#include <drogon/WebSocketController.h>
#include <drogon/utils/FunctionTraits.h>
#include <json/value.h>
#include <tuple>
#include <list>
using namespace drogon;

class PrintWebSocket : public WebSocketController<PrintWebSocket,false>
{
public:
  PrintWebSocket();
  virtual void handleNewMessage(const WebSocketConnectionPtr &, std::string &&,
                                const WebSocketMessageType &) override;
  virtual void handleNewConnection(const HttpRequestPtr &,
                                   const WebSocketConnectionPtr &) override;
  virtual void handleConnectionClosed(const WebSocketConnectionPtr &) override;

  WS_PATH_LIST_BEGIN
  WS_PATH_ADD("/websocet", Get);
  WS_PATH_LIST_END


 public:
  std::list<WebSocketConnectionPtr> _webconnections;
  std::string JsonValueToString(const Json::Value&);



};

#endif // PRINTWEBSOCKET_H
