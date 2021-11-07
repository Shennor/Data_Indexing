#include <iostream>

#include "test_b_tree.h"

int main()
{
	tests::b_tree::test_all();
	std::cout << "Tests OK";
	return 0;
}