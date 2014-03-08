#pragma once

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Interpreter/Visitor.h"
#include "Interpreter/Evaluator.h"
#include "Parser/TypeChecker.h"
#include "Parser/SemanticAnalyzer.h"
#include <vector>

class Statement;

class Interpreter : public Visitor, public Evaluator, public TypeChecker
{
public:
	Interpreter(Statement* AST) : root(AST) { };
	void interprete();
private:
	virtual void visit(VarDeclaration& node) ;
	virtual void visit(AssignStatement& node);
	virtual void visit(ForStatement& node);
	virtual void visit(ReadStatement& node);
	virtual void visit(PrintStatement& node);
	virtual void visit(AssertStatement& node);
	virtual ValueType typeCheck(const IntegerConst& node);
	virtual ValueType typeCheck(const StringLiteral& node);
	virtual ValueType typeCheck(const Variable& node);
	virtual ValueType typeCheck(BinaryOp& node);
	virtual ValueType typeCheck(UnaryOp& node);
	virtual std::string evaluate(const IntegerConst& node);
	virtual std::string evaluate(const StringLiteral& node);
	virtual std::string evaluate(const Variable& node);
	virtual std::string evaluate(BinaryOp& node);
	virtual std::string evaluate(UnaryOp& node);
	std::string typeToString(ValueType type);
	std::string intToString(int i);
	Statement* root;
	std::vector<Entry> symbolTable;
};

#endif