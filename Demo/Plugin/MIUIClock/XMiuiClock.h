#ifndef XMiuiClock_H
#define XMiuiClock_H

#include <QGraphicsView>
#include <QLayout>
#include "CxIDemo.h"

class QGraphicsScene;
class XClockItem;

class XMiuiClock : public QGraphicsView, public CxIDemo
{
    Q_OBJECT

public:
    XMiuiClock(QWidget *parent = Q_NULLPTR);
public:
    void  Display();
    void  Close();

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    QGraphicsScene *_scene;
    XClockItem *_item;
};

extern "C"
{
    Q_DECL_EXPORT CxIDemo *CreateDemo(QWidget  *parent);
    Q_DECL_EXPORT void     ReleaseDemo(CxIDemo *demo);
    Q_DECL_EXPORT const    QString& GetDemoName();
}

#endif // XMiuiClock_H
