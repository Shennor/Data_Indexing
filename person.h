#pragma once
#include <string>

enum Gender
{
	male = 'M',
	female = 'F'
};

struct Date
{
	short day;
	short month;
	unsigned int year;

	std::string to_string() const
	{
		std::string res;
		if (year < 10) res += "000";
		else if (year < 100) res += "00";
		else if (year < 1000) res += "0";
		res += std::to_string((year % 10000));
		if (month < 10) res += "0";
		res += std::to_string((month % 100));
		if (day < 10) res += "0";
		res += std::to_string((day % 100));
		return res;
	}
};

inline bool operator<(const Date& first, const Date& second)
{
	if (first.year < second.year) return true;
	if (first.month < second.month) return true;
	if (first.day < second.day) return true;
	return false;
}

struct Name
{
	std::string full_name;
};

inline bool operator<(const Name& first, const Name& second)
{
	return std::strcmp(first.full_name.data(), second.full_name.data()) < 0;
}

struct Person
{
	Name name;
	Date birth_date;
	Gender gender;

	std::string to_string() const 
	{
		std::string res = name.full_name + " ";
		res += birth_date.to_string() + " ";
		res += gender;
		return res;
	}
};

// Comparers

struct person_name_less {
	bool operator()(const Person& Left, const Person& Right) const {
		return Left.name < Right.name;
	}
};

struct person_birth_date_less {
	bool operator()(const Person& Left, const Person& Right) const {
		return Left.birth_date < Right.birth_date;
	}
};
