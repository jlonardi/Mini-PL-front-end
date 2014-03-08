#include "Scanner/ReadBuffer.h"
#include "Scanner/Scanner.h"
#include "Scanner/MiniRules.h"
#include "Parser/Parser.h"
#include "Parser/SemanticAnalyzer.h"
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
		Statement* root = parser.parse();
		SemanticAnalyzer analyzer(root);
		bool errors = analyzer.analyze();
		if(errors)
		{
			std::cout << "There was errors during the semantic analysis, skipping interpretation part." << std::endl;
		} else {
			Interpreter interpreter(root);
			interpreter.interprete();
		}

		std::cout << std::endl << std::endl;
	} else {
		std::cout << "No source file given. Please give a source file as the first argument" << std::endl;
	}

	std::cout << "Press a key to continue...";
	std::cin.get();
	return 0;
}