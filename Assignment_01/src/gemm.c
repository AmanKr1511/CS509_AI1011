#include "matrix.h"

void gemm(Matrix A, Matrix B, Matrix *C)
{
    int i, j, k;

    if (A.cols != B.rows)
    {
        printf("Error: Matrix multiplication not possible.\n");
        exit(EXIT_FAILURE);
    }

    *C = create_matrix(A.rows, B.cols);

    for (i = 0; i < C->rows; i++)
    {
        for (j = 0; j < C->cols; j++)
        {
            C->data[i][j] = 0.0;
        }
    }

    for (i = 0; i < A.rows; i++)
    {
        for (j = 0; j < B.cols; j++)
        {
            for (k = 0; k < A.cols; k++)
            {
                C->data[i][j] +=
                    A.data[i][k] * B.data[k][j];
            }
        }
    }
}