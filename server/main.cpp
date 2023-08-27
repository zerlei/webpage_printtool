
#include "./net_interface/clientwebsocket.h"
#include "./net_interface/printcontroller.h"
#include "./net_interface/printwebsocket.h"
#include "./printer/printmsgstation.h"
#include <QApplication>
#include <QDebug>
#include <QMenu>
#include <QPushButton>
#include <QSystemTrayIcon>
#include <QWebEngineView>
#include <drogon/drogon.h>
#include <functional>
#include <memory>
#include <qaction.h>
#include <qapplication.h>
#include <qicon.h>
#include <qobject.h>
#include <qsystemtrayicon.h>
#include <thread>
#include <winsock2.h>

using namespace drogon;

int main(int argc, char *argv[]) {

  QApplication a(argc, argv);

#pragma region 初始化 trayIcon

  QSystemTrayIcon _tray_icon;
  _tray_icon.setIcon(QIcon(":/bg/favicon.ico"));
  _tray_icon.setToolTip("打印助手😀");
  QMenu *trayIconMenu = new QMenu();
  QAction *quitAction =
      new QAction(QIcon(":bg/exit.png"), "退出", trayIconMenu);
  QAction *seeAction = new QAction(QIcon(":bg/look.png"), "查看", trayIconMenu);

  QObject::connect(seeAction, &QAction::triggered,
                   []() { system("start http://127.0.0.1:8847"); });
  QObject::connect(quitAction, &QAction::triggered,
                   []() { QApplication::exit(); });

  trayIconMenu->addAction(seeAction);
  trayIconMenu->addAction(quitAction);
  _tray_icon.setContextMenu(trayIconMenu);
  _tray_icon.show();
  QObject::connect(&_tray_icon, &QSystemTrayIcon::activated,
                   [](QSystemTrayIcon::ActivationReason reason) {
                     switch (reason) {
                     case QSystemTrayIcon::DoubleClick: {
                       system("start http://127.0.0.1:8847");
                       break;
                     }
                     default: {
                       break;
                     }
                     }
                   });

#pragma endregion

  // PrintWidget w;

  // w.show();
  // w.setWindowState((w.windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
  auto dr = QApplication::applicationDirPath() + "/wwwroot/";
  std::string docoment_root = dr.toStdString();
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
