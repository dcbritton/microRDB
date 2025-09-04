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
    virtual ~Visitor() = 0;
    virtual void visit(const Node::Script* n) const = 0;

    virtual void visit(const Node::Create* n) const = 0;
    virtual void visit(const Node::NameTypeList* n) const = 0;
    virtual void visit(const Node::NameTypePair* n) const = 0;

    virtual void visit(const Node::Drop* n) const = 0;

    virtual void visit(const Node::Delete* n) const = 0;
    virtual void visit(const Node::Filter* n) const = 0;
    
    virtual void visit(const Node::Update* n) const = 0;
    virtual void visit(const Node::AssignList* n) const = 0;
    virtual void visit(const Node::Assign* n) const = 0;

    virtual void visit(const Node::Insert* n) const = 0;
    virtual void visit(const Node::ExpressionList* n) const = 0;

    virtual void visit(const Node::OrExpression* n) const = 0;
    virtual void visit(const Node::AndExpression* n) const = 0;
    virtual void visit(const Node::EqualityExpression* n) const = 0;
    virtual void visit(const Node::RelationalExpression* n) const = 0;
    virtual void visit(const Node::AdditiveExpression* n) const = 0;
    virtual void visit(const Node::MultiplicativeExpression* n) const = 0;

    virtual void visit(const Node::Identifier* n) const = 0;
    virtual void visit(const Node::IntLiteral* n) const = 0;
    virtual void visit(const Node::FloatLiteral* n) const = 0;
    virtual void visit(const Node::BoolLiteral* n) const = 0;
    virtual void visit(const Node::CharsLiteral* n) const = 0;

    virtual void visit(const Node::SelectExpression* n) const = 0;
    virtual void visit(const Node::ProjectExpression* n) const = 0;
    virtual void visit(const Node::ColumnList* n) const = 0;
    virtual void visit(const Node::UnionExpression* n) const = 0;
    virtual void visit(const Node::DifferenceExpression* n) const = 0;
    virtual void visit(const Node::IntersectExpression* n) const = 0;
    virtual void visit(const Node::JoinExpression* n) const = 0;

    // virtual void visit(const Node::* n) const = 0;
};

#endif
