#include "espaceprojectif.h"


void EspaceProjectif::translate(QMatrix4x4 &matrix, QVector3D position)
{
   QMatrix4x4 temp(
        1, 0, 0, position.x(),
        0, 1, 0, position.y(),
        0, 0, 1, position.z(),
        0, 0, 0, 1
   );
   matrix = mult(matrix, temp);
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
    matrix = mult(matrix, temp);
}

void EspaceProjectif::scale(QMatrix4x4 &matrix, QVector3D position){
    QMatrix4x4 temp(
         position.x(), 0, 0, 0,
         0, position.y(), 0, 0,
         0, 0, position.z(), 0,
         0, 0, 0, 1
    );
    matrix = mult(matrix, temp);
}

QMatrix4x4 EspaceProjectif::mult(QMatrix4x4 matrix1, QMatrix4x4 matrix2){

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
