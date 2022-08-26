#pragma once
#include <algorithm>
#include <iostream>
#include <utility>
template<typename T, class Compare>
class AVLTree;

template<typename T, class Compare>
class BinSearchTree {
	Compare _compare;
	typedef BinSearchTree<T, Compare> TSelf;
	friend class AVLTree<T, Compare>;

public:
	struct tree_node_t {
		T item;
		tree_node_t *parent, *left, *right;
		bool isHeader;
		char balanceFactor;
	};


private:
	tree_node_t *_header;
	std::size_t _node_count;

	void _insertLeaf(const T &item, tree_node_t *parent, tree_node_t **child) {
		(*child) = new tree_node_t();
		(*child)->item = item;
		(*child)->item = item;
		(*child)->parent = parent;
		(*child)->left = nullptr;
		(*child)->right = nullptr;
		(*child)->isHeader = false;
		_node_count++;
	}
	void _prune(tree_node_t **node) {
		tree_node_t *nodeCopy = (*node);
		tree_node_t *newNode;
		_node_count--;

		if (((*node)->left == nullptr) && ((*node)->right == nullptr)) {
			if (*node == _header->left) { _header->left = (*node)->parent; }

			if (*node == _header->right) { _header->right = (*node)->right; }
			if ((*node)->parent->left == *node) {
				(*node)->parent->left = nullptr;
			} else if ((*node)->parent->right == *node) {
				(*node)->parent->right = nullptr;
			}
		} else if ((*node)->left == nullptr) {// node is leaf
			(*node) = (*node)->right;
			(*node)->parent = nodeCopy->parent;

			if (nodeCopy == _header->left) {
				newNode = (*node);

				while ((newNode->left) != nullptr) { newNode = newNode->left; }

				_header->left = newNode;
			}
		} else {
			(*node) = (*node)->left;
			(*node)->parent = nodeCopy->parent;

			if (nodeCopy == _header->right) {
				newNode = (*node);

				while ((newNode->right) != nullptr) { newNode = newNode->right; }

				_header->right = newNode;
			}
		}

		delete nodeCopy;
	}
	void _deleteNode(tree_node_t **node) {
		if (((*node)->left == nullptr) || ((*node)->right == nullptr)) {
			// element has no childs->simply remove
			_prune(node);
		} else if ((*node)->right->left == nullptr) {// empty left subtree of right subtree of node
			(*node)->item = std::move((*node)->right->item);
			_prune(&(*node)->right);
			// remove child node from right subtree of this node
		} else {// non-empty left subtree of right subtree of node
			tree_node_t *temp = (*node)->right->left;

			while (temp->left != nullptr) {
				temp = temp->left;
				(*node)->item = std::move(temp->item);
				_prune(&(temp->parent->left));
			}
		}
	}
	void _destroy(tree_node_t *node) {
		if (node != nullptr) {
			_destroy(node->left);
			_destroy(node->right);
			delete node;
		}
	}

	int _height(tree_node_t *node) const {
		if (node == nullptr) { return 0; }
		int left_h = _height(node->left);
		int right_h = _height(node->right);
		return 1 + std::max(left_h, right_h);
	}

public:
	class iterator {

		friend class BinSearchTree<T, Compare>;

	public:
		tree_node_t *_node;
		iterator(tree_node_t *node) : _node(node) {}

		iterator(const iterator &other) : _node(other._node) {}

