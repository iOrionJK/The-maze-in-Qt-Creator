// This window starts after selecting the settings

#ifndef FIELDWINDOW_H
#define FIELDWINDOW_H

#include <QWidget>
#include <QGraphicsScene>
#include "field.h"
#include <QKeyEvent>
#include <QCloseEvent>
#include <QMessageBox>
#include <QRandomGenerator>

QT_BEGIN_NAMESPACE
namespace Ui { class FieldWindow; }
QT_END_NAMESPACE

class FieldWindow : public QWidget
{
    Q_OBJECT
public:
    enum GenAlgorithm{
        RANDOM = 0,
        PRIM,
        KRUSKAL,
        HUNT_AND_KILL,
        RECURSIVE_BACKTRACKER,
        RECURSIVE_DIVISION
    };
    FieldWindow(double windowHeight, int rows, int columns, GenAlgorithm algorithm, int seed, QWidget *p, QWidget *parent = nullptr);
    ~FieldWindow();


private:
    Ui::FieldWindow *ui;
    QWidget *par;
    int windowHeight;
    int windowWidth;
    int rows;
    int columns;
    QGraphicsScene *scene;
    Field *field;
    bool boxCalled = false;
    void chooseGenAlgorithm(GenAlgorithm algorithm);

protected:
    void keyPressEvent(QKeyEvent *e) override;
    void closeEvent(QCloseEvent *event) override;


};

#endif // FIELDWINDOW_H

