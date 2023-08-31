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
	std::vector <Tokens> expressionTokens;
	while (Peek().has_value() && Peek().value().type != TokenType::SEMICOLON)
	{
		expressionTokens.push_back(Consume());
	}

	if (expressionTokens.size() == 0)
	{
		std::cerr << "Error, missing expression before `;`" << std::endl;
		exit(EXIT_FAILURE);
	}

	// if its a simple one token
	if (expressionTokens.size() == 1)
	{
		// this may not work but im thinking do a while (Peek() != TokenType::SEMICOLON) and add everything in...
		if (expressionTokens[0].type == TokenType::INT_LITERAL)
		{
			return NodeExpr{ .var = NodeExprIntLit {.int_lit = expressionTokens[0]}};
		}
		else if (expressionTokens[0].type == TokenType::IDENT)
		{
			return NodeExpr{ .var = NodeExprIdent {.ident = expressionTokens[0] } };
		}

		return {};
	}

	return NodeExpr{ .var = NodeExprChain {.tokens = expressionTokens} };
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
	// integer definition
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

		// check if the variable has been defined before
		if (!m_idents.empty() && std::find(m_idents.begin(), m_idents.end(), varName) != m_idents.end())
		{
			std::cerr << "Variable " << varName << " has already been initialized and defined";
			exit(EXIT_FAILURE);
		}

		// add the variable name to the list of variables
		m_idents.push_back(varName);

		// check if token DNE or isn't an =
		if (!Peek().has_value() || Peek().value().type != TokenType::EQUALS)
		{
			std::cerr << "Invalid integer definition, expected an `=` after " << varName << std::endl;
			exit(EXIT_FAILURE);
		}

		Consume();

		//std::cout << str_types[Peek().value().type] << std::endl;

		if (auto expr = ParseExpr())
		{
			stmt_INT_def.expr = expr.value();
		}
		else {
			//std::cout << str_types[Peek().value().type] << std::endl;
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
	// integer redefinition
	else if (Peek().value().type == TokenType::IDENT)
	{
		std::string varName;
		
		// check if the IDENT token doesn't have a value
		if (!Peek().value().value.has_value())
		{
			std::cerr << "Error: IDENT token has no value" << std::endl;
			exit(EXIT_FAILURE);
		}

		auto stmt_INT_assignment = NodeStmtIntAssignment{ .IDENT = Consume() };

		varName = *stmt_INT_assignment.IDENT.value;

		// check if this variable has been defined before

		// check if the list is empty or the variable has been defined before
		if (m_idents.empty() || std::find(m_idents.begin(), m_idents.end(), varName) == m_idents.end())
		{
			std::cerr << "Variable " << varName << " has not been initialized and/or defined. \nPlease define it like `INT " << varName << " = ...;`";
			exit(EXIT_FAILURE);
		}

		// check if token DNE or isn't an =
		if (!Peek().has_value() || Peek().value().type != TokenType::EQUALS)
		{
			std::cerr << "Invalid integer assignment, expected an `=` after " << varName << std::endl;
			exit(EXIT_FAILURE);
		}

		Consume();

		if (auto expr = ParseExpr())
		{
			stmt_INT_assignment.expr = expr.value();
		}
		else {
			std::cerr << "Invalid integer assignment, expected integer literal or variable." << std::endl;
			exit(EXIT_FAILURE);
		}

		if (!Peek().has_value() || Peek().value().type != TokenType::SEMICOLON)
		{
			std::cerr << "Invalid integer assignment, expected ';'" << std::endl;
			exit(EXIT_FAILURE);
		}

		Consume();

		return NodeStmt{ .var = stmt_INT_assignment };
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