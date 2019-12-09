#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void BresenhamComplete(int x1, int y1, int x2, int y2, QVector<double> *resultX, QVector<double> *resultY);

private:
    unsigned int currentStep = 0;
    QVector<double> xBresenham;
    QVector<double> yBresenham;
    QVector<double> kBresenham;

private slots:
    void on_execute_button_clicked();
    void on_continue_button_clicked();
};

#endif // MAINWINDOW_H
