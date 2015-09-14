#include <cppunit/config/SourcePrefix.h>
#include "CallsStartClauseTest.h"
#include "../SPA/ProcTable.h"
#include "../SPA/Utils.h"
#include "../SPA/CallsStarClause.h"

using namespace stringconst;
using namespace std;

void CallsStarClauseTest::setUp() {
	/*
	procedure main {
		call childProc1;	// 1
		call childProc2;	// 2
		call childProc3;	// 3
	}

	procedure childProc1 {
		call private1;		// 1
		call private2;		// 2
	}

	procedure childProc2 {
		call private1;		// 1
		call private3;		// 2
	}

	procedure childProc3 {
		call private4;		// 1
	}

	procedure private1 {
		call private2;		// 1
	}

	procedure private2 {
		v = 1;				// 1
	}

	procedure private4 {
		s = 1;				// 1
	}
	*/

	// to set up the procTable manually
	ProcTable* procTable = ProcTable::getInstance();

	Procedure* proc1 = new Procedure("main");

	Procedure* proc2 = new Procedure("childProc1");
	Procedure* proc3 = new Procedure("childProc2");
	Procedure* proc4 = new Procedure("childProc3");

	Procedure* proc5 = new Procedure("private1");
	Procedure* proc6 = new Procedure("private2");
	Procedure* proc7 = new Procedure("private3");
	Procedure* proc8 = new Procedure("private4");

	// setup calls for procedure main
	unordered_set<Procedure*> proc1CallSet;
	proc1CallSet.insert(proc2);
	proc1CallSet.insert(proc3);
	proc1CallSet.insert(proc4);
	proc1->setCalls(proc1CallSet);
	procTable->addProc(proc1);

	// setup calls and calledby for procedure childProc1
	unordered_set<Procedure*> proc2CallSet;
	proc2CallSet.insert(proc5);
	proc2CallSet.insert(proc6);
	proc2->setCalls(proc2CallSet);
	unordered_set<Procedure*> proc2CalledBySet;
	proc2CalledBySet.insert(proc1);
	proc2->setCalledBy(proc2CalledBySet);
	procTable->addProc(proc2);

	// setup calls and calledby for procedure childProc2
	unordered_set<Procedure*> proc3CallSet;
	proc3CallSet.insert(proc5);
	proc3CallSet.insert(proc7);
	proc3->setCalls(proc3CallSet);
	unordered_set<Procedure*> proc3CalledBySet;
	proc3CalledBySet.insert(proc1);
	proc3->setCalledBy(proc3CalledBySet);
	procTable->addProc(proc3);

	// setup calls and calledby for procedure childProc3
	unordered_set<Procedure*> proc4CallSet;
	proc4CallSet.insert(proc8);
	proc4->setCalls(proc4CallSet);
	unordered_set<Procedure*> proc4CalledBySet;
	proc4CalledBySet.insert(proc1);
	proc4->setCalledBy(proc4CalledBySet);
	procTable->addProc(proc4);

	// setup calls and calledby for procedure private1
	unordered_set<Procedure*> proc5CallSet;
	proc5CallSet.insert(proc6);
	proc5->setCalls(proc5CallSet);
	unordered_set<Procedure*> proc5CalledBySet;
	proc5CalledBySet.insert(proc2);
	proc5CalledBySet.insert(proc3);
	proc5->setCalledBy(proc5CalledBySet);
	procTable->addProc(proc5);

	// setup calls and calledby for procedure private2
	unordered_set<Procedure*> proc6CalledBySet;
	proc6CalledBySet.insert(proc2);
	proc6CalledBySet.insert(proc5);
	proc6->setCalledBy(proc6CalledBySet);
	procTable->addProc(proc6);

	// setup calls and calledby for procedure private3
	unordered_set<Procedure*> proc7CalledBySet;
	proc7CalledBySet.insert(proc3);
	proc7->setCalledBy(proc7CalledBySet);
	procTable->addProc(proc7);

	// setup calls and calledby for procedure private4
	unordered_set<Procedure*> proc8CalledBySet;
	proc8CalledBySet.insert(proc4);
	proc8->setCalledBy(proc8CalledBySet);
	procTable->addProc(proc8);
}

