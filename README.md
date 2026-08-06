# Matrix Multiplication and CSR Representation

**Course:** CS509
**Assignment:** 01
**Name:** AMAN KUMR
**Entry Number:** 2026AIM1011
**Language:** C
**Platform:** Linux (GCC)

---

## 1. Introduction

This project implements three fundamental matrix-processing algorithms in C using a modular design. The objectives are to understand dense matrix multiplication, optimize multiplication using cache blocking, and efficiently represent sparse matrices using the Compressed Sparse Row (CSR) format. The project also measures execution time to compare algorithm performance.

## 2. Objectives

- Implement Standard General Matrix Multiplication (GEMM).
- Implement Blocked GEMM for improved cache utilization.
- Implement CSR representation for sparse matrices.
- Use dynamic memory allocation.
- Develop modular and reusable C source files.
- Measure execution time for performance comparison.

## 3. Project Structure

```
Assignment_01/
├── src/
│   ├── main.c
│   ├── gemm.c
│   ├── gemm_block.c
│   ├── csr.c
│   ├── utils.c
│   └── matrix.h
├── test/
├── results/
├── wrapper.sh
└── README.md
```

## 4. Module Description

### Standard GEMM

Uses the classical triple nested loop algorithm to compute C = A × B.
Time complexity: O(M×N×K).

### Blocked GEMM

Divides matrices into smaller blocks to improve cache locality and reduce cache misses while producing the same mathematical result as Standard GEMM.

### CSR Representation

Stores only non-zero values of a sparse matrix using three arrays: Values, Column Indices, and Row Pointer, reducing memory usage.

## 5. Compilation and Execution

The project uses a Linux wrapper script instead of a Makefile.

```bash
chmod +x wrapper.sh
./wrapper.sh
```

## 6. Input and Output

Input files are stored in the `test/` directory. The user enters only the filename, and the program automatically loads the file. Generated outputs, including execution time, are saved in the `results/` directory.

## 7. Test Cases

| Algorithm     | Test Files                                          | Description                |
|---------------|------------------------------------------------------|-----------------------------|
| Standard GEMM | `gemm_test1.txt`, `gemm_test2.txt`, `gemm_test3.txt` | Dense matrix multiplication |
| Blocked GEMM  | `block_test1.txt`, `block_test2.txt`, `block_test3.txt` | Blocked multiplication   |
| CSR           | `csr_test1.txt`, `csr_test2.txt`, `csr_test3.txt`    | Sparse matrix conversion    |

## 8. Execution Time Record

| S.No. | Algorithm     | Test File         | Execution Time (s) |
|-------|---------------|--------------------|---------------------|
| 1     | Standard GEMM | `gemm_test1.txt`   | 0.000007            |
| 2     | Standard GEMM | `gemm_test2.txt`   | 0.000006            |
| 3     | Standard GEMM | `gemm_test3.txt`   | 0.000004            |
| 4     | Blocked GEMM  | `block_test1.txt`  | 0.000003            |
| 5     | Blocked GEMM  | `block_test2.txt`  | 0.000007            |
| 6     | Blocked GEMM  | `block_test3.txt`  | 0.000005            |
| 7     | CSR           | `csr_test1.txt`    | 0.000003            |
| 8     | CSR           | `csr_test2.txt`    | 0.000004            |
| 9     | CSR           | `csr_test3.txt`    | 0.000003            |

## 9. Time Complexity

| Algorithm      | Complexity | Remarks                        |
|-----------------|------------|---------------------------------|
| Standard GEMM   | O(M×N×K)   | Baseline implementation         |
| Blocked GEMM    | O(M×N×K)   | Improved cache performance      |
| CSR Conversion  | O(R×C)     | Single traversal of matrix      |

## 10. Conclusion

The project successfully demonstrates modular implementation of dense matrix multiplication, cache-aware blocked multiplication, and sparse matrix storage using CSR. Execution-time measurements collected during testing can be used to compare the practical performance of the implemented algorithms.
