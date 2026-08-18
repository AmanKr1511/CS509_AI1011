# CS509 — PGSL Assignments Overview

**Name:** Aman Kumar

**Entry Number:** 2026AIM1011

**Course:** CS509

**Language:** C and C++

**Platform:** Linux (GCC)

---

## Introduction

This repository contains two assignments completed for **CS509** by **Aman Kumar (2026AIM1011)**, implemented in C on a Linux/GCC platform. Both assignments explore efficient algorithmic techniques for handling matrix and graph data, with an emphasis on modular design, dynamic memory allocation, and performance measurement via execution timing.

**Assignment 1** focuses on matrix computation — implementing Standard GEMM, cache-optimized Blocked GEMM, and Compressed Sparse Row (CSR) representation for sparse matrices.

**Assignment 2** extends these ideas to graph algorithms, implementing Bellman-Ford (single-source shortest path, using a CSR-based adjacency list) and Floyd-Warshall (all-pairs shortest path, using a dense adjacency matrix), with cross-validation between the two.

**Assignment 3** focuses on Minimum Spanning Tree (MST) algorithms for weighted undirected graphs. It consists of two questions implementing Prim's Algorithm and Kruskal's Algorithm. Both algorithms are used to find an MST while minimizing the total edge weight, and their results and execution times are compared on the same input graphs.

## Repository Directory Layout

## Repository Directory Structure

```text
CS509_AIM1011/
│
├── Assignment_01/
│   ├── bin/
│   │
│   ├── src/
│   │   ├── csr.c
│   │   ├── driver.c
│   │   ├── gemm_block.c
│   │   ├── gemm.c
│   │   ├── matrix.h
│   │   └── utils.c
│   │
│   ├── tests/
│   │   ├── block_test1.txt
│   │   ├── block_test2.txt
│   │   ├── block_test3.txt
│   │   ├── csr_test1.txt
│   │   ├── csr_test2.txt
│   │   ├── csr_test3.txt
│   │   ├── gemm_test1.txt
│   │   ├── gemm_test2.txt
│   │   └── gemm_test3.txt
│   │
│   └── README.md
│
├── Assignment_02/
│   ├── bin/
│   │
│   ├── src/
│   │   ├── bellman_ford.cpp
│   │   ├── bellman_ford.hpp
│   │   ├── driver.cpp
│   │   ├── floyd_warshall.cpp
│   │   ├── floyd_warshall.hpp
│   │   ├── graph_csr.cpp
│   │   ├── graph_io.cpp
│   │   └── graph.hpp
│   │
│   ├── tests/
│   │   ├── bf_10.txt
│   │   ├── bf_100.txt
│   │   ├── bf_1000.txt
│   │   ├── bf_10000.txt
│   │   ├── bf_50000.txt
│   │   ├── bf_100000.txt
│   │   ├── fw_10.txt
│   │   ├── fw_100.txt
│   │   ├── fw_500.txt
│   │   ├── fw_1000.txt
│   │   └── fw_2000.txt
│   │
│   └── README.md
│
├── Assignment_03/
│   ├── bin/
│   │
│   ├── src/
│   │   ├── driver.cpp
│   │   ├── kruskal.cpp
│   │   ├── mst.hpp
│   │   └── prim.cpp
│   │
│   ├── tests/
│   │   ├── mst_10.txt
│   │   ├── mst_100.txt
│   │   ├── mst_10000.txt
│   │   ├── mst_50000.txt
│   │   └── mst_100000.txt
│   │
│   └── README.md
│
├── common_wrapper
├── common_wrapper.cpp
└── README.md