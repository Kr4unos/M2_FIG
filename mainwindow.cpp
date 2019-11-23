#include "mainwindow.h"

#include <QDebug>
#include <QPen>
#include <QMessageBox>

#include "qcustomplot.h"
#include "glarea.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    this->setWindowTitle("M2GIG - Projet FIG");

    tabWidget->setTabText(0, "2D");
    tabWidget->setTabText(1, "3D");
    algoStep->setVisible(false);
}

bool MainWindow::Bresenham(int x0, int y0, int x1, int y1, QVector<double> *resultX, QVector<double> *resultY)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    if(0 > dy || dy > dx) return false;
    int k = 2*dy-dx;
    int y = y0;
    for(int x = x0; x <= x1; ++x){
        resultX->push_back(x);
        resultY->push_back(y);
        if(k > 0){
            ++y;
            k -= 2*dx;
        }
        k += 2*dy;
    }
    if(y == y1) return true;
    return false;
}
void MainWindow::BresenhamComplete(int x1, int y1, int x2, int y2, QVector<double> *resultX, QVector<double> *resultY){
    int dx, dy;
    if( (dx = x2 - x1) != 0 ){
        if(dx > 0){
            if( (dy = y2 - y1) != 0 ){
                if(dy > 0){
                    // Vecteur oblique dans le 1er quadran
                    if(dx >= dy) {
                        // Vecteur diagonal ou oblique proche de l'horizontale, dans le 1er octant
                        int e = dx;
                        dx = e * 2;
                        dy = dy * 2;
                        while(true){
                            resultX->push_back(x1);
                            resultY->push_back(y1);
                            if((x1++) == x2) break;
                            if( (e -= dy) < 0 ) {
                                ++y1;
                                e += dx;
                            }
                        }
                    } else {
                        // vecteur oblique proche de la verticale, dans le 2d octant
                        int e = dy;
                        dy = e * 2;
                        dx = dx * 2;
                        while(true){
                            resultX->push_back(x1);
                            resultY->push_back(y1);
                            if((y1++) == y2) break;
                            if( (e = e - dx) < 0 ) {
                                ++x1;
                                e += dy;
                            }
                        }
                    }
                } else {
                    // vecteur oblique dans le 4e cadran
                    if(dx >= -dy){
                        // vecteur diagonal ou oblique proche de l’horizontale, dans le 8e octant
                        int e = dx;
                        dx = e * 2;
                        dy = dy * 2;
                        while(true){
                            resultX->push_back(x1);
                            resultY->push_back(y1);
                            if( (x1++) == x2 ) break;
                            if( (e += dy) < 0 ) {
                                --y1;
                                e += dx;
                            }
                        }
                    } else {
                        // vecteur oblique proche de la verticale, dans le 7e octant
                        int e = dy;
                        dy = e * 2;
                        dx = dx * 2;
                        while(true){
                            resultX->push_back(x1);
                            resultY->push_back(y1);
                            if( (y1--) == y2 ) break;
                            if( (e -= dx) > 0 ) {
                                ++x1;
                                e += dy;
                            }
                        }
                    }
                }
            } else {
                do {
                    resultX->push_back(x1);
                    resultY->push_back(y1);
                } while( (x1++) == x2);
            }
        } else {
            if( (dy = y2 - y1) != 0 ){
                if(dy > 0){
                     // vecteur oblique dans le 2d quadran
                    if( -dx >= dy){
                        // vecteur diagonal ou oblique proche de l’horizontale, dans le 4e octant
                        int e = dx;
                        dx = e * 2;
                        dy = dy * 2;
                        while(true){
                            resultX->push_back(x1);
                            resultY->push_back(y1);
                            if( (x1--) == x2) break;
                            if( (e += dy) >= 0 ){
                                ++y1;
                                e += dx;
                            }
                        }
                    } else {
                        // vecteur oblique proche de la verticale, dans le 3e octant
                        int e = dy;
                        dy = e * 2;
                        dx = dx * 2;
                        while(true){
                            resultX->push_back(x1);
                            resultY->push_back(y1);
                            if( (y1--) == y2) break;
                            if( (e += dx) <= 0 ){
                                ++x1;
                                e += dy;
                            }
                        }
                    }
                } else {
                    // vecteur oblique dans le 3e cadran
                    if(dx <= dy){
                         // vecteur diagonal ou oblique proche de l’horizontale, dans le 5e octant
                        int e = dx;
                        dx = e * 2;
                        dy = dy * 2;
                        while(true){
                            resultX->push_back(x1);
                            resultY->push_back(y1);
                            if( (x1--) == x2) break;
                            if( (e -= dy) >= 0 ){
                                --y1;
                                e += dx;
                            }
                        }
                    } else {
                        // vecteur oblique proche de la verticale, dans le 6e octant
                        int e = dy;
                        dy = e * 2;
                        dx = dx * 2;
                        while(true){
                            resultX->push_back(x1);
                            resultY->push_back(y1);
                            if( (y1--) == y2) break;
                            if( (e -= dx) >= 0 ){
                                --x1;
                                e += dy;
                            }
                        }
                    }
                }
            } else {
                do {
                    resultX->push_back(x1);
                    resultY->push_back(y1);
                } while( (x1--) == x2);
            }
        }
    } else {
        if( (dy = y2 - y1) != 0){
            if(dy > 0){
                do {
                    resultX->push_back(x1);
                    resultY->push_back(y1);
                } while( (y1++) == y2);
            } else {
                do {
                    resultX->push_back(x1);
                    resultY->push_back(y1);
                } while( (y1--) == y2);
            }
        }
    }
}

