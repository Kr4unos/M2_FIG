#ifndef QUATERNION_H
#define QUATERNION_H


#include <QMatrix4x4>
#include <cmath>

class Quaternion
{
private:
    float w, x, y, z;
public:
    Quaternion();
    Quaternion(float w, float x, float y, float z);
    QMatrix4x4 toMatrix();

    static void translate(QMatrix4x4 &matrix, QVector3D position);
    static void rotate(QMatrix4x4 &matrix, float angle, QVector3D position);
    static Quaternion angleAxis(float angle, QVector3D axis);
    static QMatrix4x4 mult(QMatrix4x4 matrix1, QMatrix4x4 matrix2);
};

#endif // QUATERNION_H
