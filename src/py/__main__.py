import argparse
import sys
import numpy as np

from .MinHeap import MinHeap
from .Puzzle import Puzzle
from .search import search


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


def parse_args():
	parser = argparse.ArgumentParser()
	parser.add_argument(
		'-r', '--heuristic',
		choices=['manhattan', 'hamming', 'euclidian', 'manhattan_linconf'],
		default='manhattan_linconf',
		help="type of heuristic (manhattan_linconf by default)")
	parser.add_argument(
		'-m', '--mode',
		choices=['a_star', 'greedy', 'uniform'],
		default='a_star',
		help="search mode (a_star by default)")
	parser.add_argument(
		'-f', '--file',
		type=str,
		help="file with puzzle (stdin by default)")
	args = parser.parse_args()
	return args


def main():
	args = parse_args()
	file = open(args.file, 'r') if args.file else sys.stdin
	tiles = parse_input(file)
	file.close()
	n = len(tiles)
	end = make_goal(n)
	Puzzle.set_goal(end)
	Puzzle.set_heuristic(args.heuristic)
	start = Puzzle(tiles)
	
	if not start.is_solvable():
		sys.stderr.write("Puzzle is insoluble")
		sys.exit(1)

	path, niter, mem_size = search(start, mode=args.mode)
	for node in path:
		print(node, end='\n\n')
	print(f"total moves:\t\t\t{len(path) - 1}")
	print(f"time complexity:\t\t{niter}")
	print(f"space complexity:\t\t{mem_size}")


if __name__ == "__main__":
	main()

