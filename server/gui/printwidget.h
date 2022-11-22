#ifndef PRINTWIDGET_H
#define PRINTWIDGET_H

#include <QWidget>

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
};

#endif // PRINTWIDGET_H
