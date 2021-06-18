import numpy as np
import sys
from collections import defaultdict


class Puzzle:

	goal = None

	def __init__(self, tiles, parent=None, zero_loc=None):
		self.tiles = tiles
		self.bytes = self.tiles.tobytes()
		self.parent = parent

		if zero_loc:
			self.zero_loc = zero_loc
		else:
			[(x, y)] = zip(*np.where(tiles == 0))
			self.zero_loc = (x, y)

		self.g = parent.g + 1 if parent else 0
		self.h = self.heuristic(False)
		self.f = self.g + self.h
		
	def get_children(self):
		(i, j) = self.zero_loc
		(k, l) = self.parent.zero_loc if self.parent else (None, None)
		n = len(self.tiles)
		children = []

		if i > 0 and i - 1 != k:
			children.append(self.get_child(i - 1, j))
		if j > 0 and j - 1 != l:
			children.append(self.get_child(i, j - 1))
		if i < n - 1 and i + 1 != k:
			children.append(self.get_child(i + 1, j))
		if j < n - 1 and j + 1 != l:
			children.append(self.get_child(i, j + 1))

		return children

	def get_child(self, i1, j1):
		(i0, j0) = self.zero_loc
		tiles = np.copy(self.tiles)
		tiles[i1, j1], tiles[i0, j0] = tiles[i0, j0], tiles[i1, j1]
		return Puzzle(tiles, self, (i1, j1))

	def is_final(self):
		return self.h == 0

	def is_solvable(self):
		n = len(self.tiles)
		nperm = 0

		for i in range(n * n):
			val1 = self.tiles[i // n, i % n]
			for j in range(i + 1, n * n):
				val2 = self.tiles[j // n, j % n]
				if val1 != 0 and val2 != 0 and self.goal[val1] > self.goal[val2]:
					nperm += 1

		if n % 2 == 1 and nperm % 2 == 1:
			return False
		if n % 2 == 0 and (nperm + self.zero_loc[0]) % 2 != n // 2 % 2:
			return False

		return True

	def __hash__(self):
		return hash(self.bytes)
	
	def __eq__(self, other):
		return self.bytes == other.bytes

	def __repr__(self):
		n = len(self.tiles)
		w = len(str(n * n - 1))
		fmt = f"{{:{w}}}"

		rows = []
		for row in self.tiles:
			rows.append(' '.join(fmt.format(val) for val in row))
		return '\n'.join(rows)

	def manhattan_linconf_heuristic(self, relative=False):
		if self.parent is None:
			return self.manhattan_heuristic() + self.linconf_heuristic()

		rel_h = (
			self.manhattan_heuristic(relative=True) +
			self.linconf_heuristic(relative=True)
		)
		return rel_h if relative else self.parent.h + rel_h

	def manhattan_heuristic(self, relative=False):
		
		def single_heuristic(tiles, i, j):
			val = tiles[i, j]
			if val == 0: return 0
			dst_i, dst_j = self.goal[val]
			return abs(i - dst_i) + abs(j - dst_j)

		if self.parent is None:
			n = len(self.tiles)
			return sum(single_heuristic(self.tiles, i, j) for i in range(n) for j in range(n))
		
		rel_h = (
			single_heuristic(self.tiles, *self.parent.zero_loc) -
			single_heuristic(self.parent.tiles, *self.zero_loc))
		return rel_h if relative else self.parent.h + rel_h

	def euclidian_heuristic(self, relative=False):
		
		def single_heuristic(tiles, i, j):
			val = tiles[i, j]
			if val == 0: return 0
			dst_i, dst_j = self.goal[val]
			return np.sqrt((i - dst_i) ** 2 + (j - dst_j) ** 2)

		if self.parent is None:
			n = len(self.tiles)
			return sum(single_heuristic(self.tiles, i, j) for i in range(n) for j in range(n))

		rel_h = (
			single_heuristic(self.tiles, *self.parent.zero_loc) -
			single_heuristic(self.parent.tiles, *self.zero_loc)
		)
		return rel_h if relative else self.parent_h + rel_h

	def hamming_heuristic(self, relative=False):
		
		def single_heuristic(tiles, i, j):
			val = tiles[i, j]
			if val == 0: return 0
			return 1 if (i, j) != self.goal[val] else 0

		if self.parent is None:
			n = len(self.tiles)
			return sum(single_heuristic(self.tiles, i, j) for i in range(n) for j in range(n))

		rel_h = (
			single_heuristic(self.tiles, *self.parent.zero_loc) -
			single_heuristic(self.parent.tiles, *self.zero_loc)
		)
		return rel_h if relative else self.parent.h + rel_h

	def linconf_heuristic(self, relative=False):
		if self.parent is None:
			n = len(self.tiles)
			h = 0
			for i in range(n):
				h += 2 * self.get_linear_conflicts(i, column=False)
			for j in range(n):
				h += 2 * self.get_linear_conflicts(j, column=True)
			return h

		(i, j) = self.zero_loc
		(k, l) = self.parent.zero_loc
		rel_h = 0

		if i == k:
			rel_h += 2 * (
				self.get_linear_conflicts(j, column=True)
				+ self.get_linear_conflicts(l, column=True)
				- self.parent.get_linear_conflicts(j, column=True)
				- self.parent.get_linear_conflicts(l, column=True)
			)
		else:
			rel_h += 2 * (
				self.get_linear_conflicts(i, column=False)
				+ self.get_linear_conflicts(k, column=False)
				- self.parent.get_linear_conflicts(i, column=False)
				- self.parent.get_linear_conflicts(k, column=False)
			)
		return rel_h if relative else self.parent.h + rel_h

	def get_linear_conflicts(self, i, column):
		vals = self.tiles[:, i] if column else self.tiles[i]
		h = 0
		graph = defaultdict(set)
		pairs = 0

		for j, v1 in enumerate(vals):
			t1 = self.goal[v1]
			if v1 == 0 or t1[1 if column else 0] != i:
				continue
			for v2 in vals[j + 1:]:
				t2 = self.goal[v2]
				if v2 == 0 or t2[1 if column else 0] != i:
					continue
				if t1[0 if column else 1] > t2[0 if column else 1]:
					graph[v1].add(v2)
					graph[v2].add(v1)
					pairs += 1

		while pairs > 0:
			v1, neighbors = max(graph.items(), key=lambda it: len(it[1]))
			del graph[v1]
			pairs -= len(neighbors)
			for v2 in neighbors:
				h += 1
				graph[v2].remove(v1)

		return h

	@classmethod
	def set_goal(cls, tiles):
		cls.goal = dict()
		for i, row in enumerate(tiles):
			for j, val in enumerate(row):
				cls.goal[val] = (i, j)

	@classmethod
	def set_heuristic(cls, name: str):
		if name.lower() not in cls.heuristic_names:
			raise IllegalArgumentException(
				f"Heuristic name must be one of {cls.heuristic_names.keys()}"
			)

		cls.heuristic = cls.heuristic_names[name.lower()]


Puzzle.heuristic = Puzzle.manhattan_linconf_heuristic

Puzzle.heuristic_names = {
	"manhattan": Puzzle.manhattan_heuristic,
	"manhattan_linconf": Puzzle.manhattan_linconf_heuristic,
	"euclidian": Puzzle.euclidian_heuristic,
	"hamming": Puzzle.hamming_heuristic,
}
