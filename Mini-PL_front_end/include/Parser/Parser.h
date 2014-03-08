#pragma once

#ifndef PARSER_H
#define PARSER_H

#include "Scanner/Scanner.h"
#include "Parser/Nodes/Node.h"
#include "Parser/Nodes/BinaryOp.h"
#include "Parser/Nodes/VarDeclaration.h"
#include "Parser/Nodes/AssignStatement.h"

enum Status {no_error, syntax_error, operator_error, type_error};

class Parser
{
public:
	Parser(Scanner& scanner);
	Statement* parse();
	virtual ~Parser() { };
private:
	Statement* prog();
	Statement* stmnts();
	Statement* stmnts_tail();
	Statement* stmnt();
	Expression* expr();
	Expression* opnd();
	OperatorType op();
	ValueType varType();
	void var_ident();
	bool match(Symbol expected, std::string tag);
	void handleError();
	void next();
	bool is_operator();
	std::string fixLexeme(std::string lexeme);

	Scanner& m_scanner;
	Status m_status;
	Token m_current_token;
	int m_current_line;
	int m_current_index;
};

#endif