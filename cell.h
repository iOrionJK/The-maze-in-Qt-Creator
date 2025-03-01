// The header file of the maze grid cell

#ifndef CELL_H
#define CELL_H

#include <QGraphicsItem>
#include <QPainter>

class Cell : public QGraphicsItem
{
public:
    Cell(double length, int borders);
    enum Border{
        UP = 1,
        RIGHT = 2,
        DOWN = 4,
        LEFT = 8
    };
    int borders;
    bool hasPlayer;
    bool hasFinish;
    bool hasTrace;
    void setPlayer(bool is);
    void setFinish(bool is);
    void setTrace(bool is);
    void removeBorder(Border border);
    QBrush innerColor;

protected:
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    QRectF boundingRect() const;

private:
    double length;


};

#endif // CELL_H
