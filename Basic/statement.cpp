/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include "statement.h"
#include "evalstate.h"
#include "exp.h"
#include "parser.h"
#include "../StanfordCPPLib/tokenscanner.h"
#include "../StanfordCPPLib/error.h"

using namespace std;

const string illegalvarName[13] = { "REM", "LET", "INPUT", "PRINT", "END", "IF", "GOTO", "RUN", "LIST", "HELP", "CLEAR", "QUIT" };
/* Implementation of the Statement class */

Statement::Statement() {
   /* Empty */
} 

Statement::~Statement() {
   /* Empty */
}

//void REMstmt::execute(EvalState & state) {
//
//}
//void REMstmt::parse(TokenScanner & scanner) {
//
//}

void LETstmt::execute(EvalState & state) {
	state.setValue(varName, exp->eval(state));
	return;
}

void LETstmt::construct(TokenScanner & scanner) {
	varName = scanner.nextToken();
	string opt = scanner.nextToken();
	TokenType type = scanner.getTokenType(varName);
	int op = 0;
	while (op != 13) {
		if (illegalvarName[op] == varName) break;
		++op;
	}

	if (type != WORD || op != 13 || opt != "=")
		error("SYNTAX ERROR");

	exp = parseExp(scanner);
}

void PRINTstmt::execute(EvalState &state) {
	cout << exp->eval(state) << endl;
}

void PRINTstmt::construct(TokenScanner & scanner) {
	try {
		exp = parseExp(scanner);
	}
	catch (...) {
		cout << "SYNTAX ERROR" << endl;
	}
}


void INPUTstmt::execute(EvalState &state) {
	while (true) {
		cout << " ? ";
		string str;
		getline(cin, str);
		try {
			int value = stringToInteger(str);
			state.setValue(varName, value);
			return;
		}
		catch (...) {
			cout << "INVALID NUMBER" << endl;
		}
	}
}

void INPUTstmt::construct(TokenScanner &scanner) {
	varName = scanner.nextToken();

	int op = 0;
	while (op != 13) {
		if (illegalvarName[op] == varName) break;
		++op;
	}
	if (op != 13) error("SYNTAX ERROR");

	if(scanner.getTokenType(varName) != WORD)
		error("SYNTAX ERROR");

	if (scanner.hasMoreTokens())
		error("SYNTAX ERROR");
}

void ENDstmt::execute(EvalState & state) {
	state.setValue("END", 1);
}
void ENDstmt::construct(TokenScanner & scanner) {
	if (scanner.hasMoreTokens()) error("SYNTAX ERROR");
}


void GOTOstmt::execute(EvalState &state) {
	state.setValue("RUN", lineNumber);
	state.setValue("GOTO", 1);
}

void GOTOstmt::construct(TokenScanner &scanner) {
	string num = scanner.nextToken();
	if (scanner.getTokenType(num) != NUMBER) error("SYNTAX ERROR");
	lineNumber = stringToInteger(num);
}



void IFstmt::execute(EvalState &state) {
	int l = lhs->eval(state), r = rhs->eval(state);
	if ((cmp == "=" && l == r) || (cmp == "<" && l < r) || (cmp == ">" && l > r)) {
		state.setValue("RUN", lineNumber);
		state.setValue("GOTO", 1);
	}
}

void IFstmt::construct(TokenScanner &scanner) {
	string left = "";
	while (true){
		string nxt = scanner.nextToken();
		if (nxt != "=" && nxt != "<" && nxt != ">") left += nxt;
		else {
			scanner.saveToken(nxt);
			break;
		}
		
	} 
	
	try {
		TokenScanner tmp;
		tmp.ignoreWhitespace();
		tmp.scanNumbers();
		tmp.setInput(left);
		lhs = parseExp(tmp);
	}
	catch (...) {
		error("SYNTAX ERROR");
	}

	cmp = scanner.nextToken();

	string right = "";
	while (true) {
		string nxt = scanner.nextToken();
		if (nxt != "THEN") right += nxt;
		else
			break;
		
	} 
	
	try {
		TokenScanner tmp;
		tmp.ignoreWhitespace();
		tmp.scanNumbers();
		tmp.setInput(right);
		rhs = parseExp(tmp);
	}
	catch (...) {
		error("SYNTAX ERROR");
	}

	string numOfLine = scanner.nextToken();
	if (scanner.getTokenType(numOfLine) == NUMBER)
		lineNumber = stringToInteger(numOfLine);
	else error("SYNTAX ERROR");
}

Statement *getStatement(TokenScanner &scanner) {
	
	string token = scanner.nextToken();
	Statement *stmt = nullptr;
	try {
		if (token == "REM") stmt = nullptr;
		else if (token == "LET") stmt = new LETstmt;
		else if (token == "PRINT") stmt = new PRINTstmt;
		else if (token == "INPUT") stmt = new INPUTstmt;
		else if (token == "END") stmt = new ENDstmt;
		else if (token == "GOTO") stmt = new GOTOstmt;
		else if (token == "IF") stmt = new IFstmt;
		else error("SYNTAX ERROR");

	}
	catch (ErrorException & ex) {
		cerr << ex.getMessage() << endl;
	}
	if (stmt != nullptr) 
		stmt->construct(scanner);

	return stmt;
}
