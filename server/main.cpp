
#include "./net_interface/clientwebsocket.h"
#include "./net_interface/printcontroller.h"
#include "./net_interface/printwebsocket.h"
#include "./printer/printmsgstation.h"
#include <QApplication>
#include <QDebug>
#include <QPushButton>
#include <QWebEngineView>
#include <drogon/drogon.h>
#include <functional>
#include <memory>
#include <thread>

using namespace drogon;

int main(int argc, char *argv[]) {

  QApplication a(argc, argv);
  std::string docoment_root = "./";
  if (argc <= 1) {
    // return 0;
  } else {
    docoment_root = argv[1];
  }

  PrintMsgStation printstation;
  auto c_websoc = std::make_shared<ClientWebsoc>(printstation);
  auto s_websoc = std::make_shared<PrintWebSocket>(printstation);
  auto controller = std::make_shared<PrintController>(printstation);
  std::thread i([s_websoc, controller, &docoment_root]() {
    app()
        .setLogPath("./")
        .setLogLevel(trantor::Logger::kWarn)
        .addListener("0.0.0.0", 8847)
        .setDocumentRoot(docoment_root)
        .setThreadNum(1)
        .registerController(controller)
        .registerController(s_websoc)
        .run();
  });

  return a.exec();
}
