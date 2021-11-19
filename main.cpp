#include <iostream>
#include "b_tree.h"
#include "b_tree_i_o.h"
#include "test_b_tree.h"

#include <set>

// индексирование (в т.ч. поисковые задачи)
// поиск по интервалам
// 
// в файле (массиве) много данных, а индексы хранить в дереве
// в файле индекс - позиция, с которой элемент начинается
// фиксировать максимальную длину ФИО (рамзмер записи в цедлм, размер каждого поля, иметь символ - символ конца строки, символ конец записи)
// символ с кодом ноль - символ конца строки \0
// проследить, чтобы не было коллизии спец-символа с символом конца файла eof


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
	BTree<int> t(3);
	int tmp = 6;
	t.Add(tmp);
	print_b_tree<int>(&t);
	std::cout << std::endl;
	tmp = 5;
	t.Add(tmp);
	print_b_tree<int>(&t);
	std::cout << std::endl;
	tmp = 50;
	t.Add(tmp);
	print_b_tree<int>(&t);
	std::cout << std::endl;
	tmp = -10;
	t.Add(tmp);
	print_b_tree<int>(&t);
	std::cout << std::endl;
	tmp = -20;
	t.Add(tmp);
	print_b_tree<int>(&t);
	std::cout << std::endl;
	tmp = -1229;
	t.Add(tmp);
	print_b_tree<int>(&t);
	std::cout << std::endl;
	tmp = 0;
	t.Add(tmp);
	print_b_tree<int>(&t);
	std::cout << std::endl;
	return 0;
}