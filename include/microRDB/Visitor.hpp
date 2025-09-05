// Visitor.hpp

#ifndef VISITOR
#define VISITOR

#include "microRDB/Node.hpp"

// forward declaration of every node type for visitor pattern
namespace Node {
    struct Node;
    struct Script;

    struct Create;
    struct NameTypeList;
    struct NameTypePair;

    struct Drop;

    struct Delete;
    struct Filter;

    struct Update;
    struct AssignList;
    struct Assign;

    struct Insert;
    struct ExpressionList;

    struct OrExpression;
    struct AndExpression;
    struct EqualityExpression;
    struct RelationalExpression;
    struct AdditiveExpression;
    struct MultiplicativeExpression;

    struct Identifier;
    struct IntLiteral;
    struct FloatLiteral;
    struct BoolLiteral;
    struct CharsLiteral;

    struct SelectExpression;
    struct ProjectExpression;
    struct ColumnList;
    struct UnionExpression;
    struct DifferenceExpression;
    struct IntersectExpression;
    struct JoinExpression;
}

class Visitor {
private:
public:
    virtual void visit(const Node::Script* n) = 0;

    virtual void visit(const Node::Create* n) = 0;
    virtual void visit(const Node::NameTypeList* n) = 0;
    virtual void visit(const Node::NameTypePair* n) = 0;

    virtual void visit(const Node::Drop* n) = 0;

    virtual void visit(const Node::Delete* n) = 0;
    virtual void visit(const Node::Filter* n) = 0;

    virtual void visit(const Node::Update* n) = 0;
    virtual void visit(const Node::AssignList* n) = 0;
    virtual void visit(const Node::Assign* n) = 0;

    virtual void visit(const Node::Insert* n) = 0;
    virtual void visit(const Node::ExpressionList* n) = 0;

    virtual void visit(const Node::OrExpression* n) = 0;
    virtual void visit(const Node::AndExpression* n) = 0;
    virtual void visit(const Node::EqualityExpression* n) = 0;
    virtual void visit(const Node::RelationalExpression* n) = 0;
    virtual void visit(const Node::AdditiveExpression* n) = 0;
    virtual void visit(const Node::MultiplicativeExpression* n) = 0;

    virtual void visit(const Node::Identifier* n) = 0;
    virtual void visit(const Node::IntLiteral* n) = 0;
    virtual void visit(const Node::FloatLiteral* n) = 0;
    virtual void visit(const Node::BoolLiteral* n) = 0;
    virtual void visit(const Node::CharsLiteral* n) = 0;

    virtual void visit(const Node::SelectExpression* n) = 0;
    virtual void visit(const Node::ProjectExpression* n) = 0;
    virtual void visit(const Node::ColumnList* n) = 0;
    virtual void visit(const Node::UnionExpression* n) = 0;
    virtual void visit(const Node::DifferenceExpression* n) = 0;
    virtual void visit(const Node::IntersectExpression* n) = 0;
    virtual void visit(const Node::JoinExpression* n) = 0;

    // virtual void visit(const Node::* n) = 0;
};

#endif
