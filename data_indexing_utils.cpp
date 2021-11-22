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
	size_t statement_length = name_length + 14;

	void print_data(const Person& data, const std::string& filename)
	{
		std::ofstream out;
		out.open(filename, std::ios::app);
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
		out.close();
	}

	void print_all(const Sequence<Person>& data, const std::string& filename)
	{
		for (size_t i = 0; i < data.GetCount(); ++i)
		{
			print_data(data.Get(i), filename);
		}
	}

	Person read_data(std::ifstream in)
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
		short t;
		in >> t;
		person.gender = Gender(t);
		in.close();
		return person;
	}

	Person read_data_at(const std::string& filename, size_t index)
	{
		std::ifstream in;
		in.open(filename);
		if(!in || in.peek() != '*')
		{
			std::cerr << "Wrong data format";
			exit(1);
		}
		for (size_t i = 0; i < index; ++i)
		{
			in.ignore(statement_length);
		}
		if (in.eof()) throw std::invalid_argument("read index out of range");
		return read_data(std::move(in));
	}

	Sequence<Person>* read_data_by_indices(const std::string& filename, Sequence<size_t>* indices)
	{
		Sequence<Person>* res = new ArraySequence<Person>();
		if (indices->GetCount() == 0) return res;
		for (size_t i = 1; i < indices->GetCount(); ++i) {
			res->Append(read_data_at(filename, indices->Get(0)));
		}
		return res;
	}

	Sequence<Person>* read_all(const std::string& filename)
	{
		Sequence<Person>* res = new ArraySequence<Person>;
		size_t i = 0;
		Person p;
		while (true) {
			try
			{
				p = read_data_at(filename, i);
			}
			catch(std::invalid_argument& e)
			{
				return res;
			}
			res->Append(p);
			++i;
		}
	}

}