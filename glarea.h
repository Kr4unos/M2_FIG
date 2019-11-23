// Basé sur :
// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#ifndef GLAREA_H
#define GLAREA_H

#include <QKeyEvent>
#include <QTimer>
#include <QElapsedTimer>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include "quaternion.h"

class GLArea : public QOpenGLWidget,
               protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GLArea(QWidget *parent = nullptr);
    ~GLArea() override;

    unsigned int getCurrentStep() const;
    void setCurrentStep(unsigned int value);

    std::vector<double> getBaseLine() const;
    void setBaseLine(const std::vector<double> &value);

    std::vector<double> getBresenheimPointsX() const;
    void setBresenheimPointsX(const std::vector<double> &value);

    std::vector<double> getBresenheimPointsY() const;
    void setBresenheimPointsY(const std::vector<double> &value);

protected:
    void initializeGL() override;
    void doProjection();
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;

private:
    // Position
    float xRot = 20.0f, yRot = 0.0f, zRot = 0.0f;
    float xPos = 0.0f,  yPos = 0.0f, zPos = -20.0f;
    float windowRatio = 1.0f;
    QPoint lastPos;
    Quaternion quaternion;

    QOpenGLShaderProgram *program_lines;
    std::vector<GLfloat> vertices_box;
    std::vector<GLfloat> rgb_box;

    // AXIS
    QOpenGLBuffer vbo_axis;
    int sizeOfAxis = 20;

    // BRESENHEIM
    QOpenGLBuffer vbo_bresenheim;
    std::vector<double> baseLine;
    std::vector<double> bresenheimPointsX;
    std::vector<double> bresenheimPointsY;
    unsigned int currentStep = 0;

    void makeGLObjects();
    void tearGLObjects();
};

#endif // GLAREA_H
