#pragma once
#include "b_tree.h"
#include "person.h"

namespace data_indexing
{
	void print_data(const Person& data, const std::string& filename);
	void print_all(const Sequence<Person>& data, const std::string& filename);

	Person read_data(std::ifstream in);
	Person read_data_at(const std::string& filename, size_t index);
	Sequence<Person>* read_data_by_indices(const std::string& filename, Sequence<size_t>* indices);
	Sequence<Person>* read_all(const std::string& filename);

}