#pragma once

#ifndef SCANNER_H
#define SCANNER_H

#include "Scanner/Buffer.h"
#include "Scanner/Rules.h"
#include <string>
#include <list>

class Scanner
{
public:
	Scanner(Buffer& buffer, Rules& rules);
	void scan();
	Token nextToken();
	bool tokensLeft();
private:
	void processNextChar();
	std::string lexeme;
	int state;
	char c;
	RuleSet ruleSet;
	Buffer& m_buffer;
	Rules& m_rules;
	std::list<Token> m_tokens;
};

#endif