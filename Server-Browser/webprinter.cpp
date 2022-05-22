#include "webprinter.h"
#include <QDebug>

WebPrinter::WebPrinter()
    :_avaliable_printer_info(QPrinterInfo::availablePrinters())
{

      qDebug()<<_avaliable_printer_info.at(0).printerName();

      qDebug()<<_avaliable_printer_info.at(0).supportedPageSizes();

//    for(auto& x: _avaliable_printer_info) {
//        qDebug()<<x.printerName();
//        qDebug()<<x.supportedPageSizes();
//        qDebug()<<"#######################";

//    }
    connect(&_render_view,&QWebEngineView::loadFinished,this,&WebPrinter::SlotLoadFinishTorint);
    connect(&_render_view,&QWebEngineView::printRequested,this,&WebPrinter::SlotJsPrintRequestToPrint);
    connect(&_render_view,&QWebEngineView::printFinished,this,&WebPrinter::SlotPrintFinshed);

    connect(&_timeout_listen,&QTimer::timeout,this,&WebPrinter::SlotPrintRequestTimeOut);

    connect(this,&WebPrinter::signalGuiThreadToWork,this,&WebPrinter::SlotMoveToGUIThreadWork,Qt::QueuedConnection);
    //_timeout_listen.start(200000);

}

void WebPrinter::AddPrintWebPageToQueue(const QUrl &url, const QPrinterInfo& printerInfo
                              ,PrintModel model,const QPageLayout& layout, std::function<void (bool, const QString &)> callback)
{


  _webpagelist.push(

        std::make_tuple(
          url,
          printerInfo,
          model,
          layout,
          callback
          )

        );

}

void WebPrinter::AddNewPrinter(const QPrinterInfo &printerinfo)
{
    _printers.push_back(new QPrinter(printerinfo,QPrinter::HighResolution));

}

const QList<QPrinterInfo> &WebPrinter::GetAvaliablePrinterInfo()
{
    return _avaliable_printer_info;

}

void WebPrinter::UpdatePrinterInfo()
{
    _avaliable_printer_info = QPrinterInfo::availablePrinters();

}

void WebPrinter::ToPrint()
{
    if(_webpagelist.empty()) {

        _currentState = PrintState::IsWaiting;
        return;
    }

    auto current = _webpagelist.front();
    _webpagelist.pop();

    for(auto& x: _printers) {
            if(x->printerName() == std::get<1>(current).printerName()) {

                auto supportPagesizes = std::get<1>(current).supportedPageSizes();
                auto pagesize = std::get<3>(current).pageSize();

                if (std::find(std::begin(supportPagesizes),std::end(supportPagesizes),pagesize)
                        == std::end(supportPagesizes))
                {
                    return std::get<4>(current)(false,QString("不匹配的纸张!"));
                }
                x->setPageLayout(std::get<3>(current));

                _currentpage = std::make_tuple(
                        std::get<0>(current),
                        x,
                        std::get<2>(current),
                        std::get<4>(current)

                      );
                 _timeout_listen.start(200000);
                 _render_view.load(std::get<0>(_currentpage));

            }
    }

        return std::get<4>(current)(false,QString("没有找到匹配的打印机！"));

}

void WebPrinter::StartWork()
{
  if(_currentState == PrintState::IsWorking) {
      return;
    }
  _currentState = PrintState::IsWorking;
  ToPrint();
}

void WebPrinter::SlotLoadFinishTorint(bool isSuccess)
{
    if(isSuccess) {
        if(std::get<2>(_currentpage) == PrintModel::LoadAchieve) {
            return _render_view.print(std::get<1>(_currentpage));
        }

    } else {
        std::get<3>(_currentpage)(false,QString("页面加载失败！"));
    }


}

void WebPrinter::SlotJsPrintRequestToPrint()
{
    if(std::get<2>(_currentpage) == PrintModel::JsPrintRequest) {
        _render_view.print(std::get<1>(_currentpage));
    }
}

void WebPrinter::SlotPrintRequestTimeOut()
{
    ///
    ///
    ///可能是用户传递的页面存在问题
    std::get<3>(_currentpage)(false,QString("超时，打印失败！"));
    ToPrint();
}

void WebPrinter::SlotPrintFinshed(bool isSuccess)
{
    if(isSuccess) {
        std::get<3>(_currentpage)(true,QString());
    } else {
        std::get<3>(_currentpage)(false,QString("预期之外的异常，打印失败！"));
    }
    _timeout_listen.stop();
    ToPrint();

}

void WebPrinter::SlotMoveToGUIThreadWork()
{
  StartWork();

}



