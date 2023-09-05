#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "tokenizer.h"
#include "parser.h"
#include "generation.cpp"


int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cerr << "40pctPP takes one argument: file name. Example..." << std::endl;
		std::cerr << "40pctPP <fileName.40pctPP>" << std::endl;
		return EXIT_FAILURE;
	}
	
	std::string sourceFile = argv[1];
	std::string sourceFileName;
	{
		size_t lastIDX = sourceFile.find_last_of("/");
		sourceFileName = sourceFile.substr(lastIDX+1, sourceFile.length());
	}
	 

	std::string fileContents;
	{
		std::stringstream contents_stream;
		std::fstream input(sourceFile, std::ios::in);
		contents_stream << input.rdbuf();
		fileContents = contents_stream.str();
	}

	Tokenizer tokenizer(fileContents, sourceFileName);
	std::vector <Tokens> tokens = tokenizer.Tokenize();

	// remove before prod
	/*for (int i = 0; i < tokens.size(); i++)
	{
		std::cout << str_types[tokens[i].type] << std::endl;
	}*/

	Parser parser(tokens, sourceFileName);
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
		size_t lastIDX = sourceFileName.find_last_of(".");
		outputFileName = sourceFileName.substr(0, lastIDX);
	}
	

	std::string command = compiler +  " -o " + outputFileName + " " + CfileName;
	
	const char* cmd = command.c_str();
	
	system(cmd);

	return EXIT_SUCCESS;
}