#ifndef  __MoveWidget_
#define __MoveWidget_
#include<QWidget>
#include<QMouseEvent>

class MoveWidget : public QWidget
{
    Q_OBJECT
public:
    MoveWidget(QWidget* parent = nullptr) ;
    virtual~MoveWidget();
    void setMoveWidget(QWidget* moveWidget);
public:
    void paintEvent(QPaintEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseDoubleClickEvent(QMouseEvent* event);
protected:
    QWidget* mMoveWidget;
private:
    QPoint mMousePressPoint;
    bool isDoMove{false};


};
#endif // ! __MoveWidget_
