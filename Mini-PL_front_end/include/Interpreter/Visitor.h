#pragma once

#ifndef VISITOR_H
#define VISITOR_H

class VarDeclaration;
class AssignStatement;
class ForStatement;
class ReadStatement;
class PrintStatement;
class AssertStatement;

class Visitor
{
public:
	virtual void visit(VarDeclaration& node) = 0;
	virtual void visit(AssignStatement& node) = 0;
	virtual void visit(ForStatement& node) = 0;
	virtual void visit(ReadStatement& node) = 0;
	virtual void visit(PrintStatement& node) = 0;
	virtual void visit(AssertStatement& node) = 0;
	virtual ~Visitor() { };
};

#endif