	public:
		iterator &operator++() {
			if (_node->right != nullptr) {
				_node = _node->right;

				while ((_node->left) != nullptr) { _node = _node->left; }
			} else {
				tree_node_t *temp = _node->parent;

				while (_node == temp->right) {// move up and left till end
					_node = temp;
					temp = temp->parent;
				}

				if ((_node->right) != temp) { _node = temp; }
			}

			return *this;
		}
		iterator operator++(int) {
			iterator tmp(*this);
			operator++();
			return tmp;
		}
		iterator &operator--() {
			if (_node->isHeader) {
				while ((_node->right) != nullptr) { _node = _node->right; }// undo to righth until the end

				return *this;
			}

			if (_node->left != nullptr) {
				_node = _node->left;

				while ((_node->right) != nullptr) { _node = _node->right; }
			} else {
				tree_node_t *temp = _node->parent;

				while (_node == temp->left) {// move up and right till end
					_node = temp;
					temp = temp->parent;
				}

				if ((_node->left) != temp) { _node = temp; }
			}

			return *this;
		}
		iterator operator--(int) {
			iterator tmp(*this);
			operator--();
			return tmp;
		}
		T &operator*() const {
			return _node->item;
		}
		bool operator==(const iterator &other) {
			return _node == other._node;
		}
		bool operator!=(const iterator &other) {
			return !operator==(other);
		}
		iterator left() const {
			return iterator(_node->left);
		}
		iterator right() const {
			return iterator(_node->right);
		}
		bool isEmpty() const {
			return _node == nullptr;
		}
	};
	std::size_t size() const {
		return _node_count;
	}
	iterator find(const T &item) const {
		tree_node_t *parent = _header;
		tree_node_t *child = _header->parent;

		while (child != nullptr) {
			if (_compare(child->item, item)) {
				parent = child;
				child = child->right;
			} else {
				parent = child;
				child = child->left;
			}
		}

		if (parent == _header || _compare(item, parent->item)) {
			return end();
		} else {
			return iterator(parent);
		}
	}
	iterator insert(const T &item) {
		tree_node_t *newNode;

		if (_header->parent == nullptr) {// insertion of first element
			_insertLeaf(item, _header, &newNode);
			_header->parent = newNode;
			_header->left = newNode;
			_header->right = newNode;
			return iterator(newNode);
		} else {
			tree_node_t *parent = _header;
			tree_node_t *child = _header->parent;

			while (child != nullptr) {
				parent = child;

				if (_compare(item, child->item)) {
					child = child->left;
				} else {
					child = child->right;
				}
			}// while

			if (_compare(item, parent->item)) {//<
				_insertLeaf(item, parent, &parent->left);

				if (_header->left == parent) {// parent->item was MIN element
					_header->left = parent->left;
				}

				return iterator(parent->left);
			} else {
				_insertLeaf(item, parent, &parent->right);

				if (_header->right == parent) {// parent->item was MAX element
					_header->right = parent->right;
				}

				return iterator(parent->right);
			}
		}
	}

	void erase(const iterator &it) {
		if (it._node->parent->parent == it._node) {
			// it set to root node
			_deleteNode(&it._node->parent->parent);
		} else if (it._node->parent->left == it._node) {// it point to left node
			_deleteNode(&it._node->parent->left);
		} else {// it point to right node
			_deleteNode(&it._node->parent->right);
		}
	}

	iterator begin() const {
		return iterator(_header->left);
	}
	iterator rbegin() const {
		return --end();
	}
	iterator end() const {
		return iterator(_header);// next after _header->right is header
	}

	iterator rend() const {
		return --begin();// next after _header->right is header
	}


	int height() const {
		if (_node_count == 0) { return -1; }

		return -1 + _height(_header->parent);
	}

	void rotateLeft(const iterator &it) {
		tree_node_t *x = it._node;
		tree_node_t *y = x->right;
		x->right = y->left;

		if (y->left != nullptr) {
			y->left->parent = x;
			y->parent = x->parent;

			if (x == _header->parent) {// if x-is root
				_header->parent = y;
			}
		} else if (x == x->parent->left) {// if x left child element
			x->parent->left = y;
		} else {
			x->parent->right = y;
		}

		y->left = x;
		x->parent = y;
	}

	void rotateRight(const iterator &it) {
		tree_node_t *x = it._node;
		tree_node_t *y = x->left;
		x->left = y->right;

		if (y->right != nullptr) {
			y->right->parent = x;
			y->parent = x->parent;

			if (x == _header->parent) {// if x-is root
				_header->parent = y;
			}
		} else if (x == x->parent->right) {// if x right child element
			x->parent->right = y;
		} else {
			x->parent->left = y;
		}

		y->right = x;
		x->parent = y;
	}

	BinSearchTree() : _node_count(0) {
		_header = new tree_node_t();
		_header->parent = nullptr;
		_header->left = _header;// left points to min element in array;
		_header->right = _header;// right points to max element in array
		_header->isHeader = true;
	}
	~BinSearchTree() {
		_destroy(_header->parent);
	}

	void putToStream(std::ostream &s, const iterator &root, int space) const {
		static const int COUNT = 10;

		if (root.isEmpty()) { return; }

		space += COUNT;
		putToStream(s, root.right(), space);
		s << std::endl;

		for (int i = COUNT; i < space; ++i) { s << " "; }

		s << (*root) << std::endl;
		putToStream(s, root.left(), space);
	}

	template<typename T2, class Compare2>
	friend std::ostream &operator<<(std::ostream &s, const BinSearchTree<T2, Compare2> &tree);
};


template<typename T, class Compare>
std::ostream &operator<<(std::ostream &s, const BinSearchTree<T, Compare> &tree) {
	typename BinSearchTree<T, Compare>::iterator tmp(tree._header->parent);
	tree.putToStream(s, tmp, 0);
	return s;
}


