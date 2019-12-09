// Basé sur :
// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#include "glarea.h"
#include <QDebug>
#include <QSurfaceFormat>
#include <QMatrix4x4>



GLArea::GLArea(QWidget *parent) :
    QOpenGLWidget(parent)
{
    QSurfaceFormat sf;
    sf.setDepthBufferSize(24);
    sf.setSamples(16);
    setFormat(sf);

    setEnabled(true);                   // événements clavier et souris
    setFocusPolicy(Qt::StrongFocus);    // accepte focus
    setFocus();                         // donne le focus
}


GLArea::~GLArea()
{
    // Contrairement aux méthodes virtuelles initializeGL, resizeGL et repaintGL,
    // dans le destructeur le contexte GL n'est pas automatiquement rendu courant.
    makeCurrent();
    tearGLObjects();
    doneCurrent();
}

void GLArea::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);

    makeGLObjects();

    // shader du sol
    program_lines = new QOpenGLShaderProgram(this);
    program_lines->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simple.vsh");
    program_lines->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.fsh");
    if (! program_lines->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << program_lines->log();
    }
    program_lines->setUniformValue("texture", 0);
}


void GLArea::makeGLObjects()
{
    float fsizeOfAxis = static_cast<float>(sizeOfAxis);

    vertices_box = {
       -fsizeOfAxis, 0, 0,
        fsizeOfAxis, 0, 0,

        0, -fsizeOfAxis, 0,
        0, fsizeOfAxis, 0,

        0, 0, -fsizeOfAxis,
        0, 0, fsizeOfAxis
    };

    rgb_box = {
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
    };

    /*
    // Version avec grille
    for(int i = -sizeOfAxis; i <= sizeOfAxis; i++){

        if(i == 0) continue;

        vertices_box.push_back(i);
        vertices_box.push_back(-sizeOfAxis);
        vertices_box.push_back(0);
        rgb_box.push_back(0.5f);
        rgb_box.push_back(0.5f);
        rgb_box.push_back(0.5f);

        vertices_box.push_back(i);
        vertices_box.push_back(sizeOfAxis);
        vertices_box.push_back(0);
        rgb_box.push_back(0.5f);
        rgb_box.push_back(0.5f);
        rgb_box.push_back(0.5f);

        // Y axis
        vertices_box.push_back(-sizeOfAxis);
        vertices_box.push_back(i);
        vertices_box.push_back(0);
        rgb_box.push_back(0.5f);
        rgb_box.push_back(0.5f);
        rgb_box.push_back(0.5f);

        vertices_box.push_back(sizeOfAxis);
        vertices_box.push_back(i);
        vertices_box.push_back(0);
        rgb_box.push_back(0.5f);
        rgb_box.push_back(0.5f);
        rgb_box.push_back(0.5f);

        // Z axis
        vertices_box.push_back(0);
        vertices_box.push_back(-sizeOfAxis);
        vertices_box.push_back(i);
        rgb_box.push_back(0.5f);
        rgb_box.push_back(0.5f);
        rgb_box.push_back(0.5f);

        vertices_box.push_back(0);
        vertices_box.push_back(sizeOfAxis);
        vertices_box.push_back(i);
        rgb_box.push_back(0.5f);
        rgb_box.push_back(0.5f);
        rgb_box.push_back(0.5f);

    }
*/
    // Version avec axes seulement
    for(int i = -sizeOfAxis; i <= sizeOfAxis; i++){

        if(i == 0) continue;

        // X axis
        vertices_box.push_back(i);
        vertices_box.push_back(-0.2f);
        vertices_box.push_back(0);
        rgb_box.push_back(1);
        rgb_box.push_back(0);
        rgb_box.push_back(0);

        vertices_box.push_back(i);
        vertices_box.push_back(0.2f);
        vertices_box.push_back(0);
        rgb_box.push_back(1);
        rgb_box.push_back(0);
        rgb_box.push_back(0);

        // Y axis
        vertices_box.push_back(-0.2f);
        vertices_box.push_back(i);
        vertices_box.push_back(0);
        rgb_box.push_back(0);
        rgb_box.push_back(1);
        rgb_box.push_back(0);

        vertices_box.push_back(0.2f);
        vertices_box.push_back(i);
        vertices_box.push_back(0);
        rgb_box.push_back(0);
        rgb_box.push_back(1);
        rgb_box.push_back(0);

        // Z axis
        vertices_box.push_back(0);
        vertices_box.push_back(-0.2f);
        vertices_box.push_back(i);
        rgb_box.push_back(0);
        rgb_box.push_back(0);
        rgb_box.push_back(1);

        vertices_box.push_back(0);
        vertices_box.push_back(0.2f);
        vertices_box.push_back(i);
        rgb_box.push_back(0);
        rgb_box.push_back(0);
        rgb_box.push_back(1);
    }


    QVector<GLfloat> vertData_axis;
    for (unsigned int i = 0; i < static_cast<unsigned int>(6+(sizeOfAxis*6*2)+2); ++i) {
        // coordonnées sommets
        for (unsigned int j = 0; j < 3; j++)
            vertData_axis.append(vertices_box[i*3+j]);
        // coordonnées box
        for (unsigned int j = 0; j < 3; j++)
            vertData_axis.append(rgb_box[i*3+j]);
    }

    vbo_axis.create();
    vbo_axis.bind();
    vbo_axis.allocate(vertData_axis.constData(), vertData_axis.count() * int(sizeof(GLfloat)));
    vertices_box.clear();
    rgb_box.clear();
}

