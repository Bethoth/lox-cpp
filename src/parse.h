// class that parses a list of tokens into an ast of expressions

#ifndef parse_h
#define parse_h

#include <vector>
#include <string>
#include <optional>

#include "token.h"
#include "token_type.h"
#include "expr.h"
#include "stmt.h"

class Parser
{
public:
    Parser() = default;
    std::optional<std::vector<Stmt>> parse(std::vector<loxc::token> in);

    // --------------
    // Error handling:
    // --------------
    struct parse_error : public std::runtime_error
    {
        using std::runtime_error::runtime_error;
    };

private:
    Stmt declaration();
    Stmt variableDeclaration();
    Stmt statement();
    Stmt printStatement();
    Stmt funcStatement();
    Stmt returnStatement();
    Stmt blockStatement();
    Stmt ifStatement();
    Stmt whileStatement();
    Stmt forStatement();
    Stmt expressionStatement();

    Expr expression();
    Expr assignment();
    Expr anonymous_function();
    Expr logical_or();
    Expr logical_and();
    Expr equality();
    Expr comparason();
    Expr addition();
    Expr multiplication();
    Expr unary();
    Expr call();
    Expr finishCall(Expr callee);
    Expr primary();

    bool match(loxc::token_type in)
    {
        if (check(in))
        {
            advance();
            return true;
        }
        return false;
    }
    template <typename... rest>
    bool match(loxc::token_type in, rest... r)
    {
        if (check(in))
        {
            advance();
            return true;
        }
        return match(r...);
    }

    bool match(std::initializer_list<loxc::token_type> in);
    bool check(loxc::token_type in) { return isAtEnd() ? false : peek().type == in; }

    loxc::token peek() const { return *current; };
    loxc::token previous() const { return *(current - 1); };
    loxc::token advance() 
    {
        if ( ! isAtEnd() ) ++current;
        return previous();
    };
    bool isAtEnd() { return peek().type == loxc::END; };

    loxc::token consume(loxc::token_type in, std::string error_message);
    static parse_error error(loxc::token bad, std::string what);
    void synchronize();

    bool had_error;

    std::vector<loxc::token> tokens;
    std::vector<loxc::token>::iterator current;
};

#endif