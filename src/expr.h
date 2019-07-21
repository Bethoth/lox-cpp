#ifndef expr_h
#define expr_h

// WARNING: THIS FILE IS AUTOGENERATED
// Changes you make will not be kept.

/**
 * In loxc, an expression is just data and as such has no associated methods.
 * Instead, loxc makes use of the visitor pattern to visit expressions. For
 * more information on each expression, and to add another one see
 * ../tools/loxc_expressions.txt
 */

#include <memory> // std::shared_ptr
#include <variant>
#include "token.h"
#include "val.h"

using Expr = std::variant<
	std::monostate,
	std::shared_ptr<struct BinaryExpr>,
	std::shared_ptr<struct GroupingExpr>,
	std::shared_ptr<struct LiteralExpr>,
	std::shared_ptr<struct UnaryExpr>,
	std::shared_ptr<struct VarExpr>>;

struct BinaryExpr
{
	Expr left;
	loxc::token op;
	Expr right;

	BinaryExpr (Expr left_in, loxc::token op_in, Expr right_in)
		: left(std::move(left_in)), op(std::move(op_in)), right(std::move(right_in)) {}
};

struct GroupingExpr
{
	Expr expression;

	GroupingExpr (Expr expression_in)
		: expression(std::move(expression_in)) {}
};

struct LiteralExpr
{
	Val value;

	LiteralExpr (Val value_in)
		: value(std::move(value_in)) {}
};

struct UnaryExpr
{
	loxc::token op;
	Expr right;

	UnaryExpr (loxc::token op_in, Expr right_in)
		: op(std::move(op_in)), right(std::move(right_in)) {}
};

struct VarExpr
{
	loxc::token name;

	VarExpr (loxc::token name_in)
		: name(std::move(name_in)) {}
};

#endif