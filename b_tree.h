#pragma once

#include "sequence.h"
#include <utility>

template <class T> struct BTreeNode;

// if left_child or right_child are nullptr
// => this is leaf-node
template <class T>
struct Key
{
	BTreeNode<T>* left_child = nullptr;
	BTreeNode<T>* right_child = nullptr;
	T data = T{};

	Key() = default;
	Key(const T& new_data, BTreeNode<T>* left = nullptr,
		BTreeNode<T>* right = nullptr);
};

template <class T>
struct BTreeNode
{
	BTreeNode<T>* parent;
	Sequence<Key<T>>* keys;

	// Constructors
	BTreeNode();
	BTreeNode(BTreeNode<T>& other, BTreeNode<T>* current_parent = nullptr);
	// recursive
	BTreeNode(const T& element, BTreeNode<T>* element_parent = nullptr);
	BTreeNode(Sequence<Key<T>>* elements, BTreeNode<T>* element_parent = nullptr);
};

// m_ - minimal degree
// every non-leaf and non-root_ node has x children
// [m_/2] <= x < m_
// root_ has at least 2 children (if it's not a leaf node)
// non-leaf node with k children contains k - 1 keys
// all leafs must be on the same height
// left subtree: less or equal, right subtree: bigger

template <class T>
class BTree
{
	size_t m_;
	BTreeNode<T>* root_;
public:
	// Constructors
	BTree(size_t max);
	BTree(BTreeNode<T>& root, size_t max);
	BTree(T element, size_t max);
	BTree(BTree<T>& tree);
	
	// Decomposition
	size_t GetM();
	BTreeNode<T>* GetRootPointer();
	void Add(T& element);

	// Search
	// Search an element or a place to insertion
	std::pair<BTreeNode<T>*, size_t> Search
		(T& element, BTreeNode<T>* current_node);
	std::pair<BTreeNode<T>*, size_t> Search(T& element);
};

template <class T>
Key<T>::Key(const T& new_data, BTreeNode<T>* left, BTreeNode<T>* right)
{
	left_child = left;
	right_child = right;
	data = new_data;
}

// BTreeNode

// Constructors

template <class T>
BTreeNode<T>::BTreeNode()
{
	parent = nullptr;
	keys = new ArraySequence<Key<T>>;
}

template <class T>
BTreeNode<T>::BTreeNode(BTreeNode<T>& other, BTreeNode<T>* current_parent)
{
	parent = current_parent;
	keys = new ArraySequence<Key<T>>;
	for (size_t i = 0; i < other.keys->GetCount(); ++i)
	{
		BTreeNode<T>* left_p = nullptr;
		if(other.keys->Get(i).left_child != nullptr)
		{
			left_p = new BTreeNode(*other.keys->Get(i).left_child, this);
		}
		BTreeNode<T>* right_p = nullptr;
		if (other.keys->Get(i).left_child != nullptr)
		{
			right_p = new BTreeNode(*other.keys->Get(i).right_child, this);
		}
		keys->Append(Key<T>(other.keys->Get(i).data, left_p, right_p));
	}
}

template <class T>
BTreeNode<T>::BTreeNode(const T& element, BTreeNode<T>* element_parent)
{
	parent = element_parent;
	keys = new ArraySequence<Key<T>>;
	keys->Append(Key<T>(element));
}

template <class T>
BTreeNode<T>::BTreeNode(Sequence<Key<T>>* elements, BTreeNode<T>* element_parent)
{
	parent = element_parent;
	keys = new ArraySequence<Key<T>>(elements);
}

// BTree

// Constructors

template <class T>
BTree<T>::BTree(size_t max)
{
	m_ = max;
	root_ = new BTreeNode<T>;
}

template <class T>
BTree<T>::BTree(BTreeNode<T>& root, size_t max)
{
	m_ = max;
	root_ = new BTreeNode<T>(root);
}

template <class T>
BTree<T>::BTree(BTree<T>& tree)
{
	m_ = tree.m_;
	root_ = new BTreeNode<T>(tree.root_->keys);
}

template <class T>
size_t BTree<T>::GetM()
{
	return m_;
}

template <class T>
BTreeNode<T>* BTree<T>::GetRootPointer()
{
	return root_;
}

template <class T>
BTree<T>::BTree(T element, size_t max)
{
	m_ = max;
	root_ = new BTreeNode<T>(element);
}

