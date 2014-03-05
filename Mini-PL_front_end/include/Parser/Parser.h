#pragma once

#ifndef PARSER_H
#define PARSER_H

#include "Scanner/Scanner.h"

enum Status {no_error, syntax_error, operator_error, type_error};

class Parser
{
public:
	Parser(Scanner& scanner);
	bool parse();
	virtual ~Parser() { };
private:
	void prog();
	void stmnts();
	void stmnt();
	void expr();
	void opnd();
	void op();
	void type();
	void var_ident();
	bool match(Symbol expected, std::string tag);
	void handleError();
	void next();
	bool is_operator();

	Scanner& m_scanner;
	Status m_status;
	Token m_current_token;
	int m_current_line;
	int m_current_index;
};

#endif