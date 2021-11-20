#pragma once

namespace tests
{
namespace b_tree
{

int three_way_cmp(const int& a, const int& b);

void test_constructors();
void test_destructor();
void test_search();
void test_add();

void test_all();

}
}