void GLArea::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QVector<GLfloat> vertData_bresenheim;

    if(!baseLine.empty()){
        // Point de la ligne
        for (unsigned int i = 0; i < 2; ++i) {
            // coordonnées sommets
            for (unsigned int j = 0; j < 3; j++)
                vertData_bresenheim.push_back(static_cast<GLfloat>(baseLine[i*3+j]));
            // coordonnées box
            for (unsigned int j = 0; j < 3; j++)
                vertData_bresenheim.push_back(1);
        }
    }


    if(!bresenheimPointsX.empty() && !bresenheimPointsY.empty()){
        for(unsigned int i = 0; i < currentStep; i++){
            // Coords
            vertData_bresenheim.push_back(static_cast<GLfloat>(bresenheimPointsX[i]));
            vertData_bresenheim.push_back(static_cast<GLfloat>(bresenheimPointsY[i]));
            vertData_bresenheim.push_back(0);

            // Colors
            vertData_bresenheim.push_back(1.0);
            vertData_bresenheim.push_back(0.1f);
            vertData_bresenheim.push_back(0.1f);
        }
    }

    vbo_bresenheim.create();
    vbo_bresenheim.bind();
    vbo_bresenheim.allocate(vertData_bresenheim.constData(), vertData_bresenheim.count() * int(sizeof(GLfloat)));

    // Matrice de projection
    QMatrix4x4 projectionMatrix;
    projectionMatrix.perspective(45.0f, windowRatio, 1.0f, 1000.0f);

    // Matrice de vue (caméra)
    QMatrix4x4 viewMatrix;
    EspaceProjectif::translate(viewMatrix, {xPos, yPos, zPos});

    /* ESPACE PROJECTIF (COORDONNEES HOMOGENES)
    EspaceProjectif::rotate(viewMatrix, xRot, { 1, 0, 0 });
    EspaceProjectif::rotate(viewMatrix, yRot, { 0, 1, 0 });
    EspaceProjectif::rotate(viewMatrix, zRot, { 0, 0, 1 });
    */

    /* QUATERNION */
    Quaternion::rotate(viewMatrix, xRot, { 1, 0, 0 });
    Quaternion::rotate(viewMatrix, yRot, { 0, 1, 0 });
    Quaternion::rotate(viewMatrix, zRot, { 0, 0, 1 });

    vbo_axis.bind();
    program_lines->bind();

    QMatrix4x4 modelMatrixAxis;
    EspaceProjectif::translate(modelMatrixAxis, { 0.0f, 0.0f, 0.0f });
    program_lines->setUniformValue("projectionMatrix", projectionMatrix);
    program_lines->setUniformValue("viewMatrix", viewMatrix);
    program_lines->setUniformValue("modelMatrix", modelMatrixAxis);

    program_lines->setAttributeBuffer("in_position", GL_FLOAT, 0, 3, 6 * sizeof(GLfloat));
    program_lines->setAttributeBuffer("colAttr", GL_FLOAT, 3 * sizeof(GLfloat), 3, 6 * sizeof(GLfloat));
    program_lines->enableAttributeArray("in_position");
    program_lines->enableAttributeArray("colAttr");

    glLineWidth(2);
    glDrawArrays(GL_LINES, 0, 6+(sizeOfAxis*6*2));

    program_lines->disableAttributeArray("in_position");
    program_lines->disableAttributeArray("colAttr");

    program_lines->release();
    vbo_axis.release();

    vbo_bresenheim.bind();
    program_lines->bind();

    QMatrix4x4 modelMatrixBresenheim;
    EspaceProjectif::translate(modelMatrixBresenheim, { 0.0f, 0.0f, 0.0f });
    program_lines->setUniformValue("projectionMatrix", projectionMatrix);
    program_lines->setUniformValue("viewMatrix", viewMatrix);
    program_lines->setUniformValue("modelMatrix", modelMatrixBresenheim);

    program_lines->setAttributeBuffer("in_position", GL_FLOAT, 0, 3, 6 * sizeof(GLfloat));
    program_lines->setAttributeBuffer("colAttr", GL_FLOAT, 3 * sizeof(GLfloat), 3, 6 * sizeof(GLfloat));
    program_lines->enableAttributeArray("in_position");
    program_lines->enableAttributeArray("colAttr");

    int start = 0;

    if(!baseLine.empty()){
        glLineWidth(3);
        glDrawArrays(GL_LINES, start, start+2);
        start += 2;
    }

    if(!bresenheimPointsX.empty() && !bresenheimPointsY.empty()){
        glPointSize(10);
        glDrawArrays(GL_POINTS, start, start+static_cast<int>(currentStep));
        //start += currentStep;
    }

    program_lines->disableAttributeArray("in_position");
    program_lines->disableAttributeArray("colAttr");

    program_lines->release();
    vbo_bresenheim.release();
}


