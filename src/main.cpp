#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "tokenizer.h"
#include "parser.h"
#include "generation.cpp"


int main(int argc, char* argv[]) {
	// remove before production
	const std::string str_types[] = {"RETURN", "INTEGER_DEF", "open_paren", "close_paren", "int_literal", "semicolon" , "ident", "equals", "ADDITION"};

	if (argc != 2) {
		std::cerr << "40pctPP takes one argument: file name. Example..." << std::endl;
		std::cerr << "40pctPP <fileName.40pctPP>" << std::endl;
		return EXIT_FAILURE;
	}

	std::string fileContents;
	{
		std::stringstream contents_stream;
		std::fstream input(argv[1], std::ios::in);
		contents_stream << input.rdbuf();
		fileContents = contents_stream.str();
	}

	Tokenizer tokenizer(fileContents);
	std::vector <Tokens> tokens = tokenizer.Tokenize();

	// remove before prod
	for (int i = 0; i < tokens.size(); i++)
	{
		std::cout << str_types[tokens[i].type] << std::endl;
	}

	Parser parser(tokens);
	std::optional<NodeProg> prog = parser.ParseProgram();

	if (!prog.has_value())
	{
		std::cerr << "Invalid program, no instructions detected" << std::endl;
		exit(EXIT_FAILURE);
	}


	Generator generator(prog.value());
	{
		std::fstream file("out.c", std::ios::out);
		file << generator.GenProg();
	}

	return 0;

	//! create file name that wont cause overriding
	//! give user option to choose output name
	std::string compiler = "gcc";
	std::string CfileName = "out.c";

	// create outputFileName
	std::string outputFileName;
	{
		std::string sourceFile = argv[1];
		size_t lastIDX = sourceFile.find_last_of(".");
		outputFileName = sourceFile.substr(0, lastIDX);
	}
	

	std::string command = compiler +  " -o " + outputFileName + " " + CfileName;
	
	const char* cmd = command.c_str();
	
	system(cmd);

	return EXIT_SUCCESS;
}