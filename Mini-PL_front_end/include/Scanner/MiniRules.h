#pragma once

#ifndef MINIRULES_H
#define MINIRULES_H

#include "Scanner/Rules.h"

/*
	The MiniRules class rapresents the lexical rules that
	recognizes the Mini-PL language.
*/

class MiniRules : public Rules
{
public:
	MiniRules();
	RuleSet getRules();
	int characterCategory(char c);
private:
	RuleSet m_rules;
};

#endif