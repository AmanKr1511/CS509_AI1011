#include "matrix.h"

void gemm_block(Matrix A,
                Matrix B,
                Matrix *C,
                int block_size)
{
    int i, j, k;
    int ii, jj, kk;
    int i_end, j_end, k_end;

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

    for (ii = 0; ii < A.rows; ii += block_size)
    {
        i_end = ii + block_size;
        if (i_end > A.rows)
            i_end = A.rows;

        for (jj = 0; jj < B.cols; jj += block_size)
        {
            j_end = jj + block_size;
            if (j_end > B.cols)
                j_end = B.cols;

            for (kk = 0; kk < A.cols; kk += block_size)
            {
                k_end = kk + block_size;
                if (k_end > A.cols)
                    k_end = A.cols;


                for (i = ii; i < i_end; i++)
                {
                    for (j = jj; j < j_end; j++)
                    {
                        for (k = kk; k < k_end; k++)
                        {
                            C->data[i][j] +=
                                A.data[i][k] * B.data[k][j];
                        }
                    }
                }
            }
        }
    }
}