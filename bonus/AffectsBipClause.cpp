#include "AffectsBipClause.h"
#include "AffectsBipCalculator.h"
#include "AssgGNode.h"
#include "DummyGNode.h"
#include "IfGNode.h"
#include "WhileGNode.h"
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <assert.h>
#include <iostream>

AffectsBipClause::AffectsBipClause(void):SuchThatClause(AFFECTS_) {
	stmtTable = StmtTable::getInstance();
	procTable = ProcTable::getInstance();
}

AffectsBipClause::~AffectsBipClause(void){
	
}

//to add if statements
bool AffectsBipClause::isValid(void){
	string firstType = this->getFirstArgType();
	string secondType = this->getSecondArgType();
	bool firstArg = (firstType == stringconst::ARG_ASSIGN) || (firstType == stringconst::ARG_STATEMENT) || (firstType == stringconst::ARG_PROGLINE) || (firstType == stringconst::ARG_GENERIC);
	bool secondArg = (secondType == stringconst::ARG_ASSIGN) || (secondType == stringconst::ARG_STATEMENT) || (secondType == stringconst::ARG_PROGLINE) || (secondType == stringconst::ARG_GENERIC);
	return firstArg && secondArg;
}

//e.g. Parent(1,2)
bool AffectsBipClause::evaluateS1FixedS2Fixed(string firstArg, string secondArg) {
	AffectsBipCalculator calc = AffectsBipCalculator();
	return calc.computeFixedFixed(firstArg, secondArg);
	return false;
}

//e.g. Parent(_,_)
bool AffectsBipClause::evaluateS1GenericS2Generic() {
	AffectsBipCalculator calc = AffectsBipCalculator();
	return calc.computeS1GenericS2Generic();
}

//e.g. Parent(2, s2)
unordered_set<string> AffectsBipClause::getAllS2WithS1Fixed(string s1) {
	AffectsBipCalculator calc = AffectsBipCalculator();
	return calc.computeFixedSyn(s1);
}

//e.g. Parent(_, s2)
//get all children
unordered_set<string> AffectsBipClause::getAllS2() {
	AffectsBipCalculator calc = AffectsBipCalculator();
	return calc.computeAllS2();
}

//e.g. Parent(2,_)
bool AffectsBipClause::evaluateS1FixedS2Generic(string s1){
	AffectsBipCalculator calc = AffectsBipCalculator();
	return calc.computeS1FixedS2Generic(s1);
}

//e.g. Affects(_, 2)
bool AffectsBipClause::evaluateS1GenericS2Fixed(string s2) {
	unordered_set<Statement*> assgStmts = stmtTable->getAssgStmts();
	BOOST_FOREACH(Statement* assgStmt, assgStmts) {
		AffectsBipCalculator calc = AffectsBipCalculator();
		if (calc.computeFixedFixed(boost::lexical_cast<string>(assgStmt->getStmtNum()), s2)) {
			return true;
		}
	}
	return false;
}

//e.g. Affects(s,2)
unordered_set<string> AffectsBipClause::getAllS1WithS2Fixed(string s2) {
	AffectsBipCalculator calc = AffectsBipCalculator();
	unordered_set<vector<string>> s1s2 = calc.computeAllS1AndS2(false);
	unordered_set<string> result = unordered_set<string>();
	BOOST_FOREACH(vector<string> pair, s1s2) {
		if (pair.at(1) == s2) {
			result.insert(pair.at(0));
		}
	}
	return result;
}

//e.g. Parent(s1,_)
//get all parents
unordered_set<string> AffectsBipClause::getAllS1() {
	AffectsBipCalculator calc = AffectsBipCalculator();
	return calc.computeAllS1();
}

//Parent(s1, s2)
unordered_set<vector<string>> AffectsBipClause::getAllS1AndS2() {
	AffectsBipCalculator calc = AffectsBipCalculator();
	return calc.computeAllS1AndS2(firstArg == secondArg);
}
