#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>
#include <optional>

#include "tokens.h"
#include "position.h"

class Tokenizer
{
public:
	Tokenizer(std::string contents);
	std::vector<Tokens> Tokenize();

private:
	std::string m_src;
	size_t m_index = 0;
	std::optional <char> Peek(int offset = 0) const;
	Position m_position{ 1,1 };
	char Consume();
	std::string m_fileName;
};

#endif