#include "settingswindow.h"
#include "ui_settingswindow.h"
#include "fieldwindow.h"

SettingsWindow::SettingsWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
    this->setFixedSize(300, 205);
    validator = new QIntValidator(std::numeric_limits<qint32>::min(), std::numeric_limits<qint32>::max(), this);
    ui->seed->setValidator(validator);
    this->setFocus();
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
    if (w != NULL)
        delete w;
    delete validator;
}


void SettingsWindow::on_startButton_clicked()
{
    FieldWindow::GenAlgorithm algorithm;
    switch (ui->comboBox->currentIndex()) {
    case 0:
        algorithm = FieldWindow::GenAlgorithm::RANDOM;
        break;
    case 1:
        algorithm = FieldWindow::GenAlgorithm::PRIM;
        break;
    case 2:
        algorithm = FieldWindow::GenAlgorithm::KRUSKAL;
        break;
    case 3:
        algorithm = FieldWindow::GenAlgorithm::HUNT_AND_KILL;
        break;
    case 4:
        algorithm = FieldWindow::GenAlgorithm::RECURSIVE_BACKTRACKER;
        break;
    case 5:
        algorithm = FieldWindow::GenAlgorithm::RECURSIVE_DIVISION;
        break;
    default:
        algorithm = FieldWindow::GenAlgorithm::RANDOM;
    }
    w = new FieldWindow(ui->windowHeight->value(), ui->rows->value(), ui->columns->value(), algorithm,
                        (ui->seed->text() == "" || ui->seed->text() == "-") ? QRandomGenerator::global()->generate() : ui->seed->text().toInt(), this);
    w->show();
    this->hide();
}

