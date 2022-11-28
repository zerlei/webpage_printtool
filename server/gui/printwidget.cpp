#include "printwidget.h"
#include "ui_printwidget.h"
#include <QIcon>
#include <qpushbutton.h>
#include <QDebug>

PrintWidget::PrintWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::PrintWidget) {

  ui->setupUi(this);
  ui->mvwidget->setMoveWidget(this);
  this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
  this->setWindowIcon(QIcon(":/bg/favicon.ico"));
  this->setWindowTitle("Installer");

  setAttribute(Qt::WA_TranslucentBackground, true);

  connect(ui->btn_close, &QPushButton::clicked, [this](bool) {
    this->hide();
  });

  connect(ui->btn_open_url, &QPushButton::clicked, [](bool){
    //NOTE  Windows Only 
    system("start http://127.0.0.1:8847");
  });
  _tray_icon.setIcon(QIcon(":/bg/favicon.ico"));
  _tray_icon.setToolTip("打印助手哦😊");
  _tray_icon.show();
  connect(&_tray_icon, &QSystemTrayIcon::activated,
          [this](QSystemTrayIcon::ActivationReason reason) {
            switch (reason) {
            case QSystemTrayIcon::Trigger: {
              this->show();
              this->activateWindow();
              break;
            }
            case QSystemTrayIcon::DoubleClick: {
              this->show();
              this->activateWindow();
              break;
            }
            default:
              break;
            }
          });
          
}

PrintWidget::~PrintWidget() { delete ui; }
