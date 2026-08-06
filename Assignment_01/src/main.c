#include "matrix.h"

#define BLOCK_SIZE 32

int main(void)
{
    int choice;
    char filename[100];
    char filepath[150];

    const char *TEST_DIR = "tests/";

    Matrix A, B;
    Matrix C;

    CSR csr;

    double start;
    double end;
    double execution_time;

    while (1)
    {
        printf("\n=====================================\n");
        printf("        MATRIX PROJECT MENU\n");
        printf("=====================================\n");
        printf("1. Standard Matrix Multiplication\n");
        printf("2. Blocked Matrix Multiplication\n");
        printf("3. Matrix A to CSR\n");
        printf("4. Exit\n");
        printf("=====================================\n");

        printf("Enter your choice : ");

        if (scanf("%d", &choice) != 1)
        {
            printf("\nInvalid input.\n");

            while (getchar() != '\n')
                ;

            continue;
        }

        switch (choice)
        {


        case 1:

            printf("\nEnter GEMM input file name (from tests/) : ");

            if (scanf("%99s", filename) != 1)
            {
                printf("\nInvalid input.\n");
                break;
            }

            snprintf(filepath, sizeof(filepath), "%s%s", TEST_DIR, filename);

            read_gemm_input(filepath, &A, &B);

            start = get_time();

            gemm(A, B, &C);

            end = get_time();

            execution_time = end - start;

            printf("\nAlgorithm : GEMM Simple\n");
            printf("\nResult Matrix\n");

            print_matrix(C);

            printf("\nExecution Time : %.6lf seconds\n",
                   execution_time);

            free_matrix(&A);
            free_matrix(&B);
            free_matrix(&C);

            break;

        case 2:

            printf("\nEnter GEMM input file name (from tests/) : ");

            if (scanf("%99s", filename) != 1)
            {
                printf("\nInvalid input.\n");
                break;
            }

            snprintf(filepath, sizeof(filepath), "%s%s", TEST_DIR, filename);

            read_gemm_input(filepath, &A, &B);

            start = get_time();

            gemm_block(A,
                       B,
                       &C,
                       BLOCK_SIZE);

            end = get_time();

            execution_time = end - start;

            printf("\nAlgorithm : GEMM Blocking\n");
            printf("\nResult Matrix\n");

            print_matrix(C);

            printf("\nBlock Size : %d\n", BLOCK_SIZE);

            printf("Execution Time : %.6lf seconds\n",
                   execution_time);

            free_matrix(&A);
            free_matrix(&B);
            free_matrix(&C);

            break;

        case 3:

            printf("\nEnter GEMM input file name (from tests/) : ");

            if (scanf("%99s", filename) != 1)
            {
                printf("\nInvalid input.\n");
                break;
            }

            snprintf(filepath, sizeof(filepath), "%s%s", TEST_DIR, filename);

            read_matrix_input(filepath, &A);

            start = get_time();

            csr = create_csr(A);

            end = get_time();

            execution_time = end - start;

            printf("\nMatrix A\n");
            print_matrix(A);

            print_csr(csr);

            printf("\nExecution Time : %.6lf seconds\n",
                   execution_time);

            free_matrix(&A);
            free_csr(&csr);

            break;

        case 4:

            printf("\nExiting...\n");

            return 0;

        default:

            printf("\nInvalid choice.\n");
        }
    }

    return 0;
}