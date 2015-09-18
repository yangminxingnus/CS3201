#pragma once
#include <string>
#include <set>
#include <vector>
#include "InsertType.h"
#include "boost/unordered_set.hpp"

using namespace std;
using boost::unordered_set;
class SingleSynInsert : InsertType
{
public:
	SingleSynInsert(void);
	//inserts the rows into the results table
	void execute(ResultTable&);
	// for clauses with 1 synonym
	void setSyn(string);
	void insertValue(string);

private:
	string singleSyn;
	unordered_set<string> singleInsertValues;
};