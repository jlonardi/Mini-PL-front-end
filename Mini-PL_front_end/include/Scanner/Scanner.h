#pragma once

#ifndef SCANNER_H
#define SCANNER_H

#include "Scanner/Buffer.h"
#include "Scanner/Rules.h"
#include <string>
#include <list>

struct Token
{
	std::string tag;
	std::string lexeme;
};

class Scanner
{
public:
	Scanner(Buffer& buffer, Rules& rules);
	void scan();
	Token nextToken();
	bool tokensLeft();
private:
	Buffer& m_buffer;
	Rules& m_rules;
	std::list<Token> m_tokens;
};

#endif