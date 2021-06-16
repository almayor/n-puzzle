def cmp(n):
	return (n.f, n.h)

class MinHeap:

	def __init__(self):
		self.idx = {}
		self.heap = [None]
		self.max_size = 0

	def insert(self, node):
		i = len(self.heap)
		self.heap.append(node)
		self.idx[node] = i
		self._sift_up(i)
		self.max_size = max(self.max_size, len(self))

	def peek(self):
		return self.heap[1]

	def pop(self):
		self._swap(1, len(self.heap) - 1)
		node = self.heap.pop()
		del self.idx[node]
		self._sift_down(1)
		return node

	def update(self, node):
		i = self.idx[node]
		if cmp(node) < cmp(self.heap[i]):
			self.heap[i] = node
			self._sift_up(i)
			self._sift_down(i)

	def __len__(self):
		return len(self.heap) - 1

	def __bool__(self):
		return len(self.heap) - 1 > 0

	def __contains__(self, node):
		return node in self.idx

	def _sift_up(self, i):
		while i > 1:
			if cmp(self.heap[i]) >= cmp(self.heap[i // 2]):
				break
			self._swap(i, i // 2)
			i //= 2

	def _sift_down(self, i):
		while i * 2 < len(self):
			if (i * 2 + 1 < len(self) and
				cmp(self.heap[i * 2 + 1]) < cmp(self.heap[i * 2]) and
				cmp(self.heap[i * 2 + 1]) < cmp(self.heap[i])
			):
				self._swap(i, i * 2 + 1)
				i = i * 2 + 1

			elif cmp(self.heap[i * 2]) < cmp(self.heap[i]):
				self._swap(i, i * 2)
				i = i * 2

			else:
				break

	def _swap(self, i, j):
		self.idx[self.heap[i]] = j
		self.idx[self.heap[j]] = i
		self.heap[i], self.heap[j] = self.heap[j], self.heap[i]
