#include "matrix.h"

CSR create_csr(Matrix mat)
{
    CSR csr;

    int i;
    int j;
    int count = 0;


    for (i = 0; i < mat.rows; i++)
    {
        for (j = 0; j < mat.cols; j++)
        {
            if (mat.data[i][j] != 0.0)
            {
                count++;
            }
        }
    }

    csr.rows = mat.rows;
    csr.cols = mat.cols;
    csr.nnz = count;

    csr.values = (double *)malloc(count * sizeof(double));
    csr.col_index = (int *)malloc(count * sizeof(int));
    csr.row_ptr = (int *)malloc((mat.rows + 1) * sizeof(int));

    if (csr.values == NULL ||
        csr.col_index == NULL ||
        csr.row_ptr == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    count = 0;

    for (i = 0; i < mat.rows; i++)
    {
        csr.row_ptr[i] = count;

        for (j = 0; j < mat.cols; j++)
        {
            if (mat.data[i][j] != 0.0)
            {
                csr.values[count] = mat.data[i][j];
                csr.col_index[count] = j;

                count++;
            }
        }
    }

    csr.row_ptr[mat.rows] = count;

    return csr;
}

void print_csr(CSR csr)
{
    int i;

    printf("\nCSR Representation\n");
    printf("-----------------------------\n");

    printf("\nValues:\n");

    for (i = 0; i < csr.nnz; i++)
    {
        printf("%.2lf ", csr.values[i]);
    }

    printf("\n");

    printf("\nColumn Index:\n");

    for (i = 0; i < csr.nnz; i++)
    {
        printf("%d ", csr.col_index[i]);
    }

    printf("\n");

    printf("\nRow Pointer:\n");

    for (i = 0; i <= csr.rows; i++)
    {
        printf("%d ", csr.row_ptr[i]);
    }

    printf("\n");
}

void free_csr(CSR *csr)
{
    if (csr == NULL)
    {
        return;
    }

    free(csr->values);
    free(csr->col_index);
    free(csr->row_ptr);

    csr->values = NULL;
    csr->col_index = NULL;
    csr->row_ptr = NULL;

    csr->rows = 0;
    csr->cols = 0;
    csr->nnz = 0;
}