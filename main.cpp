#include <iostream>
#include "test_b_tree.h"
#include "u_interface_data_indexing.h"


template <class T = void>
struct less {
	constexpr bool operator()(const T& Left, const T& Right) const {
		return Left < Right;
	}
};

int main()
{
	u_interface_data_indexing::write_persons_from_console();
	u_interface_data_indexing::data_indexing_interface();
	return 0;
}