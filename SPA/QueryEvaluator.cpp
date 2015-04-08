#include "QueryEvaluator.h"
#include "Clause.h"
#include "StringPair.h"
#include "Utils.h"
#include "StmtTable.h"
#include "VarTable.h"
#include <vector>
#include <sstream>

using namespace std;

QueryEvaluator::QueryEvaluator(void)
{

}

QueryEvaluator::~QueryEvaluator(void)
{
}

set<string> getAllSynValues(vector<StringPair> selectList) {
	StringPair syn = selectList.at(0);
	string synType = syn.getSecond();
	set<string> *results = new set<string>();

	if (synType == stringconst::ARG_WHILE) {
		StmtTable* stmtTable = StmtTable::getInstance();
		set<Statement*> whileList = stmtTable->getWhileStmts();
		for (set<Statement*>::iterator iter = whileList.begin() ; iter != whileList.end(); iter++) {
			Statement* stmtObj = *iter;
			int stmtNum = stmtObj->getStmtNum();
			
			string s;
			ostringstream convert; // stringstream int to string
			convert << stmtNum;
			s = convert.str();
			results->insert(s);

			results->insert(stmtNum);
		}
		return *results;

	} else if (synType == stringconst::ARG_STATEMENT) {
		StmtTable* stmtTable = StmtTable::getInstance();
		boost::unordered_map<int, Statement*>::iterator iter;
		for (iter = stmtTable->getIterator(); iter != stmtTable->getEnd(); iter++) {
			int stmtNum = iter->first;
			
			string s;
			ostringstream convert; // stringstream int to string
			convert << stmtNum;
			s = convert.str();
			results->insert(s); // might need to flush. currently not flushing
		}
		return *results;

	} else if (synType == stringconst::ARG_ASSIGN) {
		StmtTable* stmtTable = StmtTable::getInstance();
		set<Statement*> assignList = stmtTable->getAssgStmts();
		for (set<Statement*>::iterator iter = assignList.begin() ; iter != assignList.end(); iter++) {
			Statement* stmtObj = *iter;
			int stmtNum = stmtObj->getStmtNum();
			
			string s;
			ostringstream convert; // stringstream int to string
			convert << stmtNum;
			s = convert.str();
			results->insert(s);

			results->insert(stmtNum);
		}
		return *results;

	} else if (synType == stringconst::ARG_VARIABLE) {
		VarTable* varTable = VarTable::getInstance();
		boost::unordered_map<string, Variable*>::iterator iter;
		for (iter = varTable->getIterator(); iter != varTable->getEnd(); iter++) {
			results->insert(*iter); // might need to flush. currently not flushing
		}
		return *results;

	} else {
		//error
		return set<string>();
	}
	// check the syn type
	// get from the respective tables
	// convert all to string (if required)
	// return set of strings
	
}

// return the NUMBER of times syn appear in both obj1 and obj2
int getSameClause(Results obj1, Results obj2) {
	int numSynObj1 = obj1.getNumOfSyn();
	int numSynObj2 = obj2.getNumOfSyn();

	if (numSynObj1 == 2 && numSynObj2 == 1) {
		getSameClause(obj2, obj1);
	}
	string firstSyn;
	string secondSyn;
	string thirdSyn;
	string forthSyn;
	int count = 0;

	if (numSynObj1 == 1 && numSynObj2 == 1) {
		firstSyn = obj1.getFirstClauseSyn();
		secondSyn = obj2.getFirstClauseSyn();
		if (firstSyn == secondSyn) {
			return 1;
		} else {
			return 0;
		}
	}

	if (numSynObj1 == 1 && numSynObj2 == 2) {
		firstSyn = obj1.getFirstClauseSyn();
		secondSyn = obj2.getFirstClauseSyn();
		thirdSyn = obj2.getSecondClauseSyn();
		
		if (firstSyn == secondSyn) {
			count++;
		}

		if (firstSyn == thirdSyn) {
			count++;
		}
		return count;
	}

	if (numSynObj1 == 2 && numSynObj2 == 2) {
		firstSyn = obj1.getFirstClauseSyn();
		secondSyn = obj1.getSecondClauseSyn();
		thirdSyn = obj2.getFirstClauseSyn();
		forthSyn = obj2.getSecondClauseSyn();

		if (firstSyn == firstSyn) {
			count++;
		}

		if (secondSyn == thirdSyn) {
			count++;
		}

		if (secondSyn == forthSyn) {
			count++;
		}
		return count;
	}
	return count;
}

// get the results of select syn. Have to pull results from both pair and single results
set<string> getSelectSynResult(Results mergedResult, vector<StringPair> selectList) {
	return set<string>();
}

