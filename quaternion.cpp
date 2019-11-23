#include "quaternion.h"

float Quaternion::getW() const
{
    return w;
}
void Quaternion::setW(float value)
{
    w = value;
}
float Quaternion::getX() const
{
    return x;
}
void Quaternion::setX(float value)
{
    x = value;
}
float Quaternion::getY() const
{
    return y;
}
void Quaternion::setY(float value)
{
    y = value;
}
float Quaternion::getZ() const
{
    return z;
}
void Quaternion::setZ(float value)
{
    z = value;
}

Quaternion::Quaternion() : w(1.0f), x(0.0f), y(0.0f), z(0.0f) { }
Quaternion::Quaternion(float w, float x, float y, float z)
    : w(w), x(x), y(y), z(z) {}

void Quaternion::rotate(QMatrix4x4 &matrix, float angle, QVector3D axis)
{
    Quaternion q = angleAxis(angle, axis);
    QMatrix4x4 temp = q.toMatrix();
    matrix *= temp;
}

Quaternion Quaternion::angleAxis(float angle, QVector3D axis) {

    QVector3D normalized = axis.normalized();
    angle *= 0.0174532925f; // convert en radians
    angle *= 0.5f; // divisé par 2
    float sinAngle = sinf(angle);
    return Quaternion(cosf(angle),
                      normalized.x() * sinAngle,
                      normalized.y() * sinAngle,
                      normalized.z() * sinAngle);
}

QMatrix4x4 Quaternion::toMatrix()
{
    return QMatrix4x4(
                1-2*y*y-2*z*z,  2*x*y+2*w*z,    2*x*z-2*w*y,    0,
                2*x*y-2*w*z,    1-2*x*x-2*z*z,  2*y*z+2*w*x,    0,
                2*x*z+2*w*y,    2*y*z-2*w*x,    1-2*x*x-2*y*y,  0,
                0,              0,              0,              1
                );
}