void GLArea::tearGLObjects()
{
    vbo_axis.destroy();
    vbo_bresenheim.destroy();
}


void GLArea::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    windowRatio = float(w) / h;
}

void GLArea::keyPressEvent(QKeyEvent *ev)
{
    float da = 0.1f;

    switch(ev->key()) {
    case Qt::Key_A :
        xRot -= da;
        break;
    case Qt::Key_Q :
        xRot += da;
        break;
    case Qt::Key_Z :
        yRot -= da;
        break;
    case Qt::Key_S :
        yRot += da;
        break;
    case Qt::Key_E :
        zRot -= da;
        break;
    case Qt::Key_D :
        zRot += da;
        break;
    case Qt::Key_Up:
        yPos -= 1;
        break;
    case Qt::Key_Left:
        xPos += 1;
        break;
    case Qt::Key_Right:
        xPos -= 1;
        break;
    case Qt::Key_Down:
        yPos += 1;
        break;
    }
    update();
}


void GLArea::mousePressEvent(QMouseEvent *ev)
{
    lastPos = ev->pos();
}

void GLArea::mouseMoveEvent(QMouseEvent *ev)
{
    int dx = ev->x() - lastPos.x();
    int dy = ev->y() - lastPos.y();

    if (ev->buttons() & Qt::LeftButton) {
        xRot += dy;
        yRot += dx;
        update();
    } else if (ev->buttons() & Qt::RightButton) {
        xPos += dx/10.0f;
        yPos -= dy/10.0f;
        update();
    } else if (ev->buttons() & Qt::MidButton) {
        xPos += dx/10.0f;
        zPos += dy;
        update();
    }

    lastPos = ev->pos();
}

std::vector<double> GLArea::getBresenheimPointsY() const
{
    return bresenheimPointsY;
}

void GLArea::setBresenheimPointsY(const std::vector<double> &value)
{
    bresenheimPointsY = value;
}

std::vector<double> GLArea::getBresenheimPointsX() const
{
    return bresenheimPointsX;
}

void GLArea::setBresenheimPointsX(const std::vector<double> &value)
{
    bresenheimPointsX = value;
}

std::vector<double> GLArea::getBaseLine() const
{
    return baseLine;
}

void GLArea::setBaseLine(const std::vector<double> &value)
{
    baseLine = value;
}

unsigned int GLArea::getCurrentStep() const
{
    return currentStep;
}

void GLArea::setCurrentStep(unsigned int value)
{
    currentStep = value;
}
