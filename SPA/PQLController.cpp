#include "PQLController.h"
#include "QueryParser.h"
#include "QueryEvaluator.h"
#include "Result.h"
#include <string>
#include <iostream>

PQLController::PQLController(void) {

}


PQLController::~PQLController(void) {
}

unordered_set<string> PQLController::parse(string query) {
	// Parse query to query preprocessor (try catch?)
	// if query is valid, evaluate query with query evaluator
	// else return none
	// call query evaluator to format query
	// display query result
	try {
		//cout << "Parsing Query..."; 
		QueryParser* parser = QueryParser::getInstance();
		Query* q= parser->parseQuery(query);
		//cout << "Query Parse Successful" << endl;

		/////////////////////////////////////////////////
		//CREATE DUMMY VALUE TO SIMULATE OPTIMISATION  //
		/////////////////////////////////////////////////
		int numClauses = q->getClauseList().size();
		vector<int>* optimisedDV = new vector<int>();
		optimisedDV->push_back(numClauses);
		/////////////////////////////////////////////////
		//DELETE AFTER USE							   //
		/////////////////////////////////////////////////

		QueryEvaluator* qe = new QueryEvaluator();
		Result* resObj = qe->evalOptimisedQuery(q, optimisedDV);
		//cout << "Query Evaluated" << endl;
		vector<StringPair> selectList = q->getSelectList();
		//cout << resObj->getResultTableSize() << endl;
		boost::unordered_set<string> valueSet = qe->printValues(resObj, selectList);
		//cout << "Result Set Size: " << valueSet.size() << endl;
		//delete q;
		//delete resObj;
		//delete qe;

		return valueSet;

	} catch (std::exception& e) {
		cout << "Evalutaion Failed...";
		cout << e.what() << endl;
		unordered_set<string> empty;
		return empty;
	}
}
