# CS509 — Assignment 3: Minimum Spanning Tree (Kruskal & Prim)

Minimum Spanning Tree (MST) construction on weighted, undirected graphs
read from text files, using the CSR (Compressed Sparse Row) conversion
helper reused from Assignment 2. Both Kruskal's algorithm and Prim's
algorithm are implemented and run on the same graph inputs so their
results and execution times can be compared directly.

## Repository Layout

```
Assignment_03/
├── src/
│   ├── mst.hpp
│   ├── kruskal.cpp
│   ├── prim.cpp
│   └── driver.cpp
├── tests/
│   ├── mst_10.txt
│   ├── mst_100.txt
│   ├── mst_10000.txt
│   ├── mst_50000.txt
│   └── mst_100000.txt
├── bin/
└── README.md
```

`mst.hpp` declares `MSTEdge`/`MSTResult` and the two algorithm entry
points; it includes `graph.hpp` (Assignment 2) for the `CSRGraph` type
rather than redefining it. `kruskal.cpp` and `prim.cpp` each implement
one algorithm against a pre-built `CSRGraph`. `driver.cpp` handles
input parsing, validation, CSR conversion, and menu/CLI dispatch — none
of that work is included in the timed region.

## Build & Run

Via the shared repo-root dispatcher:

```
./common_wrapper Assignment_03
```

Directly, from the repo root (Assignment 3 reuses Assignment 2's graph
I/O and CSR conversion code, so both `src/` directories are needed):

```
g++ -std=c++17 -Wall -Wextra \
    Assignment_02/src/graph_io.cpp Assignment_02/src/graph_csr.cpp \
    Assignment_02/src/bellman_ford.cpp Assignment_02/src/floyd_warshall.cpp \
    Assignment_03/src/driver.cpp Assignment_03/src/kruskal.cpp Assignment_03/src/prim.cpp \
    -IAssignment_02/src -IAssignment_03/src \
    -o Assignment_03/bin/driver

./Assignment_03/bin/driver                 # interactive menu
./Assignment_03/bin/driver kruskal mst_10.txt
./Assignment_03/bin/driver prim    mst_10.txt
./Assignment_03/bin/driver both    mst_10.txt   # run + compare both
```

> **Pass a bare filename, not a path.** `driver.cpp` prepends `tests/`
> to whatever filename you give it, matching Assignments 1 and 2.

## Input Format

Weighted, undirected adjacency list (`mst_*.txt`):

```
V E
u0 degree neighbor1 weight1 neighbor2 weight2 ...
u1 degree neighbor1 weight1 neighbor2 weight2 ...
...
u(V-1) degree neighbor1 weight1 neighbor2 weight2 ...
```

- `V`: number of vertices. `E`: number of **undirected** edges — each
  edge is counted once in `E`, even though it appears in the adjacency
  list of both endpoints (once as `u ... v w` and once as `v ... u w`).
- The graph must be connected; `driver.cpp` validates this before
  running either algorithm (`validate_mst_graph`: checks `E > 0` for
  `V > 1`, checks that the sum of all vertex degrees equals `2E`, and
  checks that every edge appears symmetrically in both endpoints'
  lists with a matching weight).
- Edge weights may be positive, zero, or negative integers.

## Timing Methodology

`get_time_ms()` (from Assignment 2's `graph_io.cpp`) is called
immediately before and immediately after each algorithm call inside
`run_algorithm()` in `driver.cpp`. File reading, input validation, and
the adjacency-list → CSR conversion all happen **before** the timer
starts, so none of that setup work is included in the reported time.
For Kruskal specifically, the unique-edge extraction and the sort by
weight happen *inside* `kruskal_mst()`, so — per the assignment spec —
that work is correctly included in Kruskal's timed region.

## Test Files

All five required graph sizes were generated with a small script that
first builds a random spanning tree (to guarantee connectivity) and
then adds random extra edges up to the target density, so every file
is connected and sparse (`E ≈ 3V`) as required:

| File | V | E | E / V |
|---|---|---|---|
| `mst_10.txt` | 10 | 30 | 3.0 |
| `mst_100.txt` | 100 | 300 | 3.0 |
| `mst_10000.txt` | 10,000 | 30,000 | 3.0 |
| `mst_50000.txt` | 50,000 | 150,000 | 3.0 |
| `mst_100000.txt` | 100,000 | 300,000 | 3.0 |

All five ran to completion on every size — no crashes, timeouts, or
out-of-memory failures were observed at any graph size.

## Results

| File | V | E | Expected Wt. | Kruskal Wt. | Prim Wt. | Kruskal Time | Prim Time | Status |
|---|---|---|---|---|---|---|---|---|---|
| `mst_10.txt` | 10 | 30 | 9 | 9 | 9 | 0.052 ms | 0.025 ms | Pass |
| `mst_100.txt` | 100 | 300 | 99 | 99 | 99 | 0.893 ms | 0.154 ms | Pass |
| `mst_10000.txt` | 10,000 | 30,000 | 9999 | 9999 | 9999 | 7.882 ms | 53.199 ms | Pass |
| `mst_50000.txt` | 50,000 | 150,000 | 49999 | 49999 | 49999 | 43.15 ms | 274.927 ms | Pass |
| `mst_100000.txt` | 100,000 | 300,000 | 99999 | 99999 | 99999 | 71.615 ms | 555.547 ms | Pass |

"Expected Wt." is the total weight of the true MST, computed
independently and cross-checked against both implementations. Kruskal
and Prim agree on the total MST weight on every test file (the driver
also prints this comparison automatically when run with `both`); the
specific edge sets selected can differ where multiple equal-weight
MSTs exist, which is expected.

Kruskal is faster on the largest graphs, largely because Prim's
binary-heap priority queue does more `push`/`pop` work per edge
relaxation than Kruskal's one-time sort; the effect is visible from
`mst_10000.txt` onward and most pronounced at `mst_100000.txt`.

## Time Complexity

| Algorithm | Complexity | Remarks |
|---|---|---|
| Kruskal (sort + DSU) | O(E log E) | Dominated by the initial edge sort; DSU find/union with path compression + union-by-rank is ~O(α(V)) per operation |
| Prim (binary-heap PQ) | O(E log V) | One `push`/`pop` per edge relaxation via `std::priority_queue` |
| CSR conversion (Assignment 2, reused) | O(V + E) | Preprocessing only — excluded from both timed regions |


## Conclusion

Both Kruskal's and Prim's algorithms were implemented against a shared
CSR representation reused from Assignment 2, validated for
correctness against an independently computed expected MST weight,
and cross-checked against each other on all five required graph
sizes (10 to 100,000 vertices). Both algorithms agree on the total MST
weight on every test, confirming correctness; Kruskal's sort-based
approach shows a modest but consistent time advantage over Prim's
heap-based approach as the graph grows.