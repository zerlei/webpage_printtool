// #include "printdatabase.h"
// #include "printmsgstation.h"
// #include "server/printcontroller.h"
// #include "server/printwebsocket.h"
// #include <QApplication>
// #include <QDebug>
// #include <QPushButton>
// #include <QWebEngineView>
// #include <drogon/drogon.h>
// #include <functional>
// #include <memory>
// #include <thread>
// using namespace drogon;
#include <iostream>
#include <json/json.h>
#include <json/writer.h>

int main(int argc, char *argv[]) {

  std::string str{"{\"hi\":1}"};
  Json::Reader reader;
  Json::FastWriter f;

  Json::Value jsob;

  if (reader.parse(str, jsob)) {
    auto x = std::string(jsob.toStyledString());

    bool b1 = jsob["hi"].isNull();
    bool b2 = jsob["Hi"].isNull();
    std::string x1 = f.write(jsob);
  }
  return 0;

  // return 0;

  // QApplication a(argc, argv);
  // //  if(argc<=1) {
  // //      return 0;
  // //  }

  // std::string docoment_root = "./";

  // // qDebug()<<QString("导出为WPS PDF");

  // PrintMsgStation print;

  // std::thread i([&print, &docoment_root]() {
  //   auto controller = std::make_shared<PrintController>(print);
  //   auto websocket = std::make_shared<PrintWebSocket>();
  //   app()
  //       .setLogPath("./")
  //       .setLogLevel(trantor::Logger::kWarn)
  //       .addListener("0.0.0.0", 8847)
  //       .setDocumentRoot(docoment_root)
  //       .setThreadNum(1)
  //       .registerController(controller)
  //       .registerController(websocket)
  //       .run();
  // });

  //    QWebEngineView view;
  //    view.load(QUrl("http://localhost"));
  //    view.show();

  //    WebPrinter webprinter;

  //    auto item = webprinter.GetAvaliablePrinterInfo().first();

  //    webprinter.AddNewPrinter(item);

  //    auto pagesize = item.defaultPageSize();
  //    QPageLayout layout;
  //    layout.setPageSize(pagesize);

  //    QObject::connect(&station,&MutiThreadMesageStation::signalsToTest,[&layout,&webprinter,&item](){
  //        webprinter.PrintWebPage(QUrl("http://www.bing.com"),item,
  //                                PrintModel::LoadAchieve,layout,[](bool,const
  //                                QString& str){
  //            qDebug()<<str;

  //        },Qt::QueuedConnection);
  //    });

  //    std::thread j([&station](){
  //        std::function<void(bool,const QString&)> callback = [](bool is,const
  //        QString& str)  {
  //            qDebug()<<str;

  //        };
  //        auto a = callback;
  //        emit station.signalsToTest();

  //        auto id = std::this_thread::get_id();
  //        qDebug()<<*(unsigned int*)(&id);
  ////        while(true) {
  ////            std::this_thread::sleep_for(std::chrono::seconds(1));

  ////            qDebug()<<"hi";
  ////        }

  //    });

  // return a.exec();
}
