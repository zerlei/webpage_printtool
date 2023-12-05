#ifndef PRINTWEBSOCKET_H
#define PRINTWEBSOCKET_H

#include <drogon/HttpTypes.h>
#include <drogon/WebSocketConnection.h>
#include <drogon/WebSocketController.h>
#include <drogon/utils/FunctionTraits.h>
#include <json/value.h>
#include <list>
#include <stdint.h>
#include <string>
#include <tuple>
#include "../printer/printmsgstation.h"

using namespace drogon;


class PrintWebSocket : public WebSocketController<PrintWebSocket, false> {
public:
  PrintWebSocket( PrintMsgStation &);
  virtual void handleNewMessage(const WebSocketConnectionPtr &, std::string &&,
                                const WebSocketMessageType &) override;
  virtual void handleNewConnection(const HttpRequestPtr &,
                                   const WebSocketConnectionPtr &) override;
  virtual void handleConnectionClosed(const WebSocketConnectionPtr &) override;

  WS_PATH_LIST_BEGIN
  WS_PATH_ADD("/", Get);
  WS_PATH_LIST_END
private:
  std::list<WebSocketConnectionPtr> _webconnections;
  PrintMsgStation &_printmsgstation;
  std::string JsonValueToString(const Json::Value &);
  bool _is_working{false};
};

#endif // PRINTWEBSOCKET_H
