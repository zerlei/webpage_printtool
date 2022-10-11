#ifndef WEBPRINTER_H
#define WEBPRINTER_H
#include <QList>
#include <QObject>
#include <QPrinter>
#include <QPrinterInfo>
#include <QTimer>
#include <QWebEngineView>
#include <functional>
#include <memory>
#include <queue>
#include <tuple>
enum class PrintModel { LoadAchieve, JsPrintRequest };

enum class PrintState { IsWorking, IsWaiting };

///
/// Url,PageName,PrintModel,QMarginF,
///
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
  AddPrintWebPageToQueue(const QUrl &url, const QString &printerInfoName,
                         PrintModel model, const QString &PageName,
                         const QMarginsF &marginsF, QPageLayout::Orientation,
                         std::function<void(bool, const QString &)> callback);
  /// 获取当前的打印机信息
  /// \brief GetAvaliablePrinterInfo
  /// \return
  ///
  const QList<QPrinterInfo> &GetAvaliablePrinterInfo();
  /// 更新当前的打印机
  /// \brief UpdatePrinterInfo
  ///
  void UpdatePrinterInfo();

  ///
  /// \brief StartWork
  ///
  void StartWork();

private:
  ///
  /// \brief WebPageList
  ///
  std::queue<WebPagePrintInfo> _webpagelist;

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


  QPrinter* _current_print{nullptr};

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
  void ToPrint();

private slots:
  ///
  /// \brief 网页加载完毕打印
  ///
  void SlotLoadFinishTorint(bool);
  ///
  /// \brief Js window.print 调用打印页面
  ///
  void SlotJsPrintRequestToPrint();

  ///
  /// \brief SlotPrintRequestTimeOut
  ///

  void SlotPrintRequestTimeOut();

  ///
  /// \brief 打印结束调用函数
  ///

  void SlotPrintFinshed(bool);

  /// 移动到GUI线程开启打印工作
  /// \brief SlotMoveToGUIThreadWork
  ///

  void SlotMoveToGUIThreadWork();
};

#endif // WEBPRINTER_H
