
#include <QApplication>
#include <QDebug>
#include <QPushButton>
#include <QWebEngineView>
#include <drogon/drogon.h>
#include <functional>
#include <memory>
#include <thread>
#include "./net_interface/clientwebsocket.h"
#include "./net_interface/printcontroller.h"
#include "./printer/printmsgstation.h"
using namespace drogon;



int main(int argc, char *argv[]) {


QApplication a(argc, argv);
//  if(argc<=1) {
//      return 0;
//  }

std::string docoment_root = "./";

// qDebug()<<QString("导出为WPS PDF");
auto websocket = std::make_shared<PrintWebSocket>();
PrintMsgStation print(websocket.get());
auto controller = std::make_shared<PrintController>(print);
std::thread i([ websocket,controller,&docoment_root]() {
  app()
      .setLogPath("./")
      .setLogLevel(trantor::Logger::kWarn)
      .addListener("0.0.0.0", 8847)
      .setDocumentRoot(docoment_root)
      .setThreadNum(1)
      .registerController(controller)
      .registerController(websocket)
      .run();
});

return a.exec();
}
