#pragma once

#ifndef PARSER_H
#define PARSER_H

#include "Scanner/Scanner.h"
#include "Parser/Node.h"

enum Status {no_error, syntax_error, operator_error, type_error};

class Parser
{
public:
	Parser(Scanner& scanner);
	Node* parse();
	virtual ~Parser() { };
private:
	Node* prog();
	Node* stmnts();
	Node* stmnts_tail();
	Node* stmnt();
	Node* expr();
	Node* opnd();
	Type op();
	Type type();
	void var_ident();
	bool match(Symbol expected, std::string tag);
	void handleError();
	void next();
	bool is_operator();

	Node* createParentNode(Type type, Node* left, Node* right);
	Node* createNode(Type type, std::string value);

	Scanner& m_scanner;
	Status m_status;
	Token m_current_token;
	int m_current_line;
	int m_current_index;
};

#endif