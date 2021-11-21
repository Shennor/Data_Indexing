#include "data_indexing_utils.h"
#include "test_b_tree.h"
#include "person.h"
#include "fstream"
#include  <iostream>

using namespace std;

namespace u_interface_data_indexing
{

	const string filename = "C:\\Users\\Hupu\\source\\repos\\Optimal hotel accommodation\\person.txt";

	inline int get_value()
	{
		std::cout << "Enter number >> ";
		int value;
		std::cin >> value;
		return value;
	}

	int menu()
	{
		cout << "MENU\n";
		cout << "1 : Launch automatic tests\n";
		cout << "2 : Print some data to console\n";
		cout << "3 : Select data from intervals\n";
		cout << "4 : End program\n";
		int res = get_value();
		while (res < 1 || res > 4)
		{
			cout << "You've written the wrong number!\n";
			res = get_value();
		}
		return res;
	}

	void print_some_data(Sequence<Person>* persons)
	{
		size_t n = 101;
		while (n > 100) {
			cout << "Enter number of elements to print >>";
			cin >> n;
			cout << endl;
			if (n <= 100) break;
			cout << "Number must be from 0 to 100.\n";
		}
		cout << "First " << n << " persons:\n";
		for(size_t i = 0; i < n; ++i)
		{
			cout << persons->Get(i).name.full_name << " "
			<< persons->Get(i).birth_date.to_string() << " "
			<< persons->Get(i).gender << endl;
		}
	}
	
	struct connected_index
	{
		size_t index = 0;
		Sequence<Person>* seq = nullptr;

		connected_index() = default;
		connected_index(size_t new_index, Sequence<Person>* new_seq )
		{
			index = new_index;
			seq = new_seq;
		}
	};
	
	struct indices_less_by_persons_names {
		bool operator()(const connected_index& Left, 
			const connected_index& Right) const {
			person_name_less c{};
			return c(Left.seq->Get(Left.index), Right.seq->Get(Right.index));
		}
	};
	
	struct indices_less_by_persons_birth_date {

		bool operator()(const connected_index& Left, 
			const connected_index& Right) const {
			person_birth_date_less c{};
			return c(Left.seq->Get(Left.index), Right.seq->Get(Right.index));
		}
	};
	
	template<class T>
	Sequence<connected_index>* select_by_intervals(BTree<connected_index, T>& tree,
		Sequence<Person>* persons)
	{
		size_t n = 0;
		while (n < 1) {
			cout << "Enter number of intervals (al least 1) >>";
			cin >> n;
			cout << endl;
		}
		Sequence<connected_index>* selected_indices = new ArraySequence<connected_index>();
		cout << "Enter non-overlapping intervals of indices:\n";
		cout << "1\n";
		size_t begin;
		size_t end;
		cin >> begin;
		cin >> end;
		connected_index a{ begin, persons };
		connected_index b{ end, persons };
		Sequence<connected_index>* found = interval_from_b_tree(tree, a, b);
		for(size_t i = 0; i < found->GetCount(); ++i)
		{
			selected_indices->Append(found->Get(i));
		}
		string new_begin;
		string new_end;
		for (size_t i = 1; i < n; ++i)
		{
			cout << i + 1 << endl;
			cin >> new_begin;
			cin >> new_end;
			connected_index a{ begin, persons };
			connected_index b{ end, persons };
			found = interval_from_b_tree(tree, a, b);
			for (size_t i = 0; i < found->GetCount(); ++i)
			{
				selected_indices->Append(found->Get(i));
			}
		}
		return selected_indices;
	}
	
	void data_indexing_interface()
	{
		// reading data from file
		cout << filename;
		Sequence<Person>* persons = data_indexing::read_all(filename);
		Sequence<connected_index>* indices = new ArraySequence<connected_index>();
		for(size_t i = 0; i < persons->GetCount(); ++i)
		{
			indices->Append(connected_index(i, persons));
		}
		size_t max = 10;
		BTree<connected_index, indices_less_by_persons_names> tree_indices_by_name(indices, max);
		BTree<connected_index, indices_less_by_persons_birth_date> tree_indices_by_date(indices, max);
		/*ifstream in;
		in.open(filename);
		if (!in)
		{
			std::cerr << "File " << filename << " could not be opened for printing" << std::endl;
			exit(1);
		}*/
		cout << "Welcome to Data Indexing User Interface.\n";
		int menu_res = menu();
		while (menu_res != 4)
		{
			switch (menu_res)
			{
			case 1:
				tests::b_tree::test_all();
				cout << "Tests are OK\n";
				break;
			case 2:
				print_some_data(persons);
				break;
			default:
				int tmp;
				cout << "Print 0 to select by name and other number to select by birthday >>\n";
				cin >> tmp;
				Sequence<connected_index>* result;
				if (tmp == 0)
					result = select_by_intervals(tree_indices_by_name, persons);
				else 
					result = select_by_intervals(tree_indices_by_date, persons);
				for(size_t i = 0; i < result->GetCount(); ++i)
				{
					cout << result->Get(i).seq->Get(result->Get(i).index).to_string() << endl;
				}
				break;
			}
			cout << endl;
			menu_res = menu();
			cout << endl;
		}
	}

	void write_persons_from_console()
	{
		cout << "How many persons do you want to write?\n";
		size_t cnt;
		cin >> cnt;
		ofstream out;
		out.open(filename);
		if(!out)
		{
			cout << "Can't open file\n";
		}
		cout << "Enter name, birth date and gender\n";
		for(size_t i = 0; i < cnt; ++i)
		{
			Person p;
			cout << i + 1 << endl;
			string name;
			cin >> name;
			p.name.full_name = name;
			unsigned int date;
			cin >> date;
			date = date % 100000000;
			p.birth_date.year = date / 10000;
			date = date / 10000;
			p.birth_date.month = date / 100;
			date = date / 100;
			p.birth_date.day = date;
			char gender;
			cin >> gender;
			p.gender = Gender(gender);
			out = data_indexing::print_data(p, move(out));
		}
		out.close();
	}

}