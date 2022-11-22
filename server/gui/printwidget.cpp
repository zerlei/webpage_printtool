#include "printwidget.h"
#include "ui_printwidget.h"
#include <QIcon>

PrintWidget::PrintWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrintWidget)
{

    ui->setupUi(this);
    ui->mvwidget->setMoveWidget(this);
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::Tool);
    this->setWindowIcon(QIcon(":/bg/favicon.ico"));
    this->setWindowTitle("Installer");

    setAttribute(Qt::WA_TranslucentBackground, true);
}

PrintWidget::~PrintWidget()
{
    delete ui;
}
