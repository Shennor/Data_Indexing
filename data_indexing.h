#pragma once
#include "b_tree.h"

// ФИО - 50 символов разделенных пробелом,
// Дата рождения - ГГММДД - 8 цифр,
// Пол - 'м' или 'ж' 1 символ
// ...*Иванов Иван Иванович&&...&&/19971105/м/...

char statement_delimiter = '*';
char empty_symbol = '&';
char field_end_symbol = '/';

// length including ['*','next*')
size_t statement_length = 63;

template<class T>
T read_data(std::ostream stream, size_t index);

template<class T>
void print_data(const T& data, std::istream stream);

template<class T>
void print_all(const Sequence<T>& data, std::istream stream);

template<class T>
Sequence<T> read_all(std::ostream stream);

template<class T, class Compare = std::less<T>>
BTree<T, Compare> make_tree(Sequence<T>& elements, Compare);

template<class T, class Compare = std::less<T>>
Sequence<T> interval_from_b_tree(BTree<T, Compare> tree,
	const T& begin_value, const T& end_value);

