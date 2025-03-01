#include "fieldwindow.h"
#include "ui_fieldwindow.h"
#include "cell.h"

FieldWindow::FieldWindow(double windowHeight, int rows, int columns, GenAlgorithm algorithm, int seed, QWidget *p, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FieldWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Maze " + QString::number(rows) + "x" + QString::number(columns) + " | Seed: " + QString::number(seed));
    this->grabKeyboard();
    this->par = p;
    this->windowHeight = windowHeight > 900 ? 900 : windowHeight;
    this->windowWidth = this->windowHeight / rows * columns;
    this->rows = rows;
    this->columns = columns;
    //this->resize(windowWidth + this->windowHeight/rows, this->windowHeight + this->windowHeight/rows*0.2);
    this->setWindowState(Qt::WindowMaximized);

    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::RenderHint::Antialiasing);
    //ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scene->setSceneRect(0,0,windowHeight / rows * columns,windowHeight);
    scene->addRect(QRect(-windowHeight/rows * 0.05, -windowHeight/rows * 0.05, windowHeight / rows * columns + windowHeight/rows * 0.1, windowHeight + windowHeight/rows * 0.1), QPen(Qt::black), QBrush(Qt::black));
    scene->addRect(QRect(0, 0, windowHeight / rows * columns, windowHeight), QPen(Qt::black), QBrush(Qt::white));

    field = new Field(rows, columns, windowHeight/rows, seed);
    for(int row = 0; row<rows; row++){
        for(int column=0; column < columns; column++){
            scene->addItem(field->at(row, column));
        }
    }

    chooseGenAlgorithm(algorithm);

}

FieldWindow::~FieldWindow()
{
    delete ui;
    delete scene;
    delete field;
}

void FieldWindow::chooseGenAlgorithm(GenAlgorithm algorithm)
{
    switch (algorithm) {
    case GenAlgorithm::PRIM:
        field->genPrim();
        break;
    case GenAlgorithm::KRUSKAL:
        field->genKruskal();
        break;
    case GenAlgorithm::HUNT_AND_KILL:
        field->genHuntAndKill();
        break;
    case GenAlgorithm::RECURSIVE_BACKTRACKER:
        field->genRecursiveBacktracker();
        break;
    case GenAlgorithm::RECURSIVE_DIVISION:
        field->genRecursiveDivision();
        break;
    default:
        int r = QRandomGenerator::global()->generate()%5 + 1;
        chooseGenAlgorithm((GenAlgorithm)r);
    }
}

void FieldWindow::keyPressEvent(QKeyEvent *e)
{
    if (boxCalled)
        return;
    switch (e->key()) {
    case Qt::Key_Left:
        field->moveLeft();
        break;
    case Qt::Key_Right:
        field->moveRight();
        break;
    case Qt::Key_Up:
        field->moveUp();
        break;
    case Qt::Key_Down:
        field->moveDown();
        break;
    case Qt::Key_T:
        field->trace();
        break;
    case Qt::Key_Plus:
        ui->graphicsView->scale(1.25, 1.25);
        break;
    case Qt::Key_Minus:
        ui->graphicsView->scale(0.8, 0.8);
        break;
    }
    if (field->player.first == field->finish.first && field->player.second == field->finish.second){
        boxCalled = true;
        QMessageBox *box = new QMessageBox();
        box->setText("The maze is completed! Try again?");
        box->setWindowTitle("Solved!");
        box->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        box->setModal(true);
        int ret = box->exec();
        boxCalled = false;
        switch (ret) {
        case QMessageBox::Ok:
            this->close();
            break;
        case QMessageBox::Cancel:
            this->close();
            par->close();
            break;
        }
        delete box;
    }
}

void FieldWindow::closeEvent(QCloseEvent *event)
{
    par->show();
    this->close();
    Q_UNUSED(event);
}
