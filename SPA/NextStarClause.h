/*
 * NextStarClause.h
 *
 *  Created on: 14 Oct, 2015
 *      Author: Leon
 */

#pragma once
#include "CFG.h"
#include "SuchThatClause.h"
#include "StmtTable.h"

using namespace boost;


class NextStarClause
	: public SuchThatClause {

public:
	NextStarClause(void);
	~NextStarClause(void);

	bool isValid(void);

protected:

	//e.g. Parent(string,string)
	bool evaluateS1FixedS2Fixed(string, string);
	//e.g. Parent(_,_)
	bool evaluateS1GenericS2Generic();
	//e.g. Parent(_,string)
	bool evaluateS1GenericS2Fixed(string);
	//Parent(string,_)
	bool evaluateS1FixedS2Generic(string);
	//Parent(string,s2)
	unordered_set<string> getAllS2WithS1Fixed(string);
	//Parent(_,s2)
	unordered_set<string> getAllS2();
	//Parent(s1,string)
	unordered_set<string> getAllS1WithS2Fixed(string);
	//Parent(s1,__)
	unordered_set<string> getAllS1();
	//Parent(s1,s2)
	unordered_set<vector<string>> getAllS1AndS2();

private:
	bool match;
	CFG* cfg;
	unordered_set<string> results;
	unordered_set<vector<string>> resultsPair;

	void dfsFind(Statement*, string, vector<string>);
	void dfsFindPrev(Statement*, vector<string>, string);
	void dfsFindNext(Statement*, vector<string>, string);
	void dfsFindPair(Statement*, vector<string>, string, string, bool);
	bool contains(vector<string>, string);
	int getPosition(vector<string>, string);

	unordered_set<Statement*> getNeededStmts(string);
	bool isNeededArgType(string, int);
};