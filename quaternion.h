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
    Quaternion angleAxis(float angle, QVector3D axis);
    QMatrix4x4 toMatrix();

    void translate(QMatrix4x4 &matrix, QVector3D position);
    void rotate(QMatrix4x4 &matrix, float angle, QVector3D position);

    float getW() const;
    void setW(float value);
    float getX() const;
    void setX(float value);
    float getY() const;
    void setY(float value);
    float getZ() const;
    void setZ(float value);
};

#endif // QUATERNION_H
