#include "espaceprojectif.h"


void EspaceProjectif::translate(QMatrix4x4 &matrix, QVector3D position)
{
   QMatrix4x4 temp(
        1, 0, 0, position.x(),
        0, 1, 0, position.y(),
        0, 0, 1, position.z(),
        0, 0, 0, 1
   );
   matrix *= temp;
}

void EspaceProjectif::rotate(QMatrix4x4 &matrix, float angle, QVector3D position)
{
    angle *= 0.0174532925f; // conversion en radians (1° = 1 * M_PI/180)
    QMatrix4x4 temp;

    if(position.x() > 0) {

        temp.setRow(0, { 1, 0, 0, 0 });
        temp.setRow(1, { 0, cosf(angle), -sinf(angle), 0 });
        temp.setRow(2, { 0, sinf(angle), cosf(angle), 0 });
        temp.setRow(3, { 0, 0, 0, 1 });

    } else if(position.y() > 0){

        temp.setRow(0, { cosf(angle), 0, sinf(angle), 0 });
        temp.setRow(1, { 0, 1, 0, 0 });
        temp.setRow(2, { -sinf(angle), 0, cosf(angle), 0 });
        temp.setRow(3, { 0, 0, 0, 1 });

    } else if(position.z() > 0){

        temp.setRow(0, { cosf(angle), -sinf(angle), 0, 0 });
        temp.setRow(1, { sinf(angle), cosf(angle), 0, 0 });
        temp.setRow(2, { 0, 0, 1, 0 });
        temp.setRow(3, { 0, 0, 0, 1 });
    }
    matrix *= temp;
}
