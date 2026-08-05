#include "matrix.h"

Matrix create_matrix(int rows, int cols)
{
    Matrix mat;
    int i;

    mat.rows = rows;
    mat.cols = cols;

    mat.data = (double **)malloc(rows * sizeof(double *));
    if (mat.data == NULL)
    {
        printf("Error: Unable to allocate memory for matrix rows.\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < rows; i++)
    {
        mat.data[i] = (double *)malloc(cols * sizeof(double));

        if (mat.data[i] == NULL)
        {
            printf("Error: Unable to allocate memory for matrix columns.\n");

            while (--i >= 0)
                free(mat.data[i]);

            free(mat.data);
            exit(EXIT_FAILURE);
        }
    }

    return mat;
}

void free_matrix(Matrix *mat)
{
    int i;

    if (mat == NULL || mat->data == NULL)
        return;

    for (i = 0; i < mat->rows; i++)
        free(mat->data[i]);

    free(mat->data);

    mat->data = NULL;
    mat->rows = 0;
    mat->cols = 0;
}

void read_gemm_input(const char *filename, Matrix *A, Matrix *B)
{
    FILE *fp;

    int M;
    int K;
    int N;

    int i;
    int j;

    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Unable to open input file.\n");
        exit(EXIT_FAILURE);
    }

    if (fscanf(fp, "%d %d %d", &M, &K, &N) != 3)
    {
        printf("Invalid GEMM input format.\n");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    *A = create_matrix(M, K);
    *B = create_matrix(K, N);

    /* Read Matrix A */

    for (i = 0; i < M; i++)
    {
        for (j = 0; j < K; j++)
        {
            fscanf(fp, "%lf", &A->data[i][j]);
        }
    }

    /* Read Matrix B */

    for (i = 0; i < K; i++)
    {
        for (j = 0; j < N; j++)
        {
            fscanf(fp, "%lf", &B->data[i][j]);
        }
    }

    fclose(fp);
}

// Print matrix
void print_matrix(Matrix mat)
{
    int i, j;

    printf("\n");

    for (i = 0; i < mat.rows; i++)
    {
        for (j = 0; j < mat.cols; j++)
        {
            printf("%8.2lf ", mat.data[i][j]);
        }

        printf("\n");
    }

    printf("\n");
}

void random_matrix(Matrix *mat)
{
    int i, j;

    if (mat == NULL || mat->data == NULL)
        return;

    srand((unsigned int)time(NULL));

    for (i = 0; i < mat->rows; i++)
    {
        for (j = 0; j < mat->cols; j++)
        {
            mat->data[i][j] = rand() % 10;
        }
    }
}

double get_time(void)
{
    return (double)clock() / CLOCKS_PER_SEC;
}