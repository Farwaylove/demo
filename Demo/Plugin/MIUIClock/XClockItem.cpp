#include <QTimer>
#include <QTime>
#include <QPainter>
#include <QtMath>
#include <QGraphicsSceneMouseEvent>
#include <QTransform>
#include <QStyleOptionGraphicsItem>

#include <QDebug>
#include "XClockItem.h"

XClockItem::XClockItem(QGraphicsItem *parent) : QGraphicsObject(parent)
{
    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    _timer->start(1);
}

void XClockItem::setSize(const QSize &size)
{
    if(_size != size) {
        _size = size;
        update();
    }
}

void XClockItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::NoBrush);

    QRect optionRect = option->rect;
    painter->fillRect(optionRect, Qt::transparent);

    int diameter = qMin(optionRect.width(), optionRect.height());
    QRect rctViewport((optionRect.width()-diameter)/2, (optionRect.height()-diameter)/2, diameter, diameter);
    painter->setViewport(rctViewport);
    QRect rctWindow(-120, -120, 240, 240);
    painter->setWindow(rctWindow);


    drawScale(painter);
    drawScaleLabels(painter);
    drawHourHand(painter);
    drawMinuteHand(painter);
    drawSecondHand(painter);
    drawKnot(painter);
}

QRectF XClockItem::boundingRect() const
{
    if(_size.isValid()) {
        return QRectF(QPointF(0,0), _size);
    } else {
        return QRectF(0, 0, 512, 512);
    }
}

void XClockItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF ptPressed = event->pos();
    QPointF ptCenter = boundingRect().center();
    QPointF ptOffset = ptPressed-ptCenter;
    QPointF ptOriginal;
    ptOriginal.setX(2*ptCenter.x() - ptPressed.x());
    ptOriginal.setY(2*ptCenter.y() - ptPressed.y());
    qreal bevel = qSqrt(qPow(ptOffset.rx(),2) + qPow(ptOffset.ry(),2));
    qreal cx = ptOffset.ry() / bevel;
    qreal cy = ptOffset.rx() / bevel;

    QTransform transform;
    transform.rotate(-15*cx, Qt::XAxis);
    transform.rotate(-15*cy, Qt::YAxis);
    setTransform(transform);
}

void XClockItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    QTransform transform;
    setTransform(transform);
}


// 计时器槽
void XClockItem::onTimer()
{
    QTime tm = QTime::currentTime();
    _hour = tm.hour();
    _minute = tm.minute();
    _second = tm.second();
    _milliSecond = tm.msecsSinceStartOfDay();

    update();
}

// 绘制中心圆环
void XClockItem::drawKnot(QPainter *painter)
{
    painter->save();

    painter->setPen(QPen(Qt::white, 3));
    painter->drawEllipse(-5, -5, 10, 10);

    painter->restore();
}

// 绘制刻度线
void XClockItem::drawScale(QPainter *painter)
{
    int tailLength = 90;
    int total = 360;
    int index = _milliSecond%(60*1000)*total/double(60*1000);
    for(int angle = 0; angle < total; angle++) {
        if(angle%2 == 1)    continue;
        painter->save();
        if(index >= tailLength) {
            if(angle < index+1 && angle > index-tailLength) {
                painter->setPen(QPen(QColor(255, 255, 255, 255+(120-255)*(index+1-angle)/double(tailLength)), 1));
            } else {
                painter->setPen(QPen(QColor(255, 255, 255, 120), 1));
            }
        } else {
            qreal critical = 0;
            if(angle >= 0 && angle < index+1) {
                critical = 255 + (120-255)*(index-angle)/double(tailLength);
                painter->setPen(QPen(QColor(255, 255, 255, critical), 1));
            } else if(angle > total-(tailLength-index)) {
                critical = 255 + (120-255)*index/double(tailLength);
                qreal tmp = critical + (120-critical)*(total-angle)/double(tailLength-index);
                painter->setPen(QPen(QColor(255, 255, 255, tmp), 1));
            } else {
                painter->setPen(QPen(QColor(255, 255, 255, 120), 1));
            }
        }
        painter->rotate(angle);

        if(0 == angle%30 ) {
            if(0 == angle%90) {
                painter->drawLine(0, -80, 0, -99);
            } else {
               painter->drawLine(0, -85, 0, -99);
            }
        } else {
            painter->drawLine(0, -90, 0, -99);
        }

        painter->restore();
    }
}

// 绘制时针
void XClockItem::drawHourHand(QPainter *painter)
{
    painter->save();

    int minutes = _hour*60 + _minute;
    painter->setPen(QPen(QColor(227, 227, 227, 255), 3, Qt::SolidLine, Qt::RoundCap));
    painter->rotate(minutes%(12*60)*360/double(12*60));
    painter->drawLine(0, -5, 0, -60);
    QPointF polygonPoints[4] = {
        QPointF(-2.5,-5),
        QPointF(2.5,-5),
        QPointF(1.5,-60),
        QPointF(-1.5,-60)
    };
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(227, 227, 227, 255));
    painter->drawPolygon(polygonPoints,4);

    painter->restore();
}

// 绘制分针
void XClockItem::drawMinuteHand(QPainter *painter)
{
    painter->save();

    painter->setPen(QPen(Qt::white, 2, Qt::SolidLine, Qt::RoundCap));
    painter->rotate((_milliSecond%(60*60*1000))*360/double(60*60*1000));
    painter->drawLine(0, -5, 0, -65);
    QPoint polygonPoints[4] = {
        QPoint(-2,-5),
        QPoint(2,-5),
        QPoint(1,-65),
        QPoint(-1,-65)
    };
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::white);
    painter->drawPolygon(polygonPoints,4);

    painter->restore();
}

// 绘制秒针
void XClockItem::drawSecondHand(QPainter *painter)
{
    QPointF polygonPoints[3] = {
        QPointF(0,-77),
        QPointF(-5,-68),
        QPointF(5,-68)
    };

    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::white);
    painter->rotate(_milliSecond%(60*1000)*360/double(60*1000));
    painter->drawPolygon(polygonPoints, 3);
    painter->restore();
}

// 绘制刻度标签
void XClockItem::drawScaleLabels(QPainter *painter)
{
    painter->save();

    qreal angle = qRadiansToDegrees(qAtan2(10,100));
    painter->setPen(QPen(QColor(255, 255, 255, 120)));
    painter->drawArc(-110,-110,220,220, angle*16, (90-2*angle)*16);
    painter->drawArc(-110,-110,220,220, (90+angle)*16, (90-2*angle)*16);
    painter->drawArc(-110,-110,220,220, (180+angle)*16, (90-2*angle)*16);
    painter->drawArc(-110,-110,220,220, (270+angle)*16, (90-2*angle)*16);

    painter->setFont(QFont("微软雅黑",8));
    painter->drawText(-10,-120,20,20, Qt::AlignCenter, tr("12"));
    painter->drawText(100,-10,20,20, Qt::AlignCenter, tr("3"));
    painter->drawText(-10,100,20,20, Qt::AlignCenter, tr("6"));
    painter->drawText(-120,-10,20,20, Qt::AlignCenter, tr("9"));

    painter->setCompositionMode(QPainter::CompositionMode_Source);

    painter->restore();
}
