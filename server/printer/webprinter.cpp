#include "webprinter.h"
#include "printer/webprinter.h"
#include <QDebug>
#include <QFile>
#include <QPdfDocument>
#include <qbuffer.h>
#include <qevent.h>
#include <qmargins.h>
#include <qpagelayout.h>
#include <qpagesize.h>
#include <qpdfdocument.h>


WebPrinter::WebPrinter()
    : _avaliable_printer_info(QPrinterInfo::availablePrinters()) {

  //  qDebug() << _avaliable_printer_info.at(0).printerName();

  //  qDebug() << _avaliable_printer_info.at(0).supportedPageSizes();

  //    for(auto& x: _avaliable_printer_info) {
  //        qDebug()<<x.printerName();
  //        qDebug()<<x.supportedPageSizes();
  //        qDebug()<<"#######################";

  //    }
  connect(&_render_view, &QWebEngineView::loadFinished, this,
          &WebPrinter::slotLoadFinishTorint);
  connect(&_render_view, &QWebEngineView::printRequested, this,
          &WebPrinter::slotJsPrintRequestToPrint);
  connect(&_render_view, &QWebEngineView::printFinished, this,
          &WebPrinter::slotPrintFinshed);

  connect(&_timeout_listen, &QTimer::timeout, this,
          &WebPrinter::slotPrintRequestTimeOut);

  connect(this, &WebPrinter::signalGuiThreadToWork, this,
          &WebPrinter::slotMoveToPrinterThreadWork, Qt::QueuedConnection);

  _pdf_documnet = new QPdfDocument(this);
  connect(_pdf_documnet, &QPdfDocument::statusChanged,
          [this](QPdfDocument::Status status) {
            if (status == QPdfDocument::Status::Ready) {

              QFile file("test.pdf");
              if (!file.open(QIODevice::WriteOnly)) {
                std::get<2>(_current_task)(false,
                                           "写入文件失败,是否有相关权限？!");
              } else {
                file.write(_pdf_c_data);
              }
              file.close();
              _render_view.stop();
              _timeout_listen.stop();
              toPrint();
            } else if (status == QPdfDocument::Status::Error) {
              std::get<2>(_current_task)(false, "未知错误,未能渲染成功!");
              _render_view.stop();
              _timeout_listen.stop();
              toPrint();
            }
          });
  printToPdfResult = [this](const QByteArray &data) {
    if (data.isEmpty()) {
      std::get<2>(_current_task)(false, "未知错误,未能渲染成功!");
      _render_view.stop();
      _timeout_listen.stop();
      toPrint();
    } else {
      _pdf_c_data = data;
      QBuffer buffer;
      buffer.setData(_pdf_c_data);
      _pdf_documnet->load(&buffer);
    }
  };
  //_timeout_listen.start(200000);
}

void WebPrinter::addPrintWebPageToQueue(
    std::tuple<PrinterConfigPtr, PrintedPagePtr,
               std::function<void(bool, const QString &)>>
        printInfo) {
  _s_print_task.push(printInfo);
}

const QList<QPrinterInfo> &WebPrinter::getAvaliablePrinterInfo() {
  return _avaliable_printer_info;
}

void WebPrinter::updatePrinterInfo() {
  _avaliable_printer_info = QPrinterInfo::availablePrinters();
}

// void WebPrinter::toPrint() {
//   if (_webpagelist.empty()) {

//     _currentState = PrintState::IsWaiting;
//     return;
//   }

//   auto current = _webpagelist.front();
//   _webpagelist.pop();

//   for (auto &x : _avaliable_printer_info) {
//     if (x.printerName() == std::get<1>(current)) {

