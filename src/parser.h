#ifndef PARSER_H
#define PARSER_H

#include <variant>
#include <optional>
#include <vector>


#include "tokens.h"


// remove before production
const std::string str_types[] = { "RETURN", "INTEGER_DEF", "open_paren", "close_paren", "int_literal", "semicolon" , "ident", "equals", "ADDITION", "EXIT"};


struct NodeExprIntLit {
	Tokens int_lit;
};

//struct OperatorExpression {
//	std::variant<NodeExprIntLit, NodeExprIdent> var;
//};

struct NodeExprChain {
	std::vector<Tokens> tokens;
};

struct NodeExprIdent {
	Tokens ident;
};

struct NodeExpr {
	std::variant<NodeExprIntLit, NodeExprIdent, NodeExprChain> var;
};

struct NodeStmtExit {
	
};

struct NodeStmtReturn {
	NodeExpr expr;
};

struct NodeStmtIntDef {
	Tokens IDENT;
	NodeExpr expr;
};

struct NodeStmtIntAssignment {
	Tokens IDENT;
	NodeExpr expr;
};

struct NodeStmt {
	std::variant<NodeStmtReturn, NodeStmtExit, NodeStmtIntDef, NodeStmtIntAssignment> var;
};

struct NodeProg {
	std::vector<NodeStmt> stmts;
};


class Parser
{
	std::vector <Tokens> m_tokens;
	size_t m_index = 0;
public:
	Parser(std::vector <Tokens> tokens);
	std::optional<NodeExpr> ParseExpr();
	std::optional<NodeStmt> ParseStmt();
	std::optional<NodeProg> ParseProgram();
private:
	std::optional <Tokens> Peek(int offset = 0) const;
	Tokens Consume();
	std::vector <std::string> m_idents;
};

#endif