template<typename T, class Compare>
class AVLTree {
	BinSearchTree<T, Compare> _bt;
	typedef typename BinSearchTree<T, Compare>::tree_node_t tree_node_t;

public:
	typedef typename BinSearchTree<T, Compare>::iterator iterator;

private:
	void _rotateLeft(tree_node_t *x) {
		tree_node_t *y = x->right;
		x->right = y->left;

		if (y->left != nullptr) { y->left->parent = x; }

		y->parent = x->parent;

		if (x == _bt._header->parent) {// if x-is root
			_bt._header->parent = y;
		} else if (x == x->parent->left) {// if x left child element
			x->parent->left = y;
		} else {
			x->parent->right = y;
		}

		y->left = x;
		x->parent = y;
	}

	void _rotateRight(tree_node_t *x) {
		tree_node_t *y = x->left;
		x->left = y->right;

		if (y->right != nullptr) { y->right->parent = x; }

		y->parent = x->parent;

		if (x == _bt._header->parent) {// if x-is root
			_bt._header->parent = y;
		} else if (x == x->parent->right) {// if x right child element
			x->parent->right = y;
		} else {
			x->parent->left = y;
		}

		y->right = x;
		x->parent = y;
	}
	void _insertLeaf(const T &item, tree_node_t *parent, tree_node_t **child) {
		_bt._insertLeaf(item, parent, child);
		(*child)->balanceFactor = '=';
	}
	// Correct  BalanceFactor for all nodes between args (args excluded)
	void _adjustPath(const tree_node_t *to, const tree_node_t *inserted) {
		const T &item = inserted->item;
		tree_node_t *temp = inserted->parent;

		while (temp != to) {
			if (_bt._compare(item, temp->item)) {
				temp->balanceFactor = 'L';
			} else {
				temp->balanceFactor = 'R';
			}

			temp = temp->parent;
		}// while
	}
	// All balance factors on the way is corrected after left and right rotate
	void _adjustLeftRight(tree_node_t *ancestor, tree_node_t *inserted) {
		// 3 variants
		const T &item = inserted->item;

		// 5a
		if (ancestor->parent == inserted) {
			ancestor->balanceFactor = '=';
		}// 5b
		else if (_bt._compare(item, ancestor->parent->item)) {
			ancestor->balanceFactor = 'R';
			_adjustPath(ancestor->parent->left, inserted);
		}// 5c
		else {
			ancestor->balanceFactor = '=';
			ancestor->parent->left->balanceFactor = 'L';
			_adjustPath(ancestor, inserted);
		}
	}
	void _adjustRightLeft(tree_node_t *ancestor, tree_node_t *inserted) {
		// 3 случая
		const T &item = inserted->item;

		// 6a
		if (ancestor->parent == inserted) {
			ancestor->balanceFactor = '=';
		}// 6b
		else if (!_bt._compare(item, ancestor->parent->item)) {
			ancestor->balanceFactor = 'L';
			_adjustPath(ancestor->parent->right, inserted);
		}// 6c
		else {
			ancestor->balanceFactor = '=';
			ancestor->parent->right->balanceFactor = 'R';
			_adjustPath(ancestor, inserted);
		}
	}
	void _fixAfterInsertion(tree_node_t *ancestor, tree_node_t *inserted) {
		tree_node_t *root = _bt._header->parent;
		const T &item = inserted->item;

		// 6 variants
		// 1: all ancestors have balance factor '='
		if (ancestor == nullptr) {
			if (_bt._compare(item, root->item)) {
				root->balanceFactor = 'L';
			} else {
				root->balanceFactor = 'R';
			}

			_adjustPath(root, inserted);
		}// 2: insertion in subtree opposite to balance factor
		else if ((ancestor->balanceFactor == 'L' && !_bt._compare(item, ancestor->item))
						 || (ancestor->balanceFactor == 'R' && _bt._compare(item, ancestor->item))) {
			ancestor->balanceFactor = '=';
			_adjustPath(ancestor, inserted);
		}// 3: insertion in right subtree of the right subtree ancestor node
		else if (ancestor->balanceFactor == 'R' && !_bt._compare(item, ancestor->right->item)) {
			ancestor->balanceFactor = '=';
			_rotateLeft(ancestor);
			_adjustPath(ancestor->parent, inserted);
		}// 4: insertion in left subtree of left subtree ancestor node
		else if (ancestor->balanceFactor == 'L' && _bt._compare(item, ancestor->left->item)) {
			ancestor->balanceFactor = '=';
			_rotateRight(ancestor);
			_adjustPath(ancestor->parent, inserted);
		}// 5: Insertion in right subtree of left subtree of ancestor node
		else if (ancestor->balanceFactor == 'L' && !_bt._compare(item, ancestor->left->item)) {
			_rotateLeft(ancestor->left);
			_rotateRight(ancestor);
			_adjustLeftRight(ancestor, inserted);
		} else {// 6: Insertion in left subtree of right subtree of ancestor node
			_rotateRight(ancestor->right);
			_rotateLeft(ancestor);
			_adjustRightLeft(ancestor, inserted);
		}
	}

