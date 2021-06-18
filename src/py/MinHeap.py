from .Puzzle import Puzzle
import heapq


class MinHeap:

	def __init__(self, mode='a_star'):
		self.heap = []
		self.tiebreaker = 0
		self.max_size = 0
		
		if mode == 'a_star':
			self.itemize = lambda node: (node.f, -node.g, self.tiebreaker, node)
		elif mode == 'greedy':
			self.itemize = lambda node: (node.h, self.tiebreaker, node)
		elif mode == 'uniform':
			self.itemize = lambda node: (-node.g, self.tiebreaker, node)
		else:
			raise IllegalArgumentException("unknown mode")

	def push(self, node: Puzzle):
		item = self.itemize(node)
		heapq.heappush(self.heap, item)
		self.tiebreaker += 1
		self.max_size = max(self.max_size, len(self.heap))

	def peek(self) -> Puzzle:
		return self.heap[0][-1]

	def pop(self) -> Puzzle:
		return heapq.heappop(self.heap)[-1]

	def __len__(self):
		return len(self.heap)

	def __bool__(self):
		return len(self.heap) > 0

