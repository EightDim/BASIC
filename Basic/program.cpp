/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include "program.h"
#include "statement.h"
#include <string>
#include <map>
#include <set>
#include "statement.h"
#include "evalstate.h"
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "../StanfordCPPLib/tokenscanner.h"
#include "../StanfordCPPLib/error.h"
using namespace std;

Program::Program() {
	// Replace this stub with your own code
}

Program::~Program() {
	// Replace this stub with your own code
}

void Program::clear() {
	// Replace this stub with your own code
	lineset.clear();
	return;
}

void Program::addSourceLine(int lineNumber, string line) {
	// Replace this stub with your own code
	TokenScanner scanner;
	scanner.ignoreWhitespace();
	scanner.scanNumbers();
	scanner.setInput(line);
	scanner.nextToken();

	sentence present = sentence(lineNumber, getStatement(scanner), line);
	set<sentence>::iterator it = lineset.find(present);

	if (it == lineset.end()) lineset.insert(present);
	else {
		lineset.erase(it);
		lineset.insert(present);
	}
	return;
}

void Program::removeSourceLine(int lineNumber) {
	// Replace this stub with your own code
	set<sentence>::iterator it = lineset.find(sentence(lineNumber));
	if (it != lineset.end()) lineset.erase(it);
	return;
}

//string Program::getSourceLine(int lineNumber) {
//	// Replace this stub with your own code\
//}
//
//void Program::setParsedStatement(int lineNumber, Statement *stmt) {
//   // Replace this stub with your own code
//
//}
//
//Statement *Program::getParsedStatement(int lineNumber) {
//   return NULL;  // Replace this stub with your own code
//}

//int Program::getFirstLineNumber() {
//	// Replace this stub with your own code
//	
//}
//
//int Program::getNextLineNumber(int lineNumber) {
//	// Replace this stub with your own code
//	
//}


void Program::run(EvalState &state) {
	state.setValue("GOTO", 0);
	state.setValue("END", 0);

	for (set<sentence>::iterator it = lineset.begin(); 
		(it != lineset.end() || state.getValue("GOTO") != 0) && state.getValue("END") == 0;) {
		
		
		if (state.getValue("GOTO") == 1) {
			state.setValue("GOTO", 0);
			int lineNum = state.getValue("RUN");
			set<sentence>::iterator itgt = lineset.find(sentence(lineNum));
			if (itgt == lineset.end()) {
				error("LINE NUMBER ERROR");
				}
			else it = itgt;
		}
		else {
			if (it->stmt != nullptr)
				it->stmt->execute(state);
			it++;
		}
		
	}
}

void Program::list() {
	for (set<sentence>::iterator it = lineset.begin(); it != lineset.end(); ++it)
		cout << it->line << endl;
	return;
}

void Program::help() {
	cout << "Help Help Kid! Support the Weak!" << endl;
}
