#include <iostream>
#include "b_tree.h"
#include "b_tree_i_o.h"
#include "test_b_tree.h"


template <class T = void>
struct less {
	constexpr bool operator()(const T& Left, const T& Right) const {
		return Left < Right;
	}
};

int main()
{
	tests::b_tree::test_all();
	std::cout << "Tests OK";
	/*
	BTree<int> t(3);
	int tmp = 6;
	t.Add(tmp);
	print_b_tree<int>(&t);
	std::cout << std::endl;
	tmp = 5;
	t.Add(tmp);
	print_b_tree<int>(&t);
	std::cout << std::endl;
	tmp = 5;
	t.Add(tmp);
	print_b_tree<int>(&t);
	std::cout << std::endl;
	tmp = -1;
	t.Add(tmp);
	print_b_tree<int>(&t);
	std::cout << std::endl;
	tmp = -10;
	t.Add(tmp);
	print_b_tree<int>(&t);
	std::cout << std::endl;
	tmp = -1;
	t.Add(tmp);
	print_b_tree<int>(&t);
	std::cout << std::endl;
	tmp = 0;
	t.Add(tmp);
	print_b_tree<int>(&t);
	std::cout << std::endl;
	*/
	return 0;
}