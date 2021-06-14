import numpy as np

from .MinHeap import MinHeap
from .Puzzle import Puzzle


def a_star(start):
	opened = MinHeap()
	opened.insert(start)
	closed = dict()
	niter = 0

	while len(opened) > 0:
		node = opened.pop()
		
		if node.is_final():
			break

		for child in node.get_children():
			if child in closed and closed[child] <= child.f:
				continue
			if child in closed:
				del closed[child]

			if child in opened:
				opened.update(child)
			else:
				opened.insert(child)

		closed[node] = node.f
		niter += 1

	path = trace_path(node)
	return (path, niter, opened.max_size)


def trace_path(node):
	path = []
	while node is not None:
		path.append(node)
		node = node.parent

	return list(reversed(path))
	
