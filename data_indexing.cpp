#include "data_indexing.h"

template <class T, class Compare>
BTree<T, Compare> make_tree(Sequence<T>& elements, Compare)
{
	BTree<T, Compare> t{};
	for (size_t i = 0; i < elements.GetCount(); ++i) t.Add(elements.Get(i));
	return t;
}

template<class T, class Compare>
void interval_from_node_r(
	BTreeNode<T> current, Sequence<T>& res, const T& begin_value, const T& end_value)
{
	Compare c{};
	// returns first equal or bigger
	size_t start_i = binary_search(
		current.keys, begin_value, 0, current.keys->GetCount() - 1, c);
	size_t end_i = binary_search(
		current.keys, end_value, 0, current.keys->GetCount() - 1, c);
	size_t i = start_i;
	for(; i < end_i; ++i)
	{
		interval_from_node_r<T,Compare>(current.keys->Get(i).left_child, 
			res, begin_value, end_value);
		res.Append(current.keys->Get(i).data);
	}
	while(i + 1 < current.keys->GetCount() && 
		current.keys->Get(i + 1) == end_value)
	{
		++i;
		interval_from_node_r<T, Compare>(current.keys->Get(i).left_child, 
			res, begin_value, end_value);
		res.Append(current.keys->Get(i).data);
	}
}

template <class T, class Compare>
Sequence<T> interval_from_b_tree(
	BTree<T, Compare> tree, const T& begin_value, const T& end_value)
{
	Sequence<T> res{};
	interval_from_node_r<T, Compare>(tree.GetRootPointer(), res, begin_value, end_value);
	return res;
}