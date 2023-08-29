#ifndef PARSER_H
#define PARSER_H

#include <variant>
#include <optional>
#include <vector>


#include "tokens.h"

struct NodeExprIntLit {
	Tokens int_lit;
};

struct NodeExprIdent {
	Tokens ident;
};

struct NodeExpr {
	std::variant<NodeExprIntLit, NodeExprIdent> var;
};

//struct NodeStmtExit {
//	NodeExpr expr;
//};

struct NodeStmtReturn {
	NodeExpr expr;
};

struct NodeStmtIntDef {
	Tokens IDENT;
	NodeExpr expr;
};

struct NodeStmt {
	std::variant<NodeStmtReturn/*, NodeStmtExit*/, NodeStmtIntDef> var;
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
};

#endif