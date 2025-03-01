#include "cell.h"

Cell::Cell(double length, int borders)
{
    this->length = length;
    this->borders = borders;
    hasPlayer = false;
    hasFinish = false;
    hasTrace = false;
}

void Cell::setPlayer(bool is)
{
    hasPlayer = is;
    hasTrace = is;
    update();
}

void Cell::setFinish(bool is)
{
    hasFinish = is;
}

void Cell::setTrace(bool is)
{
    hasTrace = is;
    update();
}


void Cell::removeBorder(Border border)
{
    borders = borders & (~ border);
}


void Cell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(Qt::black);
    painter->setPen(Qt::NoPen);

    if (hasPlayer)
        painter->setBrush(Qt::red);
    else if (hasFinish)
        painter->setBrush(Qt::green);
    else if (hasTrace)
        painter->setBrush(QColor(196, 232, 248));
    else
        painter->setBrush(Qt::white);
    QRectF bg(length*(-0.01), length*(-0.01), length*(1.02), length*(1.02));
    painter->drawRect(bg);

    //conrers
    painter->setBrush(Qt::black);
    QRectF frameNW(length*(-0.02), length*(-0.02), length*0.07, length*0.07);
    painter->drawRect(frameNW);
    QRectF frameNE(length*0.95, length*(-0.02), length*0.07, length*0.07);
    painter->drawRect(frameNE);
    QRectF frameSW(length*(-0.02), length*0.95, length*0.07, length*0.07);
    painter->drawRect(frameSW);
    QRectF frameSE(length*0.95, length*0.95, length*0.07, length*0.07);
    painter->drawRect(frameSE);

    //borders
    if (borders & Border::UP){
        QRectF frame(length*0.03, length*(-0.02), length*0.94, length*0.07);
        painter->drawRect(frame);
    }
    if (borders & Border::RIGHT){
        QRectF frame(length*0.95, length*0.03, length*0.07, length*0.94);
        painter->drawRect(frame);
    }
    if (borders & Border::DOWN){
        QRectF frame(length*0.03, length*0.95, length*0.94, length*0.07);
        painter->drawRect(frame);
    }
    if (borders & Border::LEFT){
        QRectF frame(length*(-0.02), length*0.03, length*0.07, length*0.94);
        painter->drawRect(frame);
    }
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

QRectF Cell::boundingRect() const
{
    return QRectF(0, 0, length, length);
}
