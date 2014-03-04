#include "Parser/Parser.h"
#include <iostream>

Parser::Parser(Scanner& scanner) : m_scanner(scanner) { };

bool Parser::parse()
{
	m_scanner.scan();
	while(m_scanner.tokensLeft())
	{
		Token tkn = m_scanner.nextToken();
		std::cout << "<" << tkn.lexeme << "> " << std::endl;
		//std::cout << tkn.lexeme << std::endl;
	};

	return true;
};