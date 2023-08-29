#include <string>
#include <iostream>
#include <optional>

#include "parser.h"
#include "tokens.h"


Parser::Parser(std::vector <Tokens> tokens)
{
	m_tokens = tokens;
}

std::optional<NodeExpr> Parser::ParseExpr()
{
	if (Peek().has_value() && Peek().value().type == TokenType::INT_LITERAL)
	{
		return NodeExpr { .var = NodeExprIntLit {.int_lit = Consume() } };
	}
	else if (Peek().has_value() && Peek().value().type == TokenType::IDENT)
	{
		return NodeExpr{ .var = NodeExprIdent { .ident = Consume() } };
	}
	return {};
}

std::optional<NodeStmt> Parser::ParseStmt()
{
	// its an exit statement
	if (Peek().value().type == TokenType::RETURN)
	{
		// consume exit token
		Consume();
		// check if either the next token doesn't exist or isnt an OPEN_PAREN
		if (!Peek().has_value() || Peek().value().type != TokenType::OPEN_PAREN)
		{
			std::cerr << "Invalid exit statement, expected '(' after exit." << std::endl;
			exit(EXIT_FAILURE);
		}

		Consume();

		NodeStmtReturn stmt_return;

		if (auto node_expr = ParseExpr())
		{
			stmt_return = { .expr = node_expr.value() };
		}
		else {
			std::cerr << "Invalid Expression, expected integer literal or variable for exit code." << std::endl;
			exit(EXIT_FAILURE);
		}

		// check if either the next token doesn't exist or isnt a CLOSE_PAREN
		if (!Peek().has_value() || Peek().value().type != TokenType::CLOSE_PAREN)
		{
			std::cerr << "Expected ')'" << std::endl;
			exit(EXIT_FAILURE);
		}

		Consume();

		// check if either the next token doesn't exist or isnt a SEMICOLON
		if (!Peek().has_value() || Peek().value().type != TokenType::SEMICOLON)
		{
			std::cerr << "Expected ';'" << std::endl;
			exit(EXIT_FAILURE);
		}

		Consume();

		return NodeStmt{ .var = stmt_return };
	}
	else if (Peek().value().type == TokenType::INTEGER_DEF)
	{
		// consume integer definition 
		Consume();

		// check if either the enxt token DNE or isn't an identifier
		if (!Peek().has_value() || Peek().value().type != TokenType::IDENT)
		{
			std::cerr << "Invalid integer definition, expected an variable name after INT." << std::endl;
			exit(EXIT_FAILURE);
		}

		auto stmt_INT_def = NodeStmtIntDef{ .IDENT = Consume() };

		// no clue if this works but at this point we know that there is an IDENT token and
		// the way that IDENTs are created require them to be created with a value
		// gets the value at this pointer
		std::string varName = *stmt_INT_def.IDENT.value;


		// check if token DNE or isn't an =
		if (!Peek().has_value() || Peek().value().type != TokenType::EQUALS)
		{
			std::cerr << "Invalid integer definition, expected an `=` after " << varName << std::endl;
			exit(EXIT_FAILURE);
		}

		Consume();


		if (auto expr = ParseExpr())
		{
			stmt_INT_def.expr = expr.value();
		}
		else {
			std::cerr << "Invalid integer definition, expected integer literal or variable." << std::endl;
			exit(EXIT_FAILURE);
		}

		if (!Peek().has_value() || Peek().value().type != TokenType::SEMICOLON)
		{
			std::cerr << "Invalid integer definition, expected ';'" << std::endl;
			exit(EXIT_FAILURE);
		}

		Consume();

		return NodeStmt{ .var = stmt_INT_def };
	}

	return {};
}

std::optional<NodeProg> Parser::ParseProgram()
{
	NodeProg prog;
	while (Peek().has_value())
	{
		if (auto stmt = ParseStmt() ) {
			prog.stmts.push_back(stmt.value());
			continue;
		}
		std::cerr << "Invalid statement" << std::endl;
		exit(EXIT_FAILURE);
	}

	return prog;
}

[[nodiscard]] std::optional <Tokens> Parser::Peek(int offset) const
{
	if (m_index + offset >= m_tokens.size())
	{
		return {};
	}

	return m_tokens.at(m_index + offset);
}

Tokens Parser::Consume()
{
	return m_tokens.at(m_index++);
}