#include "webprinter.h"
#include "printer/webprinter.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QImageWriter>
#include <QPainter>
#include <QPdfDocument>
#include <qbuffer.h>
#include <qdatetime.h>
#include <qevent.h>
#include <qimagewriter.h>
#include <qmargins.h>
#include <qnamespace.h>
#include <qpagelayout.h>
#include <qpagesize.h>
#include <qpdfdocument.h>
#include <qsize.h>
#include <string>

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
  connect(&_timeout_listen, &QTimer::timeout, this,
          &WebPrinter::slotPrintRequestTimeOut);

  connect(this, &WebPrinter::signalGuiThreadToWork, this,
          &WebPrinter::slotMoveToPrinterThreadWork, Qt::QueuedConnection);

  _pdf_documnet = new QPdfDocument(this);
  connect(_pdf_documnet, &QPdfDocument::statusChanged,
          [this](QPdfDocument::Status status) {
            if (status == QPdfDocument::Status::Ready) {

              auto [printer, page, callf] = _current_task;
              const std::string &SaveType = printer->SaveType;
              if (SaveType.length() != 4) {
                callf(false, "SaveType格式错误!");
                _pdf_documnet->close();
              } else {
                auto temPath = QDir::homePath() + "/WebPrintTool";
                auto fileName =
                    temPath + "/" +
                    QString(std::to_string(QDateTime::currentMSecsSinceEpoch())
                                .c_str());
                if (!QDir(temPath).exists()) {
                  QDir(temPath).mkdir(temPath);
                }
                // 保存Pdf
                if (SaveType.at(0) == '1') {
                  if (!QFile::copy("temp.pdf", fileName + ".pdf")) {
                    // doSomething
                  }
                }

                if (SaveType.at(1) == '1' || SaveType.at(2) == '1') {
                  auto image = _pdf_documnet->render(
                      _pdf_documnet->pageCount(),
                      QSize(printer->PaperWidthInmm, printer->PaperHeightInmm));
                  // 保存png 图片
                  if (SaveType.at(1) == '1') {
                    QImageWriter writer(fileName + "./png", "png");
                    writer.write(image);
                  }
                  // 打印
                  if (SaveType.at(2) == '1') {
                    for (auto &x : _avaliable_printer_info) {
                      if (x.printerName() ==
                          QString::fromStdString(printer->PrinterName)) {
                        for (auto &y : x.supportedPageSizes()) {
                          if (y.name() ==
                              QString::fromStdString(printer->PaperName)) {
                            QPageLayout layout;
                            layout.setPageSize(y);
                            if (printer->Orientation == "横向") {
                              layout.setOrientation(QPageLayout::Landscape);
                            } else {
                              layout.setOrientation(QPageLayout::Portrait);
                            }
                            bool isHasIn = false;
                            for (auto &p : _printers) {
                              if (p->printerName() == x.printerName()) {
                                isHasIn = true;
                                _current_print = p;
                                break;
                              }
                            }
                            if (!isHasIn) {
                              _current_print =
                                  new QPrinter(x, QPrinter::HighResolution);

                              _printers.push_back(_current_print);
                            }
                            // 是否使用打印机默认配置
                            if (SaveType.at(3) == '1') {
                              _current_print->setPageLayout(layout);
                              _current_print->setResolution(4000);
                            }

                            QPainter painter;
                            if (!painter.begin(_current_print)) {
                              callf(false, "打印机错误!");
                              _pdf_documnet->close();
                              _render_view.stop();
                              _timeout_listen.stop();
                              toPrint();

                              _printers.removeAt(
                                  _printers.indexOf(_current_print));
                              delete _current_print;
                              return;
                            };
                            painter.drawImage(0, 0, image);
                            painter.end();
                            break;
                          }
                        }
                        break;
                      }
                    }
                  }
                }
                _pdf_documnet->close();

                callf(true, "");
              }
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
      QFile file("temp.pdf");
      if (!file.open(QIODevice::WriteOnly)) {
        std::get<2>(_current_task)(false, "写入文件失败,是否有相关权限？!");
      } else {
        file.write(data);
      }
      file.close();
      _pdf_documnet->load("temp.pdf");
    }
  };
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

void WebPrinter::toPrint() {
  if (_s_print_task.empty()) {
    _currentState = PrintState::IsWaiting;
    return;
  }
  _current_task = _s_print_task.front();
  _s_print_task.pop();
  auto [printerInfo, webPageInfo, callBackFun] = _current_task;
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
                                      printInfo->PaperHeightInmm),
                                QPageSize::Millimeter),
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

void WebPrinter::slotMoveToPrinterThreadWork() { startWork(); }