//       for (auto &y : x.supportedPageSizes()) {
//         if (y.name() == std::get<3>(current)) {
//           QPageLayout pagelayout;
//           pagelayout.setPageSize(y);
//           pagelayout.setMargins(std::get<4>(current));
//           pagelayout.setOrientation(std::get<5>(current));
//           bool ishasIn = false;
//           for (auto &_p : _printers) {
//             if (_p->printerName() == x.printerName()) {
//               ishasIn = true;
//               _current_print = _p;
//               break;
//             }
//           }
//           if (!ishasIn) {
//             _current_print = new QPrinter(x, QPrinter::HighResolution);
//             _current_print->setResolution(4000);
//             _printers.push_back(_current_print);
//           }
//           _current_print->setPageLayout(pagelayout);

//           _currentpage =
//               std::make_tuple(std::get<0>(current), _current_print,
//                               std::get<2>(current), std::get<6>(current));
//           _timeout_listen.start(200000);
//           _render_view.load(std::get<0>(_currentpage));
//           return;
//         }
//       }
//     }
//   }
//   return std::get<6>(current)(false, QString("没有找到匹配的打印机或者纸张！"));
// }

void WebPrinter::toPrint() {
  if (_s_print_task.empty()) {
    _currentState = PrintState::IsWaiting;
    return;
  }
  _current_task = _s_print_task.front();
  _s_print_task.pop();
  auto [printerInfo, webPageInfo, callBackFun] = _current_task;
  // const std::string &SaveType = printerInfo->SaveType;
  // if (SaveType.length() != 4) {
  //   callBackFun(false, "SaveType格式错误!");
  //   return;
  // }
  _timeout_listen.start(200000);
  _render_view.load(QString::fromStdString(webPageInfo->PageName));
}

void WebPrinter::startWork() {
  if (_currentState == PrintState::IsWorking) {
    return;
  }
  _currentState = PrintState::IsWorking;
  toPrint();
}

void WebPrinter::slotLoadFinishTorint(bool isSuccess) {
  if (isSuccess) {
    if (std::get<1>(_current_task)->PrintMode == "LoadAchieve") {
      auto printInfo = std::get<0>(_current_task);
      _render_view.printToPdf(
          printToPdfResult,
          QPageLayout(QPageSize(QSize(printInfo->PaperWidthInmm,
                                      printInfo->PaperHeightInmm)),
                      QPageLayout::Portrait,
                      QMarginsF(printInfo->LeftMargin, printInfo->TopMargin,
                                printInfo->RightMargin,
                                printInfo->BottomMargin),
                      QPageLayout::Millimeter));
    }
  } else {
    std::get<2>(_current_task)(false, QString("页面加载失败！"));
    _render_view.stop();
    _timeout_listen.stop();
    toPrint();
  }
}

void WebPrinter::slotJsPrintRequestToPrint() {

  if (std::get<1>(_current_task)->PrintMode == "JsPrintRequest") {
    auto printInfo = std::get<0>(_current_task);
    _render_view.printToPdf(
        printToPdfResult,
        QPageLayout(QPageSize(QSize(printInfo->PaperWidthInmm,
                                    printInfo->PaperHeightInmm)),
                    QPageLayout::Portrait,
                    QMarginsF(printInfo->LeftMargin, printInfo->TopMargin,
                              printInfo->RightMargin, printInfo->BottomMargin),
                    QPageLayout::Millimeter));
  }
}

void WebPrinter::slotPrintRequestTimeOut() {
  ///
  ///
  /// 可能是用户传递的页面存在问题
  std::get<3>(_currentpage)(false, QString("超时，打印失败！"));
  _render_view.stop();
  toPrint();
}

void WebPrinter::slotPrintFinshed(bool isSuccess) {
  if (isSuccess) {
    std::get<3>(_currentpage)(true, QString());
  } else {
    std::get<3>(_currentpage)(false, QString("预期之外的异常，打印失败！"));

    _printers.removeAt(_printers.indexOf(_current_print));
    delete _current_print;
  }
  _render_view.stop();
  _timeout_listen.stop();
  toPrint();
}

void WebPrinter::slotMoveToPrinterThreadWork() { startWork(); }
