#pragma once

#ifndef PARSER_H
#define PARSER_H

#include "Scanner/Scanner.h"

enum Error {no_error, unexpected_symbol, syntax_error, invalid_operator};

class Parser
{
public:
	Parser(Scanner& scanner);
	bool parse();
private:
	void prog();
	void stmnts();
	void stmnt();
	void expr();
	void opnd();
	void type();
	void var_ident();
	bool expect(std::string symbol);
	bool accept(std::string symbol);

	Scanner& m_scanner;
	Error status;
};

#endif