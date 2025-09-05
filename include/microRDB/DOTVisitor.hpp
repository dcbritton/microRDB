// DOTVisitor.hpp

#ifndef DOTVISITOR
#define DOTVISITOR

#include <fstream>
#include "microRDB/Visitor.hpp"

class DOTVisitor : public Visitor {
private:
    size_t nodeId = 0;
    std::ofstream dotFile;
    std::string fileName;

public:
    DOTVisitor(const std::string& fileName) 
        : fileName(fileName) {}

    // visit script
    void visit(const Node::Script* n) override;

    // visit create
    void visit(const Node::Create* n) override;

    // visit name-type list
    void visit(const Node::NameTypeList* n) override;

    // visit name-type pair
    void visit(const Node::NameTypePair* n) override;

    // visit drop
    void visit(const Node::Drop* n) override;
    
    // visit delete
    void visit(const Node::Delete* n) override;
    
    // visit filter
    void visit(const Node::Filter* n) override;

    // visit update
    void visit(const Node::Update* n) override;

    // visit assign list
    void visit(const Node::AssignList* n) override;

    // visit assign
    void visit(const Node::Assign* n) override;

    // visit insert
    void visit(const Node::Insert* n) override;

    // visit expression list
    void visit(const Node::ExpressionList* n) override;

    // visit or expression
    void visit(const Node::OrExpression* n) override;

    // visit and expression
    void visit(const Node::AndExpression* n) override;

    // visit equality expression
    void visit(const Node::EqualityExpression* n) override;

    // visit relational expression
    void visit(const Node::RelationalExpression* n) override;

    // visit additive expression
    void visit(const Node::AdditiveExpression* n) override;

    // visit multiplicative expression
    void visit(const Node::MultiplicativeExpression* n) override;

    // visit identifier
    void visit(const Node::Identifier* n) override;

    // visit int literal
    void visit(const Node::IntLiteral* n) override;
    
    // visit float literal
    void visit(const Node::FloatLiteral* n) override;

    // visit bool literal
    void visit(const Node::BoolLiteral* n) override;

    // visit chars literal
    void visit(const Node::CharsLiteral* n) override;

    // visit select expression
    void visit(const Node::SelectExpression* n) override;

    // visit project expression
    void visit(const Node::ProjectExpression* n) override;

    // visit column list
    void visit(const Node::ColumnList* n) override;

    // visit union expression
    void visit(const Node::UnionExpression* n) override;

    // visit difference expression
    void visit(const Node::DifferenceExpression* n) override;

    // visit intersect expression
    void visit(const Node::IntersectExpression* n) override;

    // visit join expression
    void visit(const Node::JoinExpression* n) override;
};

#endif
