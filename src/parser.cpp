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

bool Parser::CheckParentheses()
{
	return (parentheses >= 0) ? true : false;
}

std::optional<NodeExpr> Parser::ParseExpr()
{
	NodeExpr expr;
	while (Peek().has_value() && Peek().value().type != TokenType::SEMICOLON )
	{
		if (Peek().value().type == TokenType::OPEN_PAREN)
		{
			Consume();
			parentheses++;
			expr.var.push_back(ParseExpr().value());
			continue;
		}
		else if (Peek().value().type == TokenType::IDENT)
		{
			expr.var.push_back(NodeExprIdent{.ident = Consume()});
			continue;
		}
		else if (Peek().value().type == TokenType::INT_LITERAL)
		{
			expr.var.push_back(NodeExprIntLit{.int_lit= Consume()});
			continue;
		}
		else if (Peek().value().type == TokenType::CLOSE_PAREN)
		{
			Consume();
			parentheses--;
			if (CheckParentheses())
			{
				return expr;
			}
			E0111 error(position, parentheses);
		}
		else if (std::find(OperatorGroups::OperatorTokens.begin(), OperatorGroups::OperatorTokens.end(), Peek().value().type) != OperatorGroups::OperatorTokens.end())
		{
			// check for duplicate operators (checks if current operator and next token are the same
			if (Peek().value().type == Peek(1).value().type)
			{
				E0112 error(position, E0112::ErrorTypes::DUPLICATE);
			}

			// if its not addition or subtraction just push it back and we're good
			if (
				(Peek().value().type == TokenType::ADDITION || Peek().value().type == TokenType::SUBTRACTION) 
				&& (Peek(1).value().type == TokenType::MULTIPLICATION || Peek(1).value().type == TokenType::DIVISION)) {
				E0112 error(position, E0112::ErrorTypes::ORDER);
			}

			expr.var.push_back(NodeExprOperator{ .Operation = Consume() });
			continue;
		}
		else if (Peek().value().type == TokenType::EQUALS)
		{
			E0110 error(position);
			
		}
		else if (std::find(OperatorGroups::UnaryOperators.begin(), OperatorGroups::UnaryOperators.end(), Peek().value().type) != OperatorGroups::UnaryOperators.end())
		{
			// ensure next token is a semicolon
			if (!Peek(1).has_value() || Peek(1).value().type != TokenType::SEMICOLON)
			{
				E0106 error(position, E0106::ErrorTypes::EXPECTED_SEMICOLON);
			}

			expr.var.push_back(NodeExprUnaryOperator{ .Operation = Consume() });
			continue;
		}
		E0109 error(position, Peek().value().type);
		
	}

	if (parentheses != 0)
	{
		E0111 error(position, parentheses);
	}

	return expr;
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
			
		}
		Consume();

		NodeStmtReturn stmt_return;
		if (auto node_expr = ParseExpr())
		{
			stmt_return = { .expr = node_expr.value() };
		}
		else {
			E0102 error(position);
			
		}
		// check if either the next token doesn't exist or isnt a CLOSE_PAREN
		if (!Peek().has_value() || Peek().value().type != TokenType::CLOSE_PAREN)
		{
			E0103 error(position);
			
		}

		Consume();

		// check if either the next token doesn't exist or isnt a SEMICOLON
		if (!Peek().has_value() || Peek().value().type != TokenType::SEMICOLON)
		{
			E0104 error(position);
			
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
			
		}

		// add the variable name to the list of variables
		m_idents.push_back(varName);

		// check if token DNE or isn't an =
		if (!Peek().has_value() || Peek().value().type != TokenType::EQUALS)
		{
			E0105 error(position, E0105::ErrorTypes::NO_EQUALS, varName);
			
		}

		Consume();

		//std::cout << str_types[Peek().value().type] << std::endl;

		if (auto expr = ParseExpr())
		{
			stmt_INT_def.expr = expr.value();
		}
		else {
			E0105 error(position, E0105::ErrorTypes::EXPECT_INT_LITERAL);
			
		}

		if (!Peek().has_value() || Peek().value().type != TokenType::SEMICOLON)
		{
			E0105 error(position, E0105::ErrorTypes::EXPECT_SEMICOLON);
			
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
		}

		auto stmt_INT_assignment = NodeStmtIntAssignment{ .IDENT = Consume()};

		varName = *stmt_INT_assignment.IDENT.value;

		// check if this variable has been defined before

		// check if the list is empty or the variable has been defined before
		if (m_idents.empty() || std::find(m_idents.begin(), m_idents.end(), varName) == m_idents.end())
		{
			E0106 error(position, E0106::ErrorTypes::VAR_NOT_INITIALIZED, varName);
		}

		if (std::find(OperatorGroups::UnaryOperators.begin(), OperatorGroups::UnaryOperators.end(), Peek().value().type) != OperatorGroups::UnaryOperators.end())
		{
			auto expr = ParseExpr();
			
			if (!expr)
			{
				E0106 error(position, E0106::ErrorTypes::EXPECTED_OPERATOR);
			}

			// more than one operator or something after the unary operator
			if (expr.value().var.size() != 1)
			{
				E0106 error(position, E0106::ErrorTypes::EXPECTED_SEMICOLON);
			}

			auto stmt_INT_operation = NodeStmtIntOperation{ .IDENT = stmt_INT_assignment.IDENT };

			stmt_INT_operation.expr = expr.value();

			if (!Peek().has_value() || Peek().value().type != TokenType::SEMICOLON)
			{
				E0106 error(position, E0106::ErrorTypes::EXPECTED_SEMICOLON);
			}

			Consume();

			return NodeStmt{ .var = stmt_INT_operation };
		}

		// check if token DNE or isn't an =
		if (!Peek().has_value() || (Peek().value().type != TokenType::EQUALS))
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
			
		}

		if (!Peek().has_value() || Peek().value().type != TokenType::SEMICOLON)
		{
			E0106 error(position, E0106::ErrorTypes::EXPECTED_SEMICOLON);
			
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