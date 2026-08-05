#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Matrix Structure
typedef struct
{
    int rows;
    int cols;
    double **data;
} Matrix;

// CSR (Compressed Sparse Row) Structure
typedef struct
{
    int rows;
    int cols;
    int nnz;
    double *values;
    int *col_index;
    int *row_ptr;
} CSR;

// Allocate memory for a matrix
Matrix create_matrix(int rows, int cols);

// Free matrix memory
void free_matrix(Matrix *mat);

// Read matrix from file
void read_gemm_input(const char *filename, Matrix *A, Matrix *B);

// Print matrix
void print_matrix(Matrix mat);

// Fill matrix with random values
void random_matrix(Matrix *mat);

// Get execution time
double get_time(void);

//  Standard GEMM (gemm.c)
void gemm(Matrix A, Matrix B, Matrix *C);

// Blocked GEMM (gemm_block.c)

void gemm_block(Matrix A,
                Matrix B,
                Matrix *C,
                int block_size);

// CSR Functions

// Convert dense matrix into CSR format
CSR create_csr(Matrix mat);

// Print CSR arrays
void print_csr(CSR csr);

// Free CSR memory
void free_csr(CSR *csr);

#endif