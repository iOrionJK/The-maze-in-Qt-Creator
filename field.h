// The header file of the maze grid

#ifndef FIELD_H
#define FIELD_H

#include <QPair>
#include "cell.h"
#include <QMessageLogger>
#include <QStack>
#include <random>
#include <QDebug>

class Field
{
public:
    Field(int rows, int columns, double length, int seed);
    ~Field();
    int rows;
    int columns;

    QPair<int, int> player, finish;
    Cell *** field;
    Cell* at(int row, int column);
    void genPrim();
    void genKruskal();
    void genHuntAndKill();
    void genRecursiveBacktracker();
    void genRecursiveDivision();
    void setPlayer(int row, int col);
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void genStartFinish();
    void trace();
    bool traced;
    QList<QPair<int, int>>* findNeighbours(int x, int y, bool isClosedCell);

private:
    int seed;
    std::mt19937 generator;
    int random();
};

#endif // FIELD_H