set<string> evaluateOneClause(Results res, vector<StringPair> selectList) {
	string syn = selectList.at(0).getFirst();
	if (res.usesSelectSyn(syn)) {
		set<string> result = getSelectSynResult(res, selectList);
		return result;
	}

	if (res.isClauseTrue()) {
		set<string> result = getAllSynValues(selectList);
		return result;
	}

	return set<string>();
	/*
	3.if resultsList.size == 1 // there is only 1 query
	if selectSynonym is used in resultObj // helper function to compare
		return getSingleResults

	else // if selectSynonym is not used in resultObj
		if (resultObj.isClauseTrue())
			return all possible values of selectSyn
		else
			return none
	*/
}

set<string> evaluateManyClause(vector<Results> resultList, vector<StringPair> selectList) {
	Results obj1 = resultList.front();
	Results obj2 = resultList.back();
	int numRepeatingClause = getSameClause(obj1, obj2);
	string syn = selectList.at(0).getFirst();
	
	switch (numRepeatingClause) {
		case 0 : 
			if (obj1.isClauseTrue() && obj2.isClauseTrue()) {
				set<string> result = getAllSynValues(selectList);
				return result;
			} else {
				return set<string>();
			}
			break;

		case 1 :
			if (obj1.isClauseTrue() && obj2.isClauseTrue()) {
				Results mergedResult = obj1.getIntercept(obj2);
				vector<string> res = mergedResult.getSinglesResults();

				if (obj1.usesSelectSyn(syn) || obj2.usesSelectSyn(syn)) {
					set<string> result = getSelectSynResult(mergedResult, selectList);
					return result;
				} 

				if (!res.empty()) {
					set<string> result = getAllSynValues(selectList);
					return result;
				} 
			} 
			return set<string>();

		case 2 :
			if (obj1.isClauseTrue() && obj2.isClauseTrue()) {
				Results mergedResult = obj1.getIntersect(obj2);
				vector<pair<string, string>> res = mergedResult.getPairResults();

				if (obj1.usesSelectSyn(syn) || obj2.usesSelectSyn(syn)) {
					set<string> result = getSelectSynResult(mergedResult, selectList);
					return result;
				}
				
				if (!res.empty()) {
					set<string> result = getAllSynValues(selectList);
					return result;
				} 
			}
			return set<string>();

		default :
			// error
			return set<string>();
	}
	/*
	if both clauses use the same syn AND a clause uses select syn
		reduce result set via elimination and return result
	if both clauses use the same syn AND no clause uses select syn
		check if both clauses are valid
			if both are valid, return all possible select syn
			if 1 clause is invalid, return none
	if clauses do not use the same syn, 
		check if both are valid
			if both are valid, return all possible select syn
			if 1 clause is invalid, return none
	*/
	
}

set<string> QueryEvaluator::evaluateQuery(Query q) {
	vector<Clause*> clauseList = q.getClauseList();
	vector<StringPair> selectList = q.getSelectList();
	
	if (clauseList.empty()) {
		set<string> result = getAllSynValues(selectList);
		return result;

	} else {
		vector<Results> resultsList;

		for (vector<Clause*>::iterator iter = clauseList.begin() ; iter != clauseList.end(); iter++) {
			Clause* c = *iter;
			Results res = c->evaluate();
			resultsList.push_back(res);
		}

		if (resultsList.size() == 1) {
			Results resultObj= resultsList.front();
			set<string> result = evaluateOneClause(resultObj, selectList);
			return result;

		} else {
			set<string> result = evaluateManyClause(resultsList, selectList);
			return result;
		}
	}
	/*
	1.check if clauseList is empty
	if empty, 
		getSelectSynonym(0)
		populate all synonym with possible values
		put them in set<string> and return
2.if not empty
	for all clauses in getClauseList
		clause.evaluate()
		all to resultsList
3.if resultsList.size == 1 // there is only 1 query
	if selectSynonym is used in resultObj // helper function to compare
		return getSingleResults

	else // if selectSynonym is not used in resultObj
		if (resultObj.isClauseTrue())
			return all possible values of selectSyn
		else
			return none
4.else // resultsList.size > 1
	if both clauses use the same syn AND a clause uses select syn
		reduce result set via elimination and return result
	if both clauses use the same syn AND no clause uses select syn
		check if both clauses are valid
			if both are valid, return all possible select syn
			if 1 clause is invalid, return none
	if clauses do not use the same syn, 
		check if both are valid
			if both are valid, return all possible select syn
			if 1 clause is invalid, return none
	*/
}