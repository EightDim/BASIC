/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface specified in Chapter 17, which is an
 * excellent model for the Statement class hierarchy.
 */

#ifndef _statement_h
#define _statement_h

#include "evalstate.h"
#include "exp.h"
#include "parser.h"
#include "../StanfordCPPLib/tokenscanner.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/strlib.h"

/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */

class Statement {

public:

/*
 * Constructor: Statement
 * ----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 */

   Statement();

/*
 * Destructor: ~Statement
 * Usage: delete stmt;
 * -------------------
 * The destructor deallocates the storage for this expression.
 * It must be declared virtual to ensure that the correct subclass
 * destructor is called when deleting a statement.
 */

   virtual ~Statement();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a BASIC statement.  Each of the subclasses
 * defines its own execute method that implements the necessary
 * operations.  As was true for the expression evaluator, this
 * method takes an EvalState object for looking up variables or
 * controlling the operation of the interpreter.
 */

   virtual void execute(EvalState & state) = 0;

 /*
	* Method: construct
	* Usage: stmt->construct(scanner);
	* ----------------------------
	* This method constructs a BASIC statement by tokenscanner.
*/
   virtual void construct(TokenScanner & scanner) = 0;
};


//class REMstmt : public Statement {
//private:
//
//
//public:
//
//	REMstmt() = default;
//	virtual ~REMstmt() = default;
//	virtual void execute(EvalState & state);
//	virtual void parse(TokenScanner & scanner);
//
//};

class LETstmt : public Statement {
private:
	string varName;
	Expression *exp;

public:

	LETstmt() = default;
	virtual ~LETstmt() {
		delete exp;
	}
	virtual void execute(EvalState & state);
	virtual void construct(TokenScanner & scanner);

};

class PRINTstmt : public Statement {
private:
	Expression *exp;

public:

	PRINTstmt() = default;
	virtual ~PRINTstmt() {
		delete exp;
	}
	virtual void execute(EvalState & state);
	virtual void construct(TokenScanner & scanner);

};

class INPUTstmt : public Statement {
private:
	string varName;


public:

	INPUTstmt() = default;
	virtual ~INPUTstmt() = default;
	virtual void execute(EvalState & state);
	virtual void construct(TokenScanner & scanner);

};

class ENDstmt : public Statement {
private:


public:

	ENDstmt() = default;
	virtual ~ENDstmt() = default;
	virtual void execute(EvalState & state);
	virtual void construct(TokenScanner & scanner);

};

class GOTOstmt : public Statement {
private:
	int lineNumber;

public:

	GOTOstmt() = default;
	virtual ~GOTOstmt() = default;
	virtual void execute(EvalState & state);
	virtual void construct(TokenScanner & scanner);

};

class IFstmt : public Statement {
private:
	Expression *lhs, *rhs;
	string cmp;
	int lineNumber;

public:

	IFstmt() = default;
	virtual ~IFstmt() {
		delete lhs;
		delete rhs;
	}
	virtual void execute(EvalState & state);
	virtual void construct(TokenScanner & scanner);

};

Statement *getStatement(TokenScanner & scanner);

#endif
