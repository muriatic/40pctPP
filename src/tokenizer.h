#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>
#include <optional>

#include "tokens.h"

class Tokenizer
{
public:
	Tokenizer(std::string contents, std::string fileName);
	std::vector<Tokens> Tokenize();

private:
	std::string m_src;
	size_t m_index = 0;
	std::optional <char> Peek(int offset = 0) const;
	int columnNumber = 1;
	int lineNumber = 1;
	char Consume();
	std::string m_fileName;
};

#endif