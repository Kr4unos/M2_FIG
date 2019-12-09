#ifndef ESPACEPROJECTIF_H
#define ESPACEPROJECTIF_H

#include <QMatrix4x4>
#include <cmath>

class EspaceProjectif
{
public:
    static void translate(QMatrix4x4 &matrix, QVector3D position);
    static void rotate(QMatrix4x4 &matrix, float angle, QVector3D position);
    static void scale(QMatrix4x4 &matrix, QVector3D position);
    static QMatrix4x4 mult(QMatrix4x4 matrix1, QMatrix4x4 matrix2);
};

#endif // ESPACEPROJECTIF_H