void MainWindow::on_execute_button_clicked()
{
    currentStep = 0;
    xBresenham.clear();
    yBresenham.clear();
    continue_button->setText("Continuer");
    continue_button->setDisabled(false);


    BresenhamComplete(
                this->x0_spinBox->value(),
                this->y0_spinBox->value(),
                this->x1_spinBox->value(),
                this->y1_spinBox->value(),
                &xBresenham,
                &yBresenham
                );

    /*
     * 2D
     */
    customPlot->clearItems();
    customPlot->clearGraphs();
    customPlot->addGraph();
    customPlot->graph(0)->setPen(QPen(Qt::red));

    customPlot->xAxis->setSubTicks(false);
    customPlot->yAxis->setSubTicks(false);

    QSharedPointer<QCPAxisTickerFixed> fixedTicker(new QCPAxisTickerFixed);
    customPlot->xAxis->setTicker(fixedTicker);
    customPlot->yAxis->setTicker(fixedTicker);
    customPlot->yAxis->setScaleRatio(customPlot->xAxis,1.0);
    fixedTicker->setTickStep(1.0);
    fixedTicker->setScaleStrategy(QCPAxisTickerFixed::ssNone);// and no scaling of the tickstep (like multiples or powers) is allowed

    QVector<double> xLine(2), yLine(2);
    xLine[0] = this->x0_spinBox->value();
    xLine[1] = this->x1_spinBox->value();
    yLine[0] = this->y0_spinBox->value();
    yLine[1] = this->y1_spinBox->value();

    customPlot->graph(0)->setData(xLine, yLine);
    customPlot->graph(0)->rescaleAxes();

    int intCurrentStep = static_cast<int>(currentStep);

    QCPItemEllipse *xCircleItem = new QCPItemEllipse(this->customPlot);
    xCircleItem->setVisible(true);
    xCircleItem->setPen(QPen(Qt::blue));
    xCircleItem->setBrush(QBrush(QColor(0, 0, 255, 0)));
    xCircleItem->topLeft->setCoords(xBresenham[intCurrentStep]-0.1, yBresenham[intCurrentStep]+0.1);
    xCircleItem->bottomRight->setCoords(xBresenham[intCurrentStep]+0.1, yBresenham[intCurrentStep]-0.1);

    /*
    QCPItemRect *xRectItem = new QCPItemRect( this->customPlot );
    xRectItem->setVisible(true);
    xRectItem->setPen(QPen(Qt::blue));
    xRectItem->setBrush(QBrush(QColor(0, 0, 255, 20)));
    xRectItem->topLeft->setCoords(xBresenham[currentStep]-0.5, yBresenham[currentStep]+0.5);
    xRectItem->bottomRight->setCoords(xBresenham[currentStep]+0.5, yBresenham[currentStep]-0.5);
    */

    algoStep->setTitle(QString("Etape 1"));
    x_value->setText(QString::number(xBresenham[intCurrentStep]));
    y_value->setText(QString::number(yBresenham[intCurrentStep]));
    algoStep->setVisible(true);
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    /*
     * 3D
     */
    openGLWidget->setBaseLine({ xLine[0], yLine[0], 0, xLine[1], yLine[1], 0 });
    openGLWidget->setBresenheimPointsX(xBresenham.toStdVector());
    openGLWidget->setBresenheimPointsY(yBresenham.toStdVector());

    ++currentStep;
    openGLWidget->setCurrentStep(currentStep);

    customPlot->replot();
    openGLWidget->update();
}

void MainWindow::on_continue_button_clicked()
{
    int intCurrentStep = static_cast<int>(currentStep);
    if(intCurrentStep+1 >= xBresenham.size()){
        continue_button->setText("Terminé!");
        continue_button->setDisabled(true);
    }

    QCPItemEllipse *xCircleItem = new QCPItemEllipse(this->customPlot);
    xCircleItem->setVisible(true);
    xCircleItem->setPen(QPen(Qt::blue));
    xCircleItem->setBrush(QBrush(QColor(0, 0, 255, 0)));
    xCircleItem->topLeft->setCoords(xBresenham[intCurrentStep]-0.1, yBresenham[intCurrentStep]+0.1);
    xCircleItem->bottomRight->setCoords(xBresenham[intCurrentStep]+0.1, yBresenham[intCurrentStep]-0.1);

    /*
    QCPItemRect *xRectItem = new QCPItemRect( this->customPlot );
    xRectItem->setVisible(true);
    xRectItem->setPen(QPen(Qt::blue));
    xRectItem->setBrush(QBrush(QColor(0, 0, 255, 20)));
    xRectItem->topLeft->setCoords(xBresenham[currentStep]-0.5, yBresenham[currentStep]+0.5);
    xRectItem->bottomRight->setCoords(xBresenham[currentStep]+0.5, yBresenham[currentStep]-0.5);
    */

    this->customPlot->rescaleAxes();
    this->customPlot->replot();

    QString stepString = QString("Etape ") + QString::number(currentStep+1);
    algoStep->setTitle(stepString);
    x_value->setText(QString::number(xBresenham[intCurrentStep]));
    y_value->setText(QString::number(yBresenham[intCurrentStep]));
    ++currentStep;
    openGLWidget->setCurrentStep(currentStep);
    openGLWidget->update();
}
