#pragma once
#include "b_tree.h"
#include "person.h"

namespace data_indexing {

	char statement_delimiter = '*';
	char empty_symbol = '&';
	char field_end_symbol = '/';

	// ФИО - 50 символов разделенных пробелом,
	// Дата рождения - ГГММДД - 8 цифр,
	// Пол - 'м' или 'ж' 1 символ
	// ...*Иванов Иван Иванович&&...&&/19971105/M/...

	// length including ['*','next*')
	size_t name_length = 50;
	size_t statement_length = name_length + 13;

	void print_data(const Person& data, const std::string& filename);
	void print_all(const Sequence<Person>& data, const std::string& filename);

	Person read_data(std::ifstream in);
	Person read_data_at(std::ifstream in, size_t index);
	Sequence<Person>* read_data_by_indices(std::ifstream in, Sequence<size_t>* indices);
	Sequence<Person>* read_all(const std::string& filename);

}