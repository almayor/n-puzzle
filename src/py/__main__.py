import numpy as np
import sys

from .Puzzle import Puzzle
from .MinHeap import MinHeap
from .a_star import a_star


def parse_input():
	line = sys.stdin.readline()
	while line.startswith("#"): #skip comments
		line = sys.stdin.readline()

	rows = list()
	n = int(line)
	for _ in range(n):
		row = map(int, sys.stdin.readline().strip().split())
		rows.append(list(row))

	return np.array(rows)


def fill_bottom_left(arr, vals, istart, jstart, iend, jend):
	i = iend
	for j in range(jend, jstart, -1):
		arr[i, j] = next(vals)
	j = jstart
	for i in range(iend, istart, -1):
		arr[i, j] = next(vals)

	arr[istart, jstart] = next(vals)
	if jend > jstart:
		fill_top_right(arr, vals, istart, jstart + 1, iend - 1, jend)


def fill_top_right(arr, vals, istart, jstart, iend, jend):
	i = istart
	for j in range(jstart, jend):
		arr[i, j] = next(vals)
	j = jend
	for i in range(istart, iend):
		arr[i, j] = next(vals)

	arr[iend, jend] = next(vals)
	if iend > istart:
		fill_bottom_left(arr, vals, istart + 1, jstart, iend, jend - 1)


def make_goal(n):
	goal = np.zeros((n, n))
	vals = list(range(1, n * n)) + [0]
	fill_top_right(goal, iter(vals), 0, 0, n - 1, n - 1)
	return goal


def main():
	tiles = parse_input()
	n = len(tiles)
	end = make_goal(n)
	
	Puzzle.set_goal(end)
	Puzzle.set_heuristic("manhattan")
	start = Puzzle(tiles)

	if not start.is_solvable():
		print("Puzzle is insoluble")
		sys.exit(1)
	else:
		path, niter, mem_size = a_star(start)
		print(f"NUM STEPS  = {len(path)}")
		print(f"NUM ITERS  = {niter}")
		print(f"MAX OPENED = {mem_size}")
		print("\n============ SOLUTION ============\n")
		print('\n\n'.join(repr(node) for node in path))


if __name__ == "__main__":
	main()

