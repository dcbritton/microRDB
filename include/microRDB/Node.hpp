// Node.hpp

#ifndef NODE
#define NODE

#include <memory>
#include <vector>
#include "microRDB/Visitor.hpp"

namespace Node {

    // node
    struct Node {
        virtual void accept(Visitor* v) const {}
    };

    // script
    struct Script : Node {
        const std::vector<std::unique_ptr<Node>> statements;

        Script(std::vector<std::unique_ptr<Node>>& statements) : statements(std::move(statements)) {}
        void accept(Visitor* v) const { v->visit(this); }
    };

    // create
    struct Create : Node {
        const std::string tableName;
        const std::unique_ptr<Node> expression;

        Create(const std::string& tableName, std::unique_ptr<Node> expression) : tableName(tableName), expression(std::move(expression)) {}
        void accept(Visitor* v) const { v->visit(this); }
    };

    // name-type list
    struct NameTypeList : Node {
        const std::vector<std::unique_ptr<Node>> nameTypePairs;

        NameTypeList(std::vector<std::unique_ptr<Node>>& nameTypePairs)
            : nameTypePairs(std::move(nameTypePairs)) {}
        void accept(Visitor* v) const { v->visit(this); }
    };

    // name-type pair
    struct NameTypePair : Node {
        const std::string name;
        const std::string type;
        const std::string numChars;

        NameTypePair(const std::string& name, const std::string& type)
            : name(name), type(type), numChars("") {}
        NameTypePair(const std::string& name, const std::string& type, const std::string& numChars)
            : name(name), type(type), numChars(numChars) {}
        void accept(Visitor* v) const { v->visit(this); }
    };

    // drop
    struct Drop : Node {
        const std::string tableName;
        
        Drop(const std::string& tableName)
            : tableName(tableName) {}
        void accept(Visitor* v) const { v->visit(this); }
    };

    // filter
    struct Filter : Node {
        const std::unique_ptr<Node> expr;
        Filter(std::unique_ptr<Node> expr) : expr(std::move(expr)) {}
        void accept(Visitor* v) const { v->visit(this); }
    };

    // update
    struct Update : Node {
        const std::string tableName;
        const std::unique_ptr<Node> assignList;
        const std::vector<std::unique_ptr<Filter>> filters;

        Update(const std::string& tableName, std::unique_ptr<Node> assignList, std::vector<std::unique_ptr<Filter>>& filters)
            : tableName(tableName), assignList(std::move(assignList)), filters(std::move(filters)) {}
        void accept(Visitor* v) const { v->visit(this); }
    };

    // assign list
    struct AssignList : Node {
        const std::vector<std::unique_ptr<Node>> assigns;
        AssignList(std::vector<std::unique_ptr<Node>>& assigns) : assigns(std::move(assigns)) {}
        void accept(Visitor* v) const { v->visit(this); }
    };

    // assign
    struct Assign : Node {
        const std::string name;
        const std::unique_ptr<Node> expr;
        Assign(const std::string& name, std::unique_ptr<Node> expr)
            : name(name), expr(std::move(expr)) {}
        void accept(Visitor* v) const { v->visit(this); }
    };

    // delete
    struct Delete : Node {
        const std::string tableName;
        const std::vector<std::unique_ptr<Filter>> filters;

        Delete(const std::string& tableName, std::vector<std::unique_ptr<Filter>>& filters)
            : tableName(tableName), filters(std::move(filters)) {}
        void accept(Visitor* v) const { v->visit(this); }
    };


    // insert
    struct Insert : Node {
        const std::string tableName;
        const std::vector<std::unique_ptr<Node>> expressionLists;

        Insert(const std::string& tableName, std::vector<std::unique_ptr<Node>>& expressionLists) 
            : tableName(tableName), expressionLists(std::move(expressionLists)) {}
        void accept(Visitor* v) const { v->visit(this); }
    };

    // expression list
    struct ExpressionList : Node {
        const std::vector<std::unique_ptr<Node>> expressions;

        ExpressionList(std::vector<std::unique_ptr<Node>>& expressions) 
            : expressions(std::move(expressions)) {}
        void accept(Visitor* v) const { v->visit(this); }
    };

    // or expression
    struct OrExpression : Node {
        const std::unique_ptr<Node> LHS;
        const std::unique_ptr<Node> RHS;

        OrExpression(std::unique_ptr<Node> LHS, std::unique_ptr<Node> RHS)
            : LHS(std::move(LHS)), RHS(std::move(RHS)) {}
        void accept(Visitor* v) const { v->visit(this); }
    };

    // and expression
    struct AndExpression : Node {
        const std::unique_ptr<Node> LHS;
        const std::unique_ptr<Node> RHS;

        AndExpression(std::unique_ptr<Node> LHS, std::unique_ptr<Node> RHS)
            : LHS(std::move(LHS)), RHS(std::move(RHS)) {}
        void accept(Visitor* v) const { v->visit(this); }
    };

    // equality expression
    struct EqualityExpression : Node {
        const std::unique_ptr<Node> LHS;
        const std::unique_ptr<Node> RHS;
        const std::string op; // ==, !=

        EqualityExpression(std::unique_ptr<Node> LHS, std::unique_ptr<Node> RHS, const std::string& op)
            : LHS(std::move(LHS)), RHS(std::move(RHS)), op(op) {}
        void accept(Visitor* v) const { v->visit(this); }
    };

