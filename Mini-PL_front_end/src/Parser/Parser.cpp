#include "Parser/Parser.h"
#include "Scanner/miniRules.h"
#include <iostream>

Parser::Parser(Scanner& scanner) : m_scanner(scanner) { };

bool Parser::parse()
{
	m_scanner.scan();
	while(m_scanner.tokensLeft())
	{
		Token tkn = m_scanner.nextToken();
		if(tkn.symbol != Symbol::whitespace && tkn.symbol != Symbol::comment)
			std::cout << "<" << tkn.lexeme << "> " << std::endl;
	};

	return true;
};