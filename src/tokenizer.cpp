#include <iostream>
#include <vector>
#include <string>
#include <optional>


#include "tokenizer.h"
#include "errors.h"

Tokenizer::Tokenizer(std::string contents)
{
	m_src = contents;
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
				tokens.push_back({ .type = TokenType::RETURN, .position = m_position });
				buffer.clear(); 
				continue;
			}

			if (buffer == "INT")
			{
				tokens.push_back({ .type = TokenType::INTEGER_DEF, .position = m_position });
				buffer.clear();
				continue;
			}

			if (buffer == "PLEASE_STOP")
			{
				tokens.push_back({ .type = TokenType::EXIT, .position = m_position });
				buffer.clear();
				continue;
			}

			tokens.push_back({ .type = TokenType::IDENT, .position = m_position, .value = buffer });
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

			tokens.push_back({ .type = TokenType::INT_LITERAL, .position = m_position, .value = buffer });
			buffer.clear();
			continue;
		}
		else if (Peek().value() == '=')
		{
			tokens.push_back({ .type = TokenType::EQUALS, .position = m_position });
			Consume();
			continue;
		}
		else if (Peek().value() == '(')
		{
			tokens.push_back({ .type = TokenType::OPEN_PAREN, .position = m_position });
			Consume();
			continue;
		}
		else if (Peek().value() == ')')
		{
			tokens.push_back({ .type = TokenType::CLOSE_PAREN, .position = m_position });
			Consume();
			continue;
		}
		else if (Peek().value() == ';')
		{
			tokens.push_back({ .type = TokenType::SEMICOLON, .position = m_position });
			Consume();
			continue;
		}
		else if (Peek().value() == '+')
		{
			tokens.push_back({ .type = TokenType::ADDITION, .position = m_position });
			Consume();
			continue;
		}
		else if (Peek().value() == '-')
		{
			tokens.push_back({ .type = TokenType::SUBTRACTION, .position = m_position });
			Consume();
			continue;
		}
		else if (Peek().value() == '*')
		{
			tokens.push_back({ .type = TokenType::MULTIPLICATION, .position = m_position });
			Consume();
			continue;
		}
		else if (Peek().value() == '/')
		{
			tokens.push_back({ .type = TokenType::DIVISION, .position = m_position });
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
			m_position.line++;
			continue;
		}	
		else
		{
			E0109 error(m_position);
			error.Raise();
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
	m_position.column++;
	return m_src.at(m_index++);
}