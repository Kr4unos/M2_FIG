#include "quaternion.h"

Quaternion::Quaternion() : w(1.0f), x(0.0f), y(0.0f), z(0.0f) { }
Quaternion::Quaternion(float w, float x, float y, float z)
    : w(w), x(x), y(y), z(z) {}

void Quaternion::rotate(QMatrix4x4 &matrix, float angle, QVector3D axis)
{
    Quaternion q = angleAxis(angle, axis);
    QMatrix4x4 temp = q.toMatrix();
    matrix = mult(matrix, temp);
}

Quaternion Quaternion::angleAxis(float angle, QVector3D axis) {

    QVector3D normalized = axis.normalized();
    angle *= 0.0174532925f; // conversion en radians (1° = 1 * M_PI/180)
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

QMatrix4x4 Quaternion::mult(QMatrix4x4 matrix1, QMatrix4x4 matrix2){

    std::vector<float> result;

    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++){
            result.push_back(
                        matrix1.row(i)[0] * matrix2.column(j)[0] + matrix1.row(i)[1] * matrix2.column(j)[1] +
                        matrix1.row(i)[2] * matrix2.column(j)[2] + matrix1.row(i)[3] * matrix2.column(j)[3]
                    );
        }
    }

    return QMatrix4x4(
                result[0], result[1], result[2], result[3],
                result[4], result[5], result[6], result[7],
                result[8], result[9], result[10], result[11],
                result[12], result[13], result[14], result[15]
            );
}
