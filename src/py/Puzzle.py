import numpy as np


class Puzzle:

	goal = None

	def __init__(self, tiles, parent=None):
		self.tiles = tiles
		self.parent = parent
		self.g = parent.g + 1 if parent else 0
		self.h = self.heuristic()

	def get_children(self):
		[(i, j)] = zip(*np.where(self.tiles == 0))
		n = len(self.tiles)
		children = []

		if i > 0:
			tiles = np.copy(self.tiles)
			tiles[i - 1, j], tiles[i, j] = tiles[i, j], tiles[i - 1, j]
			children.append(Puzzle(tiles, self))
		if j > 0:
			tiles = np.copy(self.tiles)
			tiles[i, j - 1], tiles[i, j] = tiles[i, j], tiles[i, j - 1]
			children.append(Puzzle(tiles, self))
		if i < n - 1:
			tiles = np.copy(self.tiles)
			tiles[i + 1, j], tiles[i, j] = tiles[i, j], tiles[i + 1, j]
			children.append(Puzzle(tiles, self))
		if j < n - 1:
			tiles = np.copy(self.tiles)
			tiles[i, j + 1], tiles[i, j] = tiles[i, j], tiles[i, j + 1]
			children.append(Puzzle(tiles, self))

		return children

	def is_final(self):
		return self.heuristic() == 0

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
		if n % 2 == 0 and (nperm % 2 + i % 2) != n // 2 % 2:
			return False

		return True

	def __hash__(self):
		return hash(self.tiles.tobytes())

	def __eq__(self, other):
		return np.all(self.tiles == other.tiles)

	def __repr__(self):
		rows = []
		for row in self.tiles:
			rows.append(' '.join(f"{val: 2d}" for val in row))
		return '\n'.join(rows)

	@property
	def f(self):
		return self.g + self.h
	
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

	def manhatten_heuristic(self):
		h = 0
		n = len(self.tiles)
		for i, row in enumerate(self.tiles):
			for j, val in enumerate(row):
				if val == 0:
					continue
				dst_i, dst_j = self.goal[val]
				h += abs(i - dst_i) + abs(j - dst_j)

		return h


	def euclidian_heuristic(self):
		h = 0
		n = len(self.tiles)

		for i, row in enumerate(self.tiles):
			for j, val in enumerate(row):
				if val == 0:
					continue
				dst_i, dst_j = self.goal[val]
				h += np.sqrt((i - dst_i) ** 2 + (j - dst_j) ** 2)

		return h


	def hamming_heuristic(self):
		h = 0
		n = len(self.tiles)

		for i, row in enumerate(self.tiles):
			for j, val in enumerate(row):
				if val == 0:
					continue
				if (i, j) != self.goal[val]:
					h += 1

		return h


Puzzle.heuristic = Puzzle.manhatten_heuristic

Puzzle.heuristic_names = {
	"manhattan": Puzzle.manhatten_heuristic,
	"euclidian": Puzzle.euclidian_heuristic,
	"hamming": Puzzle.hamming_heuristic
}
