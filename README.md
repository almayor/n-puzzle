# N Puzzle

An implementation of A* algorithm to solve the N Puzzle problem. N Puzzle is a generalization of a famous [15 Puzzle problem](https://en.wikipedia.org/wiki/15_puzzle). Both Python and C++ implementations are provided.  

Please note that the puzzle is solved when a "snail" state is achieved, which is different from the classical goal state. Here are examples ![](./docs/snail_states.png)

You can choose one of the following admissible heuristic functions (in order of increase efficiency)

1. Hamming 
2. Euclidian
3. Manhattan
4. Manhattan with linear conflicts

---

To generate puzzles run

```sh
python3 npuzzle_gen.py --help
```

To run Python version,

```sh
python3 -m pip install -r requirements.txt
python3 solver.py --help
python3 solver.py < <your-puzzle-file>
```

To run C++ version,

```sh
make
./solver --help
./solver < <your-puzzle-file>
```