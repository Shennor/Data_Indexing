#include <iostream>
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
	return 0;
}