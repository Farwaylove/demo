#ifndef XClockItem_H
#define XClockItem_H

#include <QGraphicsObject>

class XClockItem : public QGraphicsObject
{
    Q_OBJECT

public:
    XClockItem(QGraphicsItem *parent = Q_NULLPTR);
    void setSize(const QSize &size);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onTimer();

private:
    void drawKnot(QPainter *painter);
    void drawScale(QPainter *painter);
    void drawHourHand(QPainter *painter);
    void drawMinuteHand(QPainter *painter);
    void drawSecondHand(QPainter *painter);
    void drawScaleLabels(QPainter *painter);

private:
    QTimer *_timer;
    unsigned int _hour;
    unsigned int _minute;
    unsigned int _second;
    unsigned int _milliSecond;
    QSize _size;
};

#endif // XClockItem_H
