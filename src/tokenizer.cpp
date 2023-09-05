#include <iostream>
#include <vector>
#include <string>
#include <optional>


#include "tokenizer.h"
#include "errors.h"

Tokenizer::Tokenizer(std::string contents, std::string fileName)
{
	m_src = contents;
	m_fileName = fileName;
}

std::vector <Tokens> Tokenizer::Tokenize()
{
	std::vector <Tokens> tokens;
	std::string buffer;

	while (Peek().has_value())
	{
		if (std::isalpha(Peek().value()) || Peek().value() == '_')
		{
			buffer.push_back(Consume());
			// set the buffer equal to the alphanumeric characters until something else
			while (Peek().has_value() && (std::isalnum(Peek().value()) || Peek().value() == '_')) {
				buffer.push_back(Consume());
			}
			// evaluate the buffers
			if (buffer == "return")
			{
				// need to better understand this
				tokens.push_back({ .type = TokenType::RETURN, .coord = { lineNumber, columnNumber } });
				buffer.clear(); 
				continue;
			}

			if (buffer == "INT")
			{
				tokens.push_back({ .type = TokenType::INTEGER_DEF, .coord = { lineNumber, columnNumber } });
				buffer.clear();
				continue;
			}

			if (buffer == "PLEASE_STOP")
			{
				tokens.push_back({ .type = TokenType::EXIT, .coord = { lineNumber, columnNumber } });
				buffer.clear();
				continue;
			}

			tokens.push_back({ .type = TokenType::IDENT, .coord = { lineNumber, columnNumber }, .value = buffer });
			buffer.clear();
			continue;
		}
		else if (std::isdigit(Peek().value()))
		{
			buffer.push_back(Consume());
			while (Peek().has_value() && std::isdigit(Peek().value()))
			{
				buffer.push_back(Consume());
			}

			tokens.push_back({ .type = TokenType::INT_LITERAL, .coord = { lineNumber, columnNumber }, .value = buffer });
			buffer.clear();
			continue;
		}
		else if (Peek().value() == '=')
		{
			tokens.push_back({ .type = TokenType::EQUALS, .coord = { lineNumber, columnNumber } });
			Consume();
			continue;
		}
		else if (Peek().value() == '(')
		{
			tokens.push_back({ .type = TokenType::OPEN_PAREN, .coord = { lineNumber, columnNumber } });
			Consume();
			continue;
		}
		else if (Peek().value() == ')')
		{
			tokens.push_back({ .type = TokenType::CLOSE_PAREN, .coord = { lineNumber, columnNumber } });
			Consume();
			continue;
		}
		else if (Peek().value() == ';')
		{
			tokens.push_back({ .type = TokenType::SEMICOLON, .coord = { lineNumber, columnNumber } });
			Consume();
			continue;
		}
		else if (Peek().value() == '+')
		{
			tokens.push_back({ .type = TokenType::ADDITION, .coord = { lineNumber, columnNumber } });
			Consume();
			continue;
		}
		else if (Peek().value() == '-')
		{
			tokens.push_back({ .type = TokenType::SUBTRACTION, .coord = { lineNumber, columnNumber } });
			Consume();
			continue;
		}
		else if (Peek().value() == '*')
		{
			tokens.push_back({ .type = TokenType::MULTIPLICATION, .coord = { lineNumber, columnNumber } });
			Consume();
			continue;
		}
		else if (Peek().value() == '/')
		{
			tokens.push_back({ .type = TokenType::DIVISION, .coord = { lineNumber, columnNumber } });
			Consume();
			continue;
		}
		else if (std::isspace(Peek().value()))
		{
			Consume();
			continue;
		}
		else if (Peek().value() == '\n')
		{
			Consume();
			lineNumber++;
			continue;
		}	
		else
		{
			Error error{
				.errorCode = Errors::TokenErrorCode::E0001, .errorMessage = "Unrecognized Character",
				.coord = {lineNumber, columnNumber}, .fileName = m_fileName, .value = Peek().value()
			};

			error.raise();
		}
	}

	return tokens;
}

// grabs current character 
[[nodiscard]] std::optional <char> Tokenizer::Peek(int offset) const
{
	if (m_index >= m_src.length())
	{
		return {};
	}
	return m_src.at(m_index);
}

// gets character at current index and increments (using post increment is not ideal due to readability but it simplifies the code and performance)
char Tokenizer::Consume()
{
	columnNumber++;
	return m_src.at(m_index++);
}