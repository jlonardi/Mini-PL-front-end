#include "Scanner/ReadBuffer.h"
#include "Scanner/Scanner.h"
#include "Scanner/MiniRules.h"
#include "Parser/Parser.h"
#include "Interpreter/Interpreter.h"
#include <iostream>

int main(int argc, char* argv[])
{
	if(argc > 1)
	{
		std::string path = argv[1];
		
		ReadBuffer buffer(path);
		MiniRules rules;
		Scanner scanner(buffer, rules);
		Parser parser(scanner);
		Node* AST = parser.parse();
		std::cout << std::endl << std::endl;
		Interpreter interpreter;
		interpreter.interprete(AST);
		/*
		scanner.scan();

		while(scanner.tokensLeft())
		{
			Token tkn = scanner.nextToken();
			std::cout << "<" << tkn.tag << " | " << tkn.lexeme << "> " << std::endl;
			//std::cout << tkn.lexeme << std::endl;
		}
		*/
	} else {
		std::cout << "No source file given. Please give a source file as the first argument" << std::endl;
	}

	std::cout << "Press a key to continue...";
	std::cin.get();
	return 0;
}