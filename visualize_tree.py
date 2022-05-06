import matplotlib.pyplot as plt
import seaborn as sns

# https://zephyrus1111.tistory.com/132 코드 수정하여 사용.

class Node:
	def __init__(self):
		self.x = None
		self.y = None
		self.tree_type = ''
		self.text = ''
		self.token = ''
		self.type = ''
		self.is_root = False
		self.parent = None
		self.left = None
		self.right = None
		self.is_leaf = False

	def get_level(self, cnt = 1):
		if self.is_root:
			return cnt
		else:
			cnt += 1
			cnt = self.parent.get_level(cnt)
			return cnt

	def set_level(self, node):
		self.left = node
		node.parent = self

	def set_right(self, node):
		self.right = node
		node.parent = self

class Tree:
	def __init__(self, root):
		assert root.is_root, 'node should be specified as root'
		self.__root = root

	def get_root(self):
		return self.__root

	def get_branch_len(self, node, cnt = 1):
		if node.parent is None:
			return cnt
		else:
			cnt += 1
			return self.get_branch_len(node.parent, cnt)

	def get_depth(self, remove_Leaf=False):
		all_nodes = self.inorder()
		if remove_Leaf:
			depth = max([self.get_branch_len(node) for node in all_nodes if not node.is_leaf])
		else:
			depth = max([self.get_branch_len(node) for node in all_nodes])
		return depth

	def inorder(self, node=None):
		if node is None:
			node = self.__root
		res = []
		if node.left != None:
			res = res + self.inorder(node.left)
		res.append(node)
		if node.right != None:
			res = res + self.inorder(node.right)
		return res

	def get_right_most_node(self, node = None, level = None):
		if node is None:
			node = self.__root
		if level is None:
			return [nd for nd in self.inorder(node)][-1]
		else:
			return [nd for nd in self.inorder(node) if nd.get_level()==level][-1]

	def get_left_most_node(self, node = None, level = None):
		if node is None:
			node = self.__root
		if level is None:
			return [nd for nd in self.inorder(node)][0]
		else:
			return [nd for nd in self.inorder(node) if nd.get_level()==level][0]

	def get_dist_between_subtrees(self):
		root.left.parent = None
		root.right.parent = None
		root.left.is_root = True
		root.right.is_root = True
		left_subtree = Tree(root.left)
		right_subtree = Tree(root.right)
		if left_subtree.get_depth() == right_subtree.get_depth():
			level = right_subtree.get_depth()
		elif left_subtree.get_depth() > right_subtree.get_depth():
			level = right_subtree.get_depth()
		else:
			level = left_subtree.get_depth()

		lrmn = left_subtree.get_right_most_node(level=level)
		rlmn = right_subtree.get_left_most_node(level=level)
		x_diff = rlmn.x - lrmn.x

		root.left.parent = root
		root.right.parent = root
		root.left.is_root = False
		root.right.is_root = False
		return x_diff

	def move_tree(self, shift=1):
		for nd in self.inorder():
			nd.x += shift
		return

root = Node()
root.is_root = True
root.text = 'Root'
left_child = Node()
left_child.text = 'Left Child'
root.set_level(left_child)
right_child = Node()
right_child.text = 'Right Child'
root.set_right(right_child)
l_child = Node()
l_child.text = 'Left Child1'
l_child.is_leaf = True
right_child.set_level(l_child)
r_child = Node()
r_child.text = 'Right Child1'
r_child.is_leaf = True
right_child.set_right(r_child)

ll_child = Node()
ll_child.text = 'Left Child2'
left_child.set_level(ll_child)

rr_child = Node()
rr_child.text = 'Right Child2'
left_child.set_right(rr_child)

lll_child = Node()
lll_child.text = 'Left Child3'
lll_child.is_leaf = True
rr_child.set_level(lll_child)

rrr_child = Node()
rrr_child.text = 'Right Child3'
rrr_child.is_leaf = True
rr_child.set_right(rrr_child)

llll_child = Node()
llll_child.text = 'LC4'
llll_child.is_leaf = True
ll_child.set_level(llll_child)

rrrr_child = Node()
rrrr_child.text = 'RC4'
rrrr_child.is_leaf = True
ll_child.set_right(rrrr_child)

tree = Tree(root)

# def draw_connect_line(node):
# 	if node is not None:
# 		if node.parent is not None:
# 			plt.plot((node.parent.x, node.x), (node.parent.y, node.y),color='k')
# 		draw_connect_line(node.left)
# 		draw_connect_line(node.right)

def drawing_binary_tree(tree, offset):
	for i, nd in enumerate(tree.inorder()):
		nd.x = i
		nd.y = -(nd.get_level() - 1)

	def tidy_drawing_tree(tree):
		root = tree.get_root()
		if root.left is None and root.right is None:
			return
		else:
			root.left.parent = None
			root.right.parent = None
			root.left.is_root = True
			root.right.is_root = True
			left_subtree = Tree(root.left)
			right_subtree = Tree(root.right)

			if left_subtree.get_depth() == right_subtree.get_depth():
				level = right_subtree.get_depth()

			elif left_subtree.get_depth() > right_subtree.get_depth():
				level = right_subtree.get_depth()
			else:
				level = left_subtree.get_depth()

			lrmn = left_subtree.get_right_most_node(level=level)
			rlmn = right_subtree.get_left_most_node(level=level)
			x_diff = rlmn.x - lrmn.x
			shift = offset - x_diff
			right_subtree.move_tree(shift=shift / 2)
			left_subtree.move_tree(shift=-shift / 2)

			tidy_drawing_tree(left_subtree)
			tidy_drawing_tree(right_subtree)

			root.x = (root.left.x + root.right.x) / 2

			root.left.parent = root
			root.right.parent = root
			root.left.is_root = False
			root.right.is_root = False

	max_cnt = tree.get_depth()
	cnt = 1
	while cnt <= max_cnt:
		tidy_drawing_tree(tree)
		if tree.get_dist_between_subtrees() == offset:
			break
		else:
			cnt += 1

drawing_binary_tree(tree, offset=2)

colors = sns.color_palette('hls', tree.get_depth() - 1)

def draw_node(node, ax):
	if node is not None:
		if node.is_leaf:
			bbox=dict(boxstyle='round',fc='white')
		else:
			bbox=dict(boxstyle='square',fc=colors[node.get_level()-1], pad=1)
		## 텍스트 표시
		ax.text(node.x,node.y,node.text,bbox=bbox,fontsize=20,ha='center',va='center')
		if node.parent is not None: ## 부모 노드와 자식 노드 연결
			ax.plot((node.parent.x, node.x), (node.parent.y, node.y),color='k')
		draw_node(node.left, ax)
		draw_node(node.right, ax)

fig = plt.figure(figsize=(20,10))
renderer = fig.canvas.get_renderer()
ax = fig.add_subplot()

draw_node(root, ax)

x_coords = []
y_coords = []

for nd in tree.inorder():
	x_coords.append(nd.x)
	y_coords.append(nd.y)

min_x, max_x = min(x_coords), max(x_coords)
min_y, max_y = min(y_coords), max(y_coords)

## 캔버스 안에 나무가 표시되도록  x,y 축 상한 하한 설정
ax.set_xlim(min_x - 1,max_x + 1)
ax.set_ylim(min_y - 0.5,max_y + 0.5)

## 축은 안보이게 설정
ax.axes.xaxis.set_visible(False)
ax.axes.yaxis.set_visible(False)

plt.scatter(x_coords, y_coords, zorder=10)
plt.show()