// Search

template<class T>
int key_is_bigger(const Key<T>& a, const Key<T>& b)
{
	return a.data - b.data;
}

template <class T>
std::pair<BTreeNode<T>*, size_t> BTree<T>::Search
(T& element, BTreeNode<T>* current_node)
{
	// empty sequence case
	if (current_node->keys->GetCount() == 0) return std::make_pair(current_node, 0);
	// search in sequence
	size_t first_equal_or_bigger = binary_search<Key<T>>(
		*current_node->keys, element, 0, current_node->keys->GetCount() - 1, key_is_bigger);
	// found element
	if (current_node->keys->Get(first_equal_or_bigger).data == element)
		return std::make_pair(current_node, first_equal_or_bigger);
	// this is leaf node
	if (current_node->keys->Get(first_equal_or_bigger).left_child == nullptr &&
		current_node->keys->Get(first_equal_or_bigger).right_child == nullptr) {
		// case when found last element in sequence and it's less than element
		if (current_node->keys->Get(first_equal_or_bigger).data < element)
			return std::make_pair(current_node, first_equal_or_bigger + 1);
		return std::make_pair(current_node, first_equal_or_bigger);
	}
	// error: internal nodes mustn't have nullptr children
	if (current_node->keys->Get(first_equal_or_bigger).left_child == nullptr ||
		current_node->keys->Get(first_equal_or_bigger).right_child == nullptr)
	{
		throw std::exception("Internal Error: node key has only one child nullptr "
			"(it must have both or neither)");
	}
	// case when found last element in sequence and it's less than element
	if(current_node->keys->Get(first_equal_or_bigger).data < element)
	{
		return Search(element, current_node->keys->Get(first_equal_or_bigger).right_child);
	}
	return Search(element, current_node->keys->Get(first_equal_or_bigger).left_child);
}

template <class T>
std::pair<BTreeNode<T>*, size_t> BTree<T>::Search(T& element)
{
	return Search(element, root_);
}

// Decomposition

template <class T>
void BTree<T>::Add(T& element)
{
	if (root_->keys.GetCount() == 0)
	{
		root_->keys.Append(element);
		return;
	}
	// Search for position of element in the tree
	std::pair<BTreeNode<T>*, size_t> insert_position = Search(element);
	// case when element already exists: we must to insert only in leaves
	while(insert_position.first->keys->Get(insert_position.second).data == element)
	{
		insert_position = Search(element, insert_position.first->keys->
			Get(insert_position.second).left_child);
	}
	// insert element in found position
	BTreeNode<T>* current_node = insert_position.first;
	current_node->keys.InsertAt(Key<T>(element), insert_position.second);
	// dealing with node sequence overflow
	while (current_node->keys.GetCount() >= m_)
	{
		// middle element
		size_t middle_i = current_node->keys.GetCount() / 2;
		// getting subsequences
		Sequence<Key<T>> seq1 = current_node->keys
			.GetSubsequence(0, middle_i - 1);
		Sequence<Key<T>> seq2 = current_node->keys
			.GetSubsequence(middle_i + 1,
				current_node->keys.GetCount());
		// root_ case
		if (current_node->parent == nullptr)
		{
			current_node->parent = new BTreeNode<T>();
		}
		// creating new child nodes from subsequences
		BTreeNode<T>* left_node = new BTreeNode<T>(seq1, current_node->parent);
		BTreeNode<T>* right_node = new BTreeNode<T>(seq2, current_node->parent);
		// creating a new element key
		Key<T> key = Key<T>(
			current_node->keys.Get(middle_i),
			left_node, right_node);
		// inserting key in parent's sequence
		// Search for position to insert key in parent node
		size_t parent_i = binary_search(
			&current_node->parent->keys, key_is_bigger, key.data, 0,
			current_node->parent->keys.GetCount() - 1);
		// inserting key
		current_node->parent->keys.InsertAt(key, parent_i);
		// left and right neighbors must contain pointers to new child nodes
		if (parent_i > 0)
		{
			Key<T> left_neighbor = current_node->parent->keys.Get(parent_i);
			left_neighbor.right_child = left_node;
		}
		if (parent_i < current_node->parent->keys.GetCount() - 1)
		{
			Key<T> right_neighbor = current_node->parent->keys.Get(parent_i + 1);
			right_neighbor.left_child = right_node;
		}
		current_node = current_node->parent;
	}
}