	bool _checkAVLTree(tree_node_t *root) const {
		if (root == nullptr) { return true; }

		if ((abs(_bt._height(root->left) - _bt._height(root->right)) <= 1) && (_checkAVLTree(root->left))
				&& (_checkAVLTree(root->right))) {
			return true;
		}

		return false;
	}

public:
	std::size_t size() const {
		return _bt.size();
	}
	iterator find(const T &item) const {
		return _bt.find(item);
	}
	iterator insert(const T &item) {
		if (_bt._header->parent == nullptr) {//вставка первого элемента
			_insertLeaf(item, _bt._header, &_bt._header->parent);
			_bt._header->left = _bt._header->parent;
			_bt._header->right = _bt._header->parent;
			return iterator(_bt._header->parent);
		} else {//вставка в узел дерева
			tree_node_t *parent = _bt._header;
			tree_node_t *child = _bt._header->parent;
			tree_node_t *ancestor = nullptr;

			while (child != nullptr) {
				parent = child;

				if (child->balanceFactor != '=') { ancestor = child; }

				if (_bt._compare(item, child->item)) {
					child = child->left;
				} else {
					child = child->right;
				}
			}// while

			if (_bt._compare(item, parent->item)) {//<
				_insertLeaf(item, parent, &parent->left);
				_fixAfterInsertion(ancestor, parent->left);

				if (_bt._header->left == parent) {// parent->item was MIN element
					_bt._header->left = parent->left;
				}

				return iterator(parent->left);
			} else {
				_insertLeaf(item, parent, &parent->right);
				_fixAfterInsertion(ancestor, parent->right);

				if (_bt._header->right == parent) {// parent->item was MAX element
					_bt._header->right = parent->right;
				}

				return iterator(parent->right);
			}
		}
	}

	void erase(const iterator &it) {
		tree_node_t *ancestor = it._node->parent;
		bool isLeft = ancestor->left == it._node;

		if (ancestor->parent == it._node) {
			// it set to root node
			_bt._deleteNode(&ancestor->parent);
		} else if (ancestor->left == it._node) {// it point to left node
			_bt._deleteNode(&ancestor->left);
		} else {// it point to right node
			_bt._deleteNode(&ancestor->right);
		}

		while (!isAVLTree()) {
			if (isLeft) {// left node removed
				// 3 variants
				if (ancestor->balanceFactor == '=') {
					// ancestor->balanceFactor = 'R';?
					break;// end loop
				} else if (ancestor->balanceFactor == 'L') {
				} else if (ancestor->balanceFactor == 'R') {
					// 3 variants
					if (ancestor->right->balanceFactor == '=') {
						// do left rotate
						_rotateLeft(ancestor);
						// TODO correct balance
					} else if (ancestor->right->balanceFactor == 'L') {
						//Делаем левый поворот
						_rotateLeft(ancestor);
						// TODO correct balance
					} else if (ancestor->right->balanceFactor == 'R') {
						ancestor = ancestor->right;//????
						// TODO correct balance
					}
				}
			} else {// right node removed
				// 3 variants
				if (ancestor->balanceFactor == '=') {
					// ancestor->balanceFactor = 'L';?
					break;//завершаем цикл
				} else if (ancestor->balanceFactor == 'R') {
					// TODO correct ancestor
					ancestor = ancestor->parent;
				} else if (ancestor->balanceFactor == 'L') {
					// 3 variants
					if (ancestor->left->balanceFactor == '=') {
						// right rotate
						_rotateRight(ancestor);
						// TODO correct balance
					} else if (ancestor->left->balanceFactor == 'L') {
						// do right rotate
						_rotateRight(ancestor);
						// TODO correct balance
					} else if (ancestor->left->balanceFactor == 'R') {
						ancestor = ancestor->left;//????
						// TODO correct balance
					}
				}
			}// while
		}
	}

	iterator begin() const {
		return _bt.begin();
	}
	iterator rbegin() const {
		return _bt.rbegin();
	}
	iterator end() const {
		return _bt.end();
	}
	int height() const {
		return _bt.height();
	}
	iterator rend() const {
		return _bt.rend();
	}
	AVLTree() {}
	~AVLTree() {}
	bool isAVLTree() const {
		return _checkAVLTree(_bt._header->parent);
	}
	template<typename T2, class Compare2>
	friend std::ostream &operator<<(std::ostream &s, const AVLTree<T2, Compare2> &tree);
};

template<typename T, class Compare>
std::ostream &operator<<(std::ostream &s, const AVLTree<T, Compare> &tree) {
	s << tree._bt;
	return s;
}
