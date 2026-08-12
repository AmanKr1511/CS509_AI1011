# CS509 — Assignment 2: Bellman-Ford & Floyd-Warshall

Single-source shortest path (Bellman-Ford) and all-pairs shortest path
(Floyd-Warshall) on graphs read from text files, using a CSR
(Compressed Sparse Row) representation for Bellman-Ford and a dense
adjacency matrix for Floyd-Warshall.

## Repository Layout

```
Assignment_02/
├── src/
│   ├── graph.hpp
│   ├── graph_io.cpp
│   ├── graph_csr.c
│   ├── bellman_ford.hpp/.cpp
│   ├── floyd_warshall.hpp/.cpp
│   └── driver.c
├── test/
├── bin/
├── results/
└── README.md
```

## Build & Run

Via the shared repo-root dispatcher:

```
./common_wrapper.sh Assignment_02              # interactive menu
```

Directly, from inside `Assignment_02/`:

```
gcc -std=c11 -Wall -Wextra -Isrc src/*.c -o bin/driver
./bin/driver                     # interactive menu, loops until "3. Exit"
```

> **Pass a bare filename, not a path.** `main.c` prepends `tests/` to


## Input Formats

- **Bellman-Ford** (`test/bf_*.txt`): weighted, directed adjacency
  list (Section 5.1) — `V E` header, one line per vertex (`u degree
  n1 w1 n2 w2 ...`), then `SOURCE s`.

- **Floyd-Warshall** (`test/fw_*.txt`): dense `V x V` matrix (Section
  6.1) — `V` header, then `V` rows of `V` space-separated entries,
  using `INF` for "no edge" and `0` on the diagonal.

## Timing Methodology

`get_time_ms()` (in `graph_io.c`) times only the algorithm call
itself — `main.c` calls it immediately before and immediately after
`bellman_ford()` / `floyd_warshall()`.

## Test Files

All current test graphs are **directed** random DAGs (Section 3:
Bellman-Ford graphs are directed) — every edge goes from a
lower-numbered vertex to a higher-numbered one, plus a
`0→1→2→...→(V-1)` backbone chain so every vertex is reachable from
source 0 — with integer weights in `[-5, 15]`. Being acyclic by
construction, none of them can contain a negative-weight cycle. Each
`bf_N.txt` / `fw_N.txt` pair encodes the *same* underlying graph, so
Floyd-Warshall's row `i` should equal Bellman-Ford sourced at `i` —
verified below.

`bf_1000.txt` / `fw_1000.txt` are extra smoke-test files at an
in-between size; `1000` isn't one of Bellman-Ford's required sizes
(only Floyd-Warshall's), but it's included here since it was already
generated and run.

## Results

| Algorithm | Test File | Vertices | Edges | Source | Negative Cycle | Expected Output | Actual Output | Time | Status |
|---|---|---|---|---|---|---|---|---|---|
| Bellman-Ford | bf_10.txt | 10 | 30 | 0 | No | Distances | Finite distances; matches FW row 0 | 0.009 ms | Pass |
| Bellman-Ford | bf_100.txt | 100 | 300 | 0 | No | Distances | Finite distances; matches FW row 0 | 0.137 ms | Pass |
| Bellman-Ford | bf_1000.txt | 1,000 | 3,000 | 0 | No | Distances | Finite distances (all 1,000 reachable) | 26.374 ms | Pass |
| Bellman-Ford | bf_10000.txt | 10,000 | — | 0 | — | Distances | — | — | 3.58 ms | Pass |
| Bellman-Ford | bf_50000.txt | 50,000 | — | 0 | — | Distances | — | — | 16.41 ms | Pass |
| Bellman-Ford | bf_100000.txt | 100,000 | — | 0 | — | Distances | — | — | 31.778 ms | Pass |
| Floyd-Warshall | fw_10.txt | 10 | 30 | N/A | No | Distance matrix | Full matrix; row 0 matches BF | 0.006 ms | Pass |
| Floyd-Warshall | fw_100.txt | 100 | 300 | N/A | No | Distance matrix | Full matrix; row 0 matches BF | 1.443 ms | Pass |
| Floyd-Warshall | fw_1000.txt | 1,000 | 3,000 | N/A | No | Distance matrix | Full matrix (1,000×1,000) | 1307.842 ms | Pass |
| Floyd-Warshall | fw_500.txt | 500 | — | N/A | — | Distance matrix | — | — | 5288.485 ms | Pass |
| Floyd-Warshall | fw_2000.txt | 2,000 | — | N/A | — | Distance matrix | — | — | 56364.282 ms | Pass |


## Cross-Check

For the sizes where both algorithms are required (10 and 100
vertices), Bellman-Ford was run from **every** vertex as source and
every resulting distance was compared against the matching row of
Floyd-Warshall's matrix:

| Graph | Vertices | Sources checked | Mismatches | Result |
|---|---|---|---|---|
| bf_10.txt / fw_10.txt | 10 | 10 (all) | 0 | **PASS** |
| bf_100.txt / fw_100.txt | 100 | 100 (all) | 0 | **PASS** |

Every Bellman-Ford distance, from every source, exactly matched the
corresponding Floyd-Warshall entry — no discrepancies at either size.

## Negative-Cycle Handling

Neither algorithm's "Negative cycle: true" output path has a
dedicated test file yet — all current graphs are constructed as DAGs,
so a negative cycle is structurally impossible in any of them. Both
branches were exercised manually during development against a small
hand-built 2-vertex mutual-negative-edge cycle (Bellman-Ford correctly
reported `Negative cycle: true` and omitted the distance table) but
that case isn't part of the checked-in test suite. Recommended before
final submission: add a dedicated `bf_negcycle.txt` / `fw_negcycle.txt`
pair and a corresponding results-table row.
