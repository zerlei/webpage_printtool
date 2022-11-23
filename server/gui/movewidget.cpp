#include "movewidget.h"
#include  <QPainter>
#include <QStyleOption>

MoveWidget::MoveWidget(QWidget * parent /* = nullptr */) :
    QWidget(parent),
    mMoveWidget(this)
{
}
MoveWidget::~MoveWidget()
{

}
void MoveWidget::setMoveWidget(QWidget* moveWidget)
{
    mMoveWidget = moveWidget;
}
void MoveWidget::paintEvent(QPaintEvent*)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
void MoveWidget::mouseMoveEvent(QMouseEvent* event)
{
    if(isDoMove) {
        mMoveWidget->move(mMoveWidget->pos().x() + event->pos().x() - mMousePressPoint.x(),
            mMoveWidget->pos().y() + event->pos().y() - mMousePressPoint.y());
    }

}
void MoveWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button()==Qt::LeftButton)
    {
        isDoMove = true;
        mMousePressPoint = event->pos();
    }

    //return;
}
void MoveWidget::mouseReleaseEvent(QMouseEvent*)
{
    isDoMove = false;
    //return QWidget::mouseReleaseEvent(event);
}
void MoveWidget::mouseDoubleClickEvent(QMouseEvent*)
{
    //return QWidget::mouseDoubleClickEvent(event);
}
