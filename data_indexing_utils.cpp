#include "data_indexing_utils.h"

#include <iostream>
#include <fstream>

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

	std::ofstream print_data(const Person& data, std::ofstream out)
	{
		out << statement_delimiter;
		if (data.name.full_name.length() > name_length)
			out << data.name.full_name.substr(0, name_length);
		else
		{
			out << data.name.full_name;
			out << std::string(name_length - data.name.full_name.length(), empty_symbol);
		}
		out << field_end_symbol;
		out << data.birth_date.to_string() << field_end_symbol;
		out << data.gender << field_end_symbol;
		return out;
	}

	std::ofstream print_all(const Sequence<Person>& data, std::ofstream out)
	{
		for (size_t i = 0; i < data.GetCount(); ++i)
		{
			out = print_data(data.Get(i), move(out));
		}
		return out;
	}

	std::pair<Person, std::ifstream> read_data(std::ifstream in)
	{
		if (!in.is_open())
		{
			std::cerr << "Stream is not opened for reading" << std::endl;
			exit(1);
		}
		if (in.peek() != statement_delimiter)
			throw std::invalid_argument("read_data: wrong structure of the file");
		in.ignore();
		Person person;
		while (in.peek() != empty_symbol && in.peek() != field_end_symbol)
		{
			char t;
			in >> t;
			person.name.full_name += t;
		}
		while (in.peek() != field_end_symbol)
		{
			in.ignore();
		}
		in.ignore();
		std::string tmp;
		for (int i = 0; i < 4; ++i)
		{
			char t;
			in >> t;
			tmp += t;
		}
		person.birth_date.year = stoi(tmp);
		tmp = "";
		for (int i = 0; i < 2; ++i)
		{
			char t;
			in >> t;
			tmp += t;
		}
		person.birth_date.month = stoi(tmp);
		tmp = "";
		for (int i = 0; i < 2; ++i)
		{
			char t;
			in >> t;
			tmp += t;
		}
		person.birth_date.day = stoi(tmp);
		in.ignore();
		char t;
		in >> t;
		person.gender = Gender(t);
		return std::make_pair(person, in);
	}

	std::pair<Person, std::ifstream> read_data_at(std::ifstream in, size_t index)
	{
		for (size_t i = 1; i < index; ++i)
		{
			in.ignore(statement_length);
		}
		return read_data(std::move(in));
	}

	std::pair<Sequence<Person>*, std::ifstream> read_data_by_indices(std::ifstream in, Sequence<size_t>* indices)
	{
		Sequence<Person>* res = new ArraySequence<Person>();
		if (indices->GetCount() == 0) return std::make_pair(res, in);
		auto a = read_data_at(std::move(in), indices->Get(0));
		res->Append(a.first);
		in = a.second;
		size_t diff_index;
		for (size_t i = 1; i < indices->GetCount(); ++i) {
			diff_index = indices->Get(i) - indices->Get(i - 1);
			auto a = read_data_at(std::move(in), diff_index);
			res->Append(a.first);
			in = a.second;
		}
		return std::make_pair(res, in);
	}

	Sequence<Person>* read_all(const std::string& filename)
	{
		std::ifstream in;
		in.open(filename);
		if (!in)
		{
			std::cerr << "File " << filename << " could not be opened for reading" << std::endl;
			exit(1);
		}
		Sequence<Person>* res = new ArraySequence<Person>;
		while (!in.eof()) {
			auto a = read_data(std::move(in));
			Person p = a.first;
			in = a.second;
			res->Append(p);
		}
		in.close();
		return res;
	}

}