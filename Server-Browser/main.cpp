#include <QApplication>
#include <QWebEngineView>
#include "webprinter.h"
#include <QDebug>
#include <thread>
#include <QPushButton>
#include <functional>
#include <drogon/drogon.h>
#include <memory>
#include "server/printcontroller.h"
#include "server/printwebsocket.h"

using namespace drogon;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    //qDebug()<<QString("导出为WPS PDF");


    WebPrinter print;


        std::thread i([&print](){

            auto controller = std::make_shared<PrintController>(print);
            auto websocket = std::make_shared<PrintWebSocket>();
            app().setLogPath("./")
                 .setLogLevel(trantor::Logger::kWarn)
                 .addListener("0.0.0.0", 8847)
                 .setThreadNum(1)
                 .registerController(controller)
                 .registerController(websocket)
                 .run();

          });




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
//                                PrintModel::LoadAchieve,layout,[](bool,const QString& str){
//            qDebug()<<str;

//        },Qt::QueuedConnection);
//    });

//    std::thread j([&station](){
//        std::function<void(bool,const QString&)> callback = [](bool is,const QString& str)  {
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






    return a.exec();
}
