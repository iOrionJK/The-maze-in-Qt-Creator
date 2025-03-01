// It's the first window

#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QMainWindow>
#include "fieldwindow.h"
#include <QIntValidator>
#include <QRandomGenerator>

QT_BEGIN_NAMESPACE
namespace Ui { class SettingsWindow; }
QT_END_NAMESPACE

class SettingsWindow : public QMainWindow
{
    Q_OBJECT

public:
    SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow();

private slots:
    void on_startButton_clicked();

private:
    Ui::SettingsWindow *ui;
    FieldWindow *w = NULL;
    QIntValidator *validator;
};
#endif // SETTINGSWINDOW_H