void CallsStarClauseTest::tearDown() {
	ProcTable::getInstance()->clearTable();
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( CallsStarClauseTest );

void CallsStarClauseTest::testCallsStarFixedFixedPass() {
	Results* result = new Results();
	CallsStarClause* c1 = new CallsStarClause();
	c1->setFirstArg("main");
	c1->setFirstArgFixed(true);
	c1->setFirstArgType(ARG_PROCEDURE);
	c1->setSecondArg("private2");
	c1->setSecondArgFixed(true);
	c1->setSecondArgType(ARG_PROCEDURE);
	CPPUNIT_ASSERT(c1->isValid());

	bool evalResult = c1->evaluate(result);
	CPPUNIT_ASSERT(evalResult);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 0);
}

void CallsStarClauseTest::testCallsStarFixedFixedFail() {
	Results* result = new Results();
	CallsStarClause* c1 = new CallsStarClause();
	c1->setFirstArg("private1");
	c1->setFirstArgFixed(true);
	c1->setFirstArgType(ARG_PROCEDURE);
	c1->setSecondArg("main");
	c1->setSecondArgFixed(true);
	c1->setSecondArgType(ARG_PROCEDURE);
	CPPUNIT_ASSERT(c1->isValid());

	bool evalResult = c1->evaluate(result);
	CPPUNIT_ASSERT(!evalResult);
	CPPUNIT_ASSERT(result->getResultsTableSize() == 0);
}

void CallsStarClauseTest::testCallsStarSynFixedPass() {
	Results* res = new Results();
	CallsStarClause* c1 = new CallsStarClause();
	c1->setFirstArg("a");
	c1->setFirstArgFixed(false);
	c1->setFirstArgType(ARG_PROCEDURE);
	c1->setSecondArg("private2");
	c1->setSecondArgFixed(true);
	c1->setSecondArgType(ARG_PROCEDURE);
	CPPUNIT_ASSERT(c1->isValid());

	bool result = c1->evaluate(res);
	CPPUNIT_ASSERT(result);
	CPPUNIT_ASSERT(res->hasResults("a"));
	CPPUNIT_ASSERT(res->getResultsTableSize() == 4);
}

void CallsStarClauseTest::testCallsStarSynFixedFail() {
	Results* res = new Results();
	CallsStarClause* c1 = new CallsStarClause();
	c1->setFirstArg("a");
	c1->setFirstArgFixed(false);
	c1->setFirstArgType(ARG_PROCEDURE);
	c1->setSecondArg("main");
	c1->setSecondArgFixed(true);
	c1->setSecondArgType(ARG_PROCEDURE);
	CPPUNIT_ASSERT(c1->isValid());

	bool result = c1->evaluate(res);
	CPPUNIT_ASSERT(result == false);
	CPPUNIT_ASSERT(res->getResultsTableSize() == 0);
	CPPUNIT_ASSERT(res->hasResults("a") == false);
}

void CallsStarClauseTest::testCallsStarFixedSynPass() {
	Results* res = new Results();
	CallsStarClause* c1 = new CallsStarClause();
	c1->setFirstArg("childProc2");
	c1->setFirstArgFixed(true);
	c1->setFirstArgType(ARG_PROCEDURE);
	c1->setSecondArg("b");
	c1->setSecondArgFixed(false);
	c1->setSecondArgType(ARG_PROCEDURE);
	CPPUNIT_ASSERT(c1->isValid());

	bool result = c1->evaluate(res);
	CPPUNIT_ASSERT(result == true);
	CPPUNIT_ASSERT(res->hasResults("b") == true);
	CPPUNIT_ASSERT(res->getResultsTableSize() == 3);
}

