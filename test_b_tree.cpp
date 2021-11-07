#include "test_b_tree.h"
#include <cassert>
#include "b_tree.h"
#include "b_tree_i_o.h"

namespace tests
{
namespace b_tree
{

void test_constructors()
{
	BTree<int> t1(4);
	assert(t1.GetM() == 4);
	assert(t1.GetRootPointer()->keys->GetCount() == 0);
	assert(t1.GetRootPointer()->parent == nullptr);

	BTree<char> t2('b', 2);
	assert(t2.GetM() == 2);
	assert(t2.GetRootPointer()->keys->GetCount() == 1);
	assert(t2.GetRootPointer()->parent == nullptr);
	assert(t2.GetRootPointer()->keys->GetFirst().data == 'b');
	assert(t2.GetRootPointer()->keys->GetFirst().left_child == nullptr);
	assert(t2.GetRootPointer()->keys->GetFirst().right_child == nullptr);

	BTree<char> t3(*t2.GetRootPointer(), 3);
	assert(t3.GetM() == 3);
	assert(t3.GetRootPointer()->keys->GetCount() == 1);
	assert(t3.GetRootPointer()->parent == nullptr);
	assert(t3.GetRootPointer()->keys->GetFirst().data == 'b');
	assert(t3.GetRootPointer()->keys->GetFirst().left_child == nullptr);
	assert(t3.GetRootPointer()->keys->GetFirst().right_child == nullptr);
	assert(t3.GetRootPointer() != t2.GetRootPointer());

	BTree<char> t4 = t2;
	assert(t4.GetM() == 2);
	assert(t4.GetRootPointer()->keys->GetCount() == 1);
	assert(t4.GetRootPointer()->parent == nullptr);
	assert(t4.GetRootPointer()->keys->GetFirst().data == 'b');
	assert(t4.GetRootPointer()->keys->GetFirst().left_child == nullptr);
	assert(t4.GetRootPointer()->keys->GetFirst().right_child == nullptr);
	assert(t4.GetRootPointer() != t2.GetRootPointer());
}

void test_search()
{
	//          2
	//        /   \
	//      -1     5,8
	//     /  \  /  |  \
	//    -3  1	 4  6  9
	//          n8
	//        /    \
	//      n3      n7
	//     / \   /  |  \
	//    n1 n2  n4  n5  n6
	BTreeNode<int>* n1 = new BTreeNode<int>(-3);
	BTreeNode<int>* n2 = new BTreeNode<int>(1);
	BTreeNode<int>* n3 = new BTreeNode<int>();
	n1->parent = n3;
	n2->parent = n3;
	n3->keys->Append(Key<int>(-1, n1, n2));
	BTreeNode<int>* n4 = new BTreeNode<int>(4);
	BTreeNode<int>* n5 = new BTreeNode<int>(6);
	BTreeNode<int>* n6 = new BTreeNode<int>(9);
	BTreeNode<int>* n7 = new BTreeNode<int>();
	n4->parent = n7;
	n5->parent = n7;
	n6->parent = n7;
	n7->keys->Append(Key<int>(5, n4, n5));
	n7->keys->Append(Key<int>(8, n5, n6));
	BTreeNode<int>* n8 = new BTreeNode<int>();
	n8->keys->Append(Key<int>(2, n3, n7));
	n3->parent = n8;
	n7->parent = n8;
	BTree<int> t1(*n8, 3);
	//print_b_tree(&t1);
	// Search existing elements
	int a = 2;
	auto res = t1.Search(a);
	assert(res.first == t1.GetRootPointer());
	assert(res.second == 0);
	a = -1;
	res = t1.Search(a);
	assert(res.first == t1.GetRootPointer()->keys->Get(0)
						.left_child);
	assert(res.second == 0);
	a = -3;
	res = t1.Search(a);
	assert(res.first == t1.GetRootPointer()->keys->Get(0)
						.left_child->keys->Get(0)
						.left_child);
	assert(res.second == 0);
	a = 1;
	res = t1.Search(a);
	assert(res.first == t1.GetRootPointer()->keys->Get(0)
						.left_child->keys->Get(0)
						.right_child);
	assert(res.second == 0);
	a = 5;
	res = t1.Search(a);
	assert(res.first == t1.GetRootPointer()->keys->Get(0)
						.right_child);
	assert(res.second == 0);
	a = 8;
	res = t1.Search(a);
	assert(res.first == t1.GetRootPointer()->keys->Get(0)
						.right_child);
	assert(res.second == 1);
	a = 4;
	res = t1.Search(a);
	assert(res.first == t1.GetRootPointer()->keys->Get(0)
						.right_child->keys->Get(0)
						.left_child);
	assert(res.second == 0);
	a = 6;
	res = t1.Search(a);
	assert(key_is_bigger(res.first->keys->GetFirst(), t1.GetRootPointer()->keys->Get(0)
						.right_child->keys->Get(0)
						.right_child->keys->GetFirst()) == 0);
	assert(key_is_bigger(res.first->keys->GetFirst(), t1.GetRootPointer()->keys->Get(0)
						.right_child->keys->Get(1)
						.left_child->keys->GetFirst()) == 0);
	assert(res.second == 0);
	a = 9;
	res = t1.Search(a);
	assert(res.first == t1.GetRootPointer()->keys->Get(0)
						.right_child->keys->Get(1)
						.right_child);
	assert(res.second == 0);
	// Search places to insert (non existing elements)
	a = 0;
	res = t1.Search(a);
	assert(res.first == t1.GetRootPointer()->keys->Get(0)
						.left_child->keys->Get(0)
						.right_child);
	assert(res.second == 0);
	a = 7;
	res = t1.Search(a);
	assert(res.first->keys->GetFirst().data == t1.GetRootPointer()->keys->Get(0)
						.right_child->keys->Get(0)
						.right_child->keys->GetFirst().data);
	assert(res.first->keys->GetFirst().data == t1.GetRootPointer()->keys->Get(0)
						.right_child->keys->Get(1)
						.left_child->keys->GetFirst().data);
	assert(res.second == 1);

	BTree<char> t2(4);
	char b = 'b';
	auto res2 = t2.Search(b);
	assert(res2.first == t2.GetRootPointer());
	assert(res2.second == 0);

	// deleting tree
	delete n1;
	delete n2;
	delete n3;
	delete n4;
	delete n5;
	delete n6;
	delete n7;
	delete n8;
}
	
void test_add()
{
	BTree<int> t1(4);
	int tmp = 1;
	t1.Add(tmp);
	assert(t1.GetRootPointer()->keys->GetCount() == 1);
	assert(t1.GetRootPointer()->keys->GetFirst().data == 1);
	assert(t1.GetRootPointer()->keys->GetFirst().left_child == nullptr);
	assert(t1.GetRootPointer()->keys->GetFirst().right_child == nullptr);
	tmp = 0;
	t1.Add(tmp)
}

void test_all()
{
	test_constructors();
	test_search();
	test_add();
}


}
}