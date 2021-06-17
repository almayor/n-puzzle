import sys
import numpy as np

from .MinHeap import MinHeap
from .Puzzle import Puzzle
from .a_star import a_star


def parse_input(f):
	line = f.readline()
	while line.startswith("#"): #skip comments
		line = f.readline()

	rows = list()
	n = int(line)
	for _ in range(n):
		line = f.readline().strip()
		row = map(int, line.split())
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
	tiles = parse_input(sys.stdin)
	n = len(tiles)
	end = make_goal(n)
	Puzzle.set_goal(end)
	Puzzle.set_heuristic("manhattan_linconf")
	start = Puzzle(tiles)

	if not start.is_solvable():
		print("Puzzle is insoluble")
		sys.exit(1)

	path, niter, mem_size = a_star(start)
	for node in path:
		print(node, end='\n\n')
	print(f"total moves:\t\t\t{len(path) - 1}")
	print(f"time complexity:\t\t{niter}")
	print(f"space complexity:\t\t{mem_size}")


if __name__ == "__main__":
	main()

