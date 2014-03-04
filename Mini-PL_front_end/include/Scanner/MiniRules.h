#pragma once

#ifndef MINIRULES_H
#define MINIRULES_H

#include "Scanner/Rules.h"

/*
	The MiniRules class rapresents the lexical rules that
	recognizes the Mini-PL language.
*/

enum Symbol {	error, identifier, integer, op_div, whitespace, op_mult, 
				op_plus, op_minus, op_less, colon, op_equal, op_and, 
				op_not, semicolon, lparen, rparen, string, op_assign, 
				range, comment, var, for_stmt, end, in, do_stmt, read, 
				print, boolean, assert };

class MiniRules : public Rules
{
public:
	MiniRules();
	RuleSet getRules();
	int characterCategory(char c);
	Symbol generateSymbol(std::string tag);
private:
	RuleSet m_rules;
};

#endif