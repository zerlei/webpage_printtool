#include "webprinter.h"
#include <QDebug>

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
  //_timeout_listen.start(200000);
}

void WebPrinter::addPrintWebPageToQueue(
    const QUrl &url, const QString &printerInfoName, PrintModel model,
    const QString &PageName, const QMarginsF &marginsF,
    QPageLayout::Orientation oriention,
    std::function<void(bool, const QString &)> callback) {
  _webpagelist.push(std::make_tuple(url, printerInfoName, model, PageName,
                                    marginsF, oriention, callback));
}

const QList<QPrinterInfo> &WebPrinter::getAvaliablePrinterInfo() {
  return _avaliable_printer_info;
}

void WebPrinter::updatePrinterInfo() {
  _avaliable_printer_info = QPrinterInfo::availablePrinters();
}

void WebPrinter::toPrint() {
  if (_webpagelist.empty()) {

    _currentState = PrintState::IsWaiting;
    return;
  }

  auto current = _webpagelist.front();
  _webpagelist.pop();

  for (auto &x : _avaliable_printer_info) {
    if (x.printerName() == std::get<1>(current)) {

      for (auto &y : x.supportedPageSizes()) {
        if (y.name() == std::get<3>(current)) {
          QPageLayout pagelayout;
          pagelayout.setPageSize(y);
          pagelayout.setMargins(std::get<4>(current));
          pagelayout.setOrientation(std::get<5>(current));
          bool ishasIn = false;
          for (auto &_p : _printers) {
            if (_p->printerName() == x.printerName()) {
              ishasIn = true;
              _current_print = _p;
              break;
            }
          }
          if (!ishasIn) {
            _current_print = new QPrinter(x, QPrinter::HighResolution);
            _current_print->setResolution(4000);
            _printers.push_back(_current_print);
          }
          _current_print->setPageLayout(pagelayout);

          _currentpage =
              std::make_tuple(std::get<0>(current), _current_print,
                              std::get<2>(current), std::get<6>(current));
          _timeout_listen.start(200000);
          _render_view.load(std::get<0>(_currentpage));
          return;
        }
      }
    }
  }
  return std::get<6>(current)(false, QString("没有找到匹配的打印机或者纸张！"));
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
    if (std::get<2>(_currentpage) == PrintModel::LoadAchieve) {
      return _render_view.print(std::get<1>(_currentpage));
    }

  } else {
    std::get<3>(_currentpage)(false, QString("页面加载失败！"));
    _render_view.stop();
    _timeout_listen.stop();
    toPrint();
  }
}

void WebPrinter::slotJsPrintRequestToPrint() {
  if (std::get<2>(_currentpage) == PrintModel::JsPrintRequest) {
    _render_view.print(std::get<1>(_currentpage));
  }
}

void WebPrinter::slotPrintRequestTimeOut() {
  ///
  ///
  ///可能是用户传递的页面存在问题
  std::get<3>(_currentpage)(false, QString("超时，打印失败！"));
  _render_view.stop();
  toPrint();
}

void WebPrinter::slotPrintFinshed(bool isSuccess) {
  if (isSuccess) {
    std::get<3>(_currentpage)(true, QString());
  } else {
    std::get<3>(_currentpage)(false, QString("预期之外的异常，打印失败！"));

    _printers.removeAt( _printers.indexOf(_current_print));
    delete _current_print;
  }
  _render_view.stop();
  _timeout_listen.stop();
  toPrint();
}

void WebPrinter::slotMoveToPrinterThreadWork() { startWork(); }
