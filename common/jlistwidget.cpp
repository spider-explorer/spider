#include "jlistwidget.h"
JListWidget::JListWidget(QWidget *parent) : QListWidget(parent)
{
}
void JListWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    Qt::MouseButton button = event->button();
    if (button != Qt::LeftButton && button != Qt::MiddleButton && button != Qt::RightButton)
        return;
    qDebug() << button;
    Qt::KeyboardModifiers modifiers = event->modifiers();
    qDebug() << modifiers;
    QListWidgetItem *item = this->itemAt(event->pos());
    emit signal_mouseDoubleClick(item, button, modifiers);
}
