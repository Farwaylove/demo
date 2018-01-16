#include <QGraphicsScene>
#include <QMouseEvent>
#include <QPainter>
#include <QtMath>

#include <QDebug>
#include "XMiuiClock.h"
#include "XClockItem.h"

XMiuiClock::XMiuiClock(QWidget *parent)
    : QGraphicsView(parent)
{
    _item = new XClockItem;
    _scene = new QGraphicsScene(rect());
    _scene->addItem(_item);
    _scene->setBackgroundBrush(QColor("dodgerblue"));
    setScene(_scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setStyleSheet("border");
    setVisible(false);
}

void XMiuiClock::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    _scene->setSceneRect(rect());
    _item->setSize(rect().size());
}

void  XMiuiClock::Display()
{
    this->show();
}


void  XMiuiClock::Close()
{
    this->hide();
}

CxIDemo* CreateDemo(QWidget *parent)
{
    XMiuiClock *demo = new XMiuiClock(parent);
    QLayout    *layout = parent->layout();
    if (layout)
    {
        layout->addWidget(demo);
    }
    return demo;
}

void ReleaseDemo(CxIDemo *demo)
{
    if (demo) delete (XMiuiClock*)demo;
    demo = Q_NULLPTR;
}

const QString& GetDemoName()
{
    static QString PluginName = QStringLiteral("MIUI Clock");

    return PluginName;
}
