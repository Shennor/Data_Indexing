#pragma once
#include "b_tree.h"
#include "iostream"

template <class T> void print_b_tree(BTree<T>* tree);
template <class T> void print_b_tree_node_r(BTreeNode<T>* node);

template <class T>
void print_b_tree(BTree<T>* tree)
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
	//for(int i = 0; i < node->keys->GetCount(); ++i)
	//{
	//		std::cout << node->keys->Get(i).data << ",";
	//}
	print_b_tree_node_r(node->keys->Get(0).left_child);
	for (int i = 0; i < node->keys->GetCount(); ++i)
	{
		std::cout << node->keys->Get(i).data;
		if (i != node->keys->GetCount() - 1) std::cout << ",";
		print_b_tree_node_r(node->keys->Get(i).right_child);
	}
	std::cout << "}";
}