void CallsStarClauseTest::testCallsStarFixedSynFail() {
	Results* res = new Results();
	CallsStarClause* c1 = new CallsStarClause();
	c1->setFirstArg("private2");
	c1->setFirstArgFixed(true);
	c1->setFirstArgType(ARG_PROCEDURE);
	c1->setSecondArg("b");
	c1->setSecondArgFixed(false);
	c1->setSecondArgType(ARG_PROCEDURE);
	CPPUNIT_ASSERT(c1->isValid());

	bool result = c1->evaluate(res);
	CPPUNIT_ASSERT(result == false);
	CPPUNIT_ASSERT(res->getResultsTableSize() == 0);
	CPPUNIT_ASSERT(res->hasResults("b") == false);
}

void CallsStarClauseTest::testCallsStarFirstUnderscorePass() {
	Results* res = new Results();
	CallsStarClause* c1 = new CallsStarClause();
	c1->setFirstArg("_");
	c1->setFirstArgFixed(false);
	c1->setFirstArgType(ARG_GENERIC);
	c1->setSecondArg("p");
	c1->setSecondArgFixed(false);
	c1->setSecondArgType(ARG_PROCEDURE);
	CPPUNIT_ASSERT(c1->isValid());

	bool result = c1->evaluate(res);
	CPPUNIT_ASSERT(result == true);
	CPPUNIT_ASSERT(res->getResultsTableSize() == 7);
	CPPUNIT_ASSERT(res->hasResults("p") == true);
}

void CallsStarClauseTest::testCallsStarSecondUnderscorePass() {
	Results* res = new Results();
	CallsStarClause* c1 = new CallsStarClause();
	c1->setFirstArg("p");
	c1->setFirstArgFixed(false);
	c1->setFirstArgType(ARG_PROCEDURE);
	c1->setSecondArg("_");
	c1->setSecondArgFixed(false);
	c1->setSecondArgType(ARG_GENERIC);
	CPPUNIT_ASSERT(c1->isValid());

	bool result = c1->evaluate(res);
	CPPUNIT_ASSERT(result == true);
	CPPUNIT_ASSERT(res->getResultsTableSize() == 5);
	CPPUNIT_ASSERT(res->hasResults("p") == true);
}

void CallsStarClauseTest::testCallsStarBothUnderscorePass() {
	Results* res = new Results();
	CallsStarClause* c1 = new CallsStarClause();
	c1->setFirstArg("_");
	c1->setFirstArgFixed(false);
	c1->setFirstArgType(ARG_GENERIC);
	c1->setSecondArg("_");
	c1->setSecondArgFixed(false);
	c1->setSecondArgType(ARG_GENERIC);
	CPPUNIT_ASSERT(c1->isValid());

	bool result = c1->evaluate(res);
	CPPUNIT_ASSERT(result == true);
}

void CallsStarClauseTest::testCallsStarSynSynPass() {
	Results* res = new Results();
	CallsStarClause* c1 = new CallsStarClause();
	c1->setFirstArg("p");
	c1->setFirstArgFixed(false);
	c1->setFirstArgType(ARG_PROCEDURE);
	c1->setSecondArg("q");
	c1->setSecondArgFixed(false);
	c1->setSecondArgType(ARG_PROCEDURE);
	CPPUNIT_ASSERT(c1->isValid());

	bool result = c1->evaluate(res);
	CPPUNIT_ASSERT(result == true);
	CPPUNIT_ASSERT(res->hasResults("p") == true);
	CPPUNIT_ASSERT(res->hasResults("q") == true);
	CPPUNIT_ASSERT(res->getResultsTableSize() == 14);
}

void CallsStarClauseTest::testCallsStarSynSynSame() {
	Results* res = new Results();
	CallsStarClause* c1 = new CallsStarClause();
	c1->setFirstArg("p");
	c1->setFirstArgFixed(false);
	c1->setFirstArgType(ARG_PROCEDURE);
	c1->setSecondArg("p");
	c1->setSecondArgFixed(false);
	c1->setSecondArgType(ARG_PROCEDURE);
	CPPUNIT_ASSERT(c1->isValid());

	bool result = c1->evaluate(res);
	CPPUNIT_ASSERT(result == false);
	CPPUNIT_ASSERT(res->hasResults("p") == false);
	CPPUNIT_ASSERT(res->getResultsTableSize() == 0);
}