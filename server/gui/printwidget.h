#ifndef PRINTWIDGET_H
#define PRINTWIDGET_H

#include <QWidget>
#include <QSystemTrayIcon>

namespace Ui {
class PrintWidget;
}

class PrintWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PrintWidget(QWidget *parent = nullptr);
    ~PrintWidget();

private:
    Ui::PrintWidget *ui;
    QSystemTrayIcon _tray_icon;

};

#endif // PRINTWIDGET_H
