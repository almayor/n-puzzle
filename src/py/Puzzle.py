import numpy as np


class Puzzle:

	goal = None

	def __init__(self, tiles, parent=None, h=None):
		self.tiles = tiles
		self.parent = parent
		self.g = parent.g + 1 if parent else 0
		self.h = h if h is not None else self.total_heuristic()
		self.f = self.g + self.h
		self.bytes = self.tiles.tobytes()

	def get_children(self):
		[(i, j)] = zip(*np.where(self.tiles == 0))
		n = len(self.tiles)
		children = []

		if i > 0:
			children.append(self.get_child(i, j, i - 1, j))
		if j > 0:
			children.append(self.get_child(i, j, i, j - 1))
		if i < n - 1:
			children.append(self.get_child(i, j, i + 1, j))
		if j < n - 1:
			children.append(self.get_child(i, j, i, j + 1))

		return children

	def get_child(self, i0, j0, i1, j1):
		tiles = np.copy(self.tiles)
		tiles[i1, j1], tiles[i0, j0] = tiles[i0, j0], tiles[i1, j1]
		child = Puzzle(tiles, self, self.h)
		child.h += child.heuristic(i0, j0) - self.heuristic(i1, j1)
		child.f = child.g + child.h
		return child

	def total_heuristic(self):
		h = 0
		for i, row in enumerate(self.tiles):
			for j, val in enumerate(row):
				if val == 0:
					continue
				h += self.heuristic(i, j)

		return h

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

		[(i, j)] = zip(*np.where(self.tiles == 0))
		if n % 2 == 1 and nperm % 2 == 1:
			return False
		if n % 2 == 0 and (nperm + i) % 2 != n // 2 % 2:
			return False

		return True

	def __hash__(self):
		return hash(self.tiles.tobytes())

	def __lt__(self, other):
		return self.f < other.f
	
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

	# @property
	# def f(self):
	# 	return self.g + self.h
	
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

	def manhattan_heuristic(self, i, j):
		val = self.tiles[i, j]
		dst_i, dst_j = self.goal[val]
		return abs(i - dst_i) + abs(j - dst_j)

	def euclidian_heuristic(self, i, j):
		val = self.tiles[i, j]
		dst_i, dst_j = self.goal[val]
		return np.sqrt((i - dst_i) ** 2 + (j - dst_j) ** 2)

	def hamming_heuristic(self, i, j):
		val = self.tiles[i, j]
		return 1 if (i, j) != cls.goal[val] else 0


Puzzle.heuristic = Puzzle.manhattan_heuristic

Puzzle.heuristic_names = {
	"manhattan": Puzzle.manhattan_heuristic,
	"euclidian": Puzzle.euclidian_heuristic,
	"hamming": Puzzle.hamming_heuristic
}
