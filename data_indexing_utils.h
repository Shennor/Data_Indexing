#pragma once
#include "b_tree.h"
#include "person.h"

namespace data_indexing
{
	std::ofstream print_data(const Person& data, std::ofstream out);
	std::ofstream print_all(const Sequence<Person>& data, std::ofstream out);

	std::pair<Person, std::ifstream> read_data(std::ifstream in);
	std::pair<Person, std::ifstream> read_data_at(std::ifstream in, size_t index);
	std::pair<Sequence<Person>*, std::ifstream> read_data_by_indices(std::ifstream in, Sequence<size_t>* indices);
	Sequence<Person>* read_all(const std::string& filename);

}