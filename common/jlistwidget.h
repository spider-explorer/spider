#ifndef JLISTWIDGET_H
#define JLISTWIDGET_H
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
class JListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit JListWidget(QWidget *parent = nullptr);
signals:
    void signal_mouseDoubleClick(QListWidgetItem *item,
                                 Qt::MouseButton button,
                                 Qt::KeyboardModifiers modifiers);
protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
};
#endif // JLISTWIDGET_H
