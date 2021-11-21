#pragma once
#include "b_tree.h"
#include "iostream"

template <class T, class Compare> void print_b_tree(BTree<T, Compare>* tree);
template <class T> void print_b_tree_node_r(BTreeNode<T>* node);

template <class T, class Compare>
void print_b_tree(BTree<T, Compare>* tree)
{
	print_b_tree_node_r(tree->GetRootPointer());
}

template <class T>
void print_b_tree_node_r(BTreeNode<T>* node)
{
	if(node == nullptr || node->keys->GetCount() == 0)
	{
		return;
	}
	std::cout << "{";
	print_b_tree_node_r(node->keys->Get(0).left_child);
	for (size_t i = 0; i < node->keys->GetCount(); ++i)
	{
		std::cout << node->keys->Get(i).data;
		if (i != node->keys->GetCount() - 1) std::cout << ",";
		print_b_tree_node_r(node->keys->Get(i).right_child);
	}
	std::cout << "}";
}