#ifndef WEBPRINTER_H
#define WEBPRINTER_H
#include "base.h"
#include <QList>
#include <QObject>
#include <QPrinter>
#include <QPrinterInfo>
#include <QTimer>
#include <QWebEngineView>
#include <functional>
#include <memory>
#include <qbuffer.h>
#include <queue>
#include <tuple>
#include <qpdfdocument.h>
#include <QBuffer>
enum class PrintModel { LoadAchieve, JsPrintRequest };
enum class PrintState { IsWorking, IsWaiting };
using WebPagePrintInfo = std::tuple<QUrl, QString, PrintModel, QString,
                                    QMarginsF, QPageLayout::Orientation,
                                    std::function<void(bool, const QString &)>>;



class WebPrinter : public QObject {
  Q_OBJECT
public:
  WebPrinter();

Q_SIGNALS:
  void signalGuiThreadToWork();

public:
  /// 打印网页
  /// \brief PrintUrl
  /// \param url
  /// \param printName
  /// \param model
  ///
  void
  addPrintWebPageToQueue(std::tuple<PrinterConfigPtr, PrintedPagePtr,
                                    std::function<void(bool, const QString &)>>
                             printInfo);
  /// 获取当前的打印机信息
  /// \brief GetAvaliablePrinterInfo
  /// \return
  ///
  const QList<QPrinterInfo> &getAvaliablePrinterInfo();
  /// 更新当前的打印机
  /// \brief UpdatePrinterInfo
  ///
  void updatePrinterInfo();

  ///
  /// \brief StartWork
  ///
  void startWork();

private:
  std::queue<std::tuple<PrinterConfigPtr, PrintedPagePtr,
                        std::function<void(bool, const QString &)>>>
      _s_print_task;

  std::tuple<PrinterConfigPtr, PrintedPagePtr,
             std::function<void(bool, const QString &)>>
      _current_task;

  ///
  /// \brief 当前打印的网页
  ///
  std::tuple<QUrl, QPrinter *, PrintModel,
             std::function<void(bool, const QString &)>>
      _currentpage;
  ///
  /// \brief 计时器，处理网页超时异常
  ///

  QTimer _print_request_time;
  ///
  /// \brief 当前电脑上可使用的打印机信息
  ///

  QList<QPrinterInfo> _avaliable_printer_info;

  ///
  /// \brief 当前已经生成的打印机
  ///

  QList<QPrinter *> _printers;

  QPrinter *_current_print{nullptr};

  /// 渲染页面
  /// \brief _render_view
  ///
  QWebEngineView _render_view;

  /// when a webpage take much time
  /// \brief _timeout_listen
  ///
  QTimer _timeout_listen;

  PrintState _currentState{PrintState::IsWaiting};

  ///
  /// \brief 打印当前网页
  ///
  void toPrint();


  std::function<void(const QByteArray &)> printToPdfResult;

  QPdfDocument *_pdf_documnet;

private slots:
  ///
  /// \brief 网页加载完毕打印
  ///
  void slotLoadFinishTorint(bool);
  ///
  /// \brief Js window.print 调用打印页面
  ///
  void slotJsPrintRequestToPrint();

  ///
  /// \brief SlotPrintRequestTimeOut
  ///

  void slotPrintRequestTimeOut();


  /// 移动到GUI线程开启打印工作
  /// \brief SlotMoveToGUIThreadWork
  ///

  void slotMoveToPrinterThreadWork();
};

#endif // WEBPRINTER_H
