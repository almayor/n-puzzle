import numpy as np
import sys
import heapq

from .MinHeap import MinHeap
from .Puzzle import Puzzle


# def a_star(start):
# 	opened = MinHeap()
# 	opened.insert(start)
# 	closed = dict()
# 	niter = 0

# 	while len(opened) > 0:
# 		node = opened.pop()
# 		# print(niter)
# 		# print(f"\nConsidering: ({(node.h, node.f)})")
# 		# print(node.tiles)
# 		if node.is_final():
# 			break

# 		for child in node.get_children():
# 			if child in closed and closed[child] <= child.f:
# 				continue
# 			if child in closed:
# 				del closed[child]

# 			if child in opened:
# 				opened.update(child)
# 			else:
# 				opened.insert(child)

# 		closed[node] = node.f
# 		niter += 1

# 	path = trace_path(node)
# 	return (path, niter, opened.max_size)

def a_star(start):
	opened = [(start.f, start.h, start)]
	closed = {start: start.f}
	niter = 0

	while len(opened) > 0:
		node = heapq.heappop(opened)[2]
		# print(niter)
		if node.is_final():
			break

		for child in node.get_children():
			if child in closed and closed[child] <= child.f:
				continue
			if child in closed:
				del closed[child]

			heapq.heappush(opened, (child.f, child.h, child))
			# if child in opened:
			# 	opened.update(child)
			# else:
			# 	opened.insert(child)

			closed[child] = child.f
		niter += 1

	path = trace_path(node)
	return (path, niter, 1000)


def trace_path(node):
	path = []
	while node is not None:
		path.append(node)
		node = node.parent

	return list(reversed(path))
	