    // relational expression
    struct RelationalExpression : Node {
        const std::unique_ptr<Node> LHS;
        const std::unique_ptr<Node> RHS;
        const std::string op; // <, >, <=, >=

        RelationalExpression(std::unique_ptr<Node> LHS, std::unique_ptr<Node> RHS, const std::string& op)
            : LHS(std::move(LHS)), RHS(std::move(RHS)), op(op) {}
        void accept(Visitor* v) const { v->visit(this); }
    };

    // additive expression
    struct AdditiveExpression : Node {
        const std::unique_ptr<Node> LHS;
        const std::unique_ptr<Node> RHS;
        const std::string op; // +, -

        AdditiveExpression(std::unique_ptr<Node> LHS, std::unique_ptr<Node> RHS, const std::string& op)
            : LHS(std::move(LHS)), RHS(std::move(RHS)), op(op) {}
        void accept(Visitor* v) const { v->visit(this); }
    };

    // multiplicative expression
    struct MultiplicativeExpression : Node {
        const std::unique_ptr<Node> LHS;
        const std::unique_ptr<Node> RHS;
        const std::string op; // *, /, %

        MultiplicativeExpression(std::unique_ptr<Node> LHS, std::unique_ptr<Node> RHS, const std::string& op)
            : LHS(std::move(LHS)), RHS(std::move(RHS)), op(op) {}
        void accept(Visitor* v) const { v->visit(this); }
    };

    // identifier
    struct Identifier : Node {
        const std::string name;

        Identifier(const std::string& name)
            : name(name) {}
        void accept(Visitor* v) const { v->visit(this); }
    };

    // int literal
    struct IntLiteral : Node {
        const int value;

        IntLiteral(int value)
            : value(value) {}
        void accept(Visitor* v) const { v->visit(this); }
    };

    // float literal
    struct FloatLiteral : Node {
        const float value;

        FloatLiteral(float value)
            : value(value) {}
        void accept(Visitor* v) const { v->visit(this); }
    };

    // bool literal
    struct BoolLiteral : Node {
        const bool value;

        BoolLiteral(bool value)
            : value(value) {}
        void accept(Visitor* v) const { v->visit(this); }
    };

    // chars literal
    struct CharsLiteral : Node {
        const std::string value;
        CharsLiteral(const std::string& value)
            : value(value) {}
        void accept(Visitor* v) const { v->visit(this); }
    };

    // select expression
    struct SelectExpression : Node {
        const std::unique_ptr<Node> LHS; // SelectExpression or ProjectExpression
        const std::unique_ptr<Node> RHS; // ORExpression

        SelectExpression(std::unique_ptr<Node> LHS, std::unique_ptr<Node> RHS)
            : LHS(std::move(LHS)), RHS(std::move(RHS)) {}
        void accept(Visitor* v) const { v->visit(this); }
    };

    // project expression
    struct ProjectExpression : Node {
        const std::unique_ptr<Node> LHS; // ProjectExpression or UnionExpression
        const std::unique_ptr<Node> RHS; // ColumnList

        ProjectExpression(std::unique_ptr<Node> LHS, std::unique_ptr<Node> RHS)
            : LHS(std::move(LHS)), RHS(std::move(RHS)) {}
        void accept(Visitor* v) const { v->visit(this); }
    };

    // column list
    struct ColumnList : Node {
        const std::vector<std::unique_ptr<Identifier>> columns;
        ColumnList(std::vector<std::unique_ptr<Identifier>>& columns) : columns(std::move(columns)) {}
        void accept(Visitor* v) const { v->visit(this); }
    };

    // union expression
    struct UnionExpression : Node {
        const std::unique_ptr<Node> LHS;
        const std::unique_ptr<Node> RHS;

        UnionExpression(std::unique_ptr<Node> LHS, std::unique_ptr<Node> RHS)
            : LHS(std::move(LHS)), RHS(std::move(RHS)) {}
        void accept(Visitor* v) const { v->visit(this); }
    };

    // diff expression
    struct DifferenceExpression : Node {
        const std::unique_ptr<Node> LHS;
        const std::unique_ptr<Node> RHS;

        DifferenceExpression(std::unique_ptr<Node> LHS, std::unique_ptr<Node> RHS)
            : LHS(std::move(LHS)), RHS(std::move(RHS)) {}
        void accept(Visitor* v) const { v->visit(this); }
    };

    // intersect expression
    struct IntersectExpression : Node {
        const std::unique_ptr<Node> LHS;
        const std::unique_ptr<Node> RHS;

        IntersectExpression(std::unique_ptr<Node> LHS, std::unique_ptr<Node> RHS)
            : LHS(std::move(LHS)), RHS(std::move(RHS)) {}
        void accept(Visitor* v) const { v->visit(this); }
    };

    // join expression
    struct JoinExpression : Node {
        const std::unique_ptr<Node> LHS;
        const std::unique_ptr<Node> RHS;

        JoinExpression(std::unique_ptr<Node> LHS, std::unique_ptr<Node> RHS)
            : LHS(std::move(LHS)), RHS(std::move(RHS)) {}
        void accept(Visitor* v) const { v->visit(this); }
    };

}

#endif
