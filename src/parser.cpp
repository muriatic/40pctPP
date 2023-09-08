#include <string>
#include <iostream>
#include <optional>

#include "parser.h"
#include "tokens.h"
#include "errors.h"

Parser::Parser(std::vector <Tokens> tokens)
{
	m_tokens = tokens;
}

std::optional<NodeExpr> Parser::ParseExpr()
{
	std::vector <Tokens> expressionTokens;
	int index = 0;
	//																			this addition might be bad, please note this when sth breaks ;)
	while (Peek(index).has_value() && Peek(index).value().type != TokenType::SEMICOLON && Peek().value().type != TokenType::CLOSE_PAREN)
	{
		expressionTokens.push_back(Consume());
	}

	if (expressionTokens.size() == 0)
	{
		E0101 error(position);
		error.Raise();
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
			E0102 error(position, "Invalid Return Statement, Expected '(' after return");
			error.Raise();
		}

		Consume();

		NodeStmtReturn stmt_return;
		if (auto node_expr = ParseExpr())
		{
			stmt_return = { .expr = node_expr.value() };
		}
		else {
			E0102 error(position);
			error.Raise();
		}
		// check if either the next token doesn't exist or isnt a CLOSE_PAREN
		if (!Peek().has_value() || Peek().value().type != TokenType::CLOSE_PAREN)
		{
			E0103 error(position);
			error.Raise();
		}

		Consume();

		// check if either the next token doesn't exist or isnt a SEMICOLON
		if (!Peek().has_value() || Peek().value().type != TokenType::SEMICOLON)
		{
			E0104 error(position);
			error.Raise();
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
			E0105 error(position, E0105::ErrorTypes::NO_VAR_NAME);
			error.Raise();
		}

		auto stmt_INT_def = NodeStmtIntDef{ .IDENT = Consume() };

		// no clue if this works but at this point we know that there is an IDENT token and
		// the way that IDENTs are created require them to be created with a value
		// gets the value at this pointer
		std::string varName = *stmt_INT_def.IDENT.value;

		// check if the variable has been defined before
		if (!m_idents.empty() && std::find(m_idents.begin(), m_idents.end(), varName) != m_idents.end())
		{
			E0105 error(position, E0105::ErrorTypes::ALREADY_DEFINED, varName);
			error.Raise();
		}

		// add the variable name to the list of variables
		m_idents.push_back(varName);

		// check if token DNE or isn't an =
		if (!Peek().has_value() || Peek().value().type != TokenType::EQUALS)
		{
			E0105 error(position, E0105::ErrorTypes::NO_EQUALS, varName);
			error.Raise();
		}

		Consume();

		//std::cout << str_types[Peek().value().type] << std::endl;

		if (auto expr = ParseExpr())
		{
			stmt_INT_def.expr = expr.value();
		}
		else {
			E0105 error(position, E0105::ErrorTypes::EXPECT_INT_LITERAL);
			error.Raise();
		}

		if (!Peek().has_value() || Peek().value().type != TokenType::SEMICOLON)
		{
			E0105 error(position, E0105::ErrorTypes::EXPECT_SEMICOLON);
			error.Raise();
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
			E0106 error(position, E0106::ErrorTypes::NO_IDENT);
			error.Raise();
		}

		auto stmt_INT_assignment = NodeStmtIntAssignment{ .IDENT = Consume() };

		varName = *stmt_INT_assignment.IDENT.value;

		// check if this variable has been defined before

		// check if the list is empty or the variable has been defined before
		if (m_idents.empty() || std::find(m_idents.begin(), m_idents.end(), varName) == m_idents.end())
		{
			E0106 error(position, E0106::ErrorTypes::VAR_NOT_INITIALIZED, varName);
			error.Raise();
		}

		// check if token DNE or isn't an =
		if (!Peek().has_value() || Peek().value().type != TokenType::EQUALS)
		{
			E0106 error(position, E0106::ErrorTypes::NO_EQUAL, varName);
		}

		Consume();

		if (auto expr = ParseExpr())
		{
			stmt_INT_assignment.expr = expr.value();
		}
		else {
			E0106 error(position, E0106::ErrorTypes::EXPECTED_INT_LITERAL);
			error.Raise();
		}

		if (!Peek().has_value() || Peek().value().type != TokenType::SEMICOLON)
		{
			E0106 error(position, E0106::ErrorTypes::EXPECTED_SEMICOLON);
			error.Raise();
		}

		Consume();

		return NodeStmt{ .var = stmt_INT_assignment };
	}

	else if (Peek().value().type == TokenType::EXIT)
	{
		// consume exit token
		Consume();

		if (!Peek().has_value() || Peek().value().type != TokenType::SEMICOLON)
		{
			E0107 error(position);
			error.Raise();
		}

		Consume();

		return NodeStmt{ .var = NodeStmtExit{} };
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
		E0108 error(position);
		error.Raise();
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
	position.line = m_tokens.at(m_index).position.line;
	position.column = m_tokens.at(m_index).position.column;
	return m_tokens.at(m_index++);
}