// DOTVisitor.cpp

#include <string>
#include "microRDB/Node.hpp"
#include "microRDB/DOTVisitor.hpp"

// visit script
void DOTVisitor::visit(const Node::Script* n) {
            // header
    dotFile.open(fileName);
    dotFile << "graph G {\n";

    int thisId = nodeId;
    ++nodeId;

    // create this node
    dotFile << "node" << std::to_string(thisId)
            << " [label=\"program\"];\n";

    // process child(ren)
    std::vector<int> statementIds = {};
    for (const auto& statement : n->statements) {
        statementIds.push_back(nodeId);
        statement->accept(this);
    }

    // connect child(ren) to this node
    for (auto id : statementIds) {
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(id) << ";\n";
    }

    // footer
    dotFile << "}\n";
    dotFile.close();
}

// visit create
void DOTVisitor::visit(const Node::Create* n) {
    size_t thisId = nodeId;
    ++nodeId;

    // create this node
    dotFile << "node" << std::to_string(thisId)
            << " [label=\"create\\n" + n->tableName + "\"];\n";

    // process child(ren)
    int exprId = nodeId;
    n->expression->accept(this);

    // connect child(ren) to this node
    dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(exprId) << ";\n";
}

// visit name-type list
void DOTVisitor::visit(const Node::NameTypeList* n) {
    size_t thisId = nodeId;
    ++nodeId;

    // create this node
    dotFile << "node" << std::to_string(thisId)
            << " [label=\"name-type list\"];\n";

    // process child(ren)
    std::vector<int> pairIds = {};
    for (const auto& pair : n->nameTypePairs) {
        pairIds.push_back(nodeId);
        pair->accept(this);
    }

    // connect child(ren) to this node
    for (auto id : pairIds) {
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(id) << ";\n";
    }
}

// visit name-type pair
void DOTVisitor::visit(const Node::NameTypePair* n) {
    size_t thisId = nodeId;
    ++nodeId;

    // create this node
    dotFile << "node" << std::to_string(thisId)
            << " [label=\"name-type pair\\n" + n->name + ":" + n->type + (n->numChars != "" ? ("(" + n->numChars + ")") : "") + "\"];\n";
}

// visit drop
void DOTVisitor::visit(const Node::Drop* n) {
    size_t thisId = nodeId;
    ++nodeId;

    // create this node
    dotFile << "node" << std::to_string(thisId)
            << " [label=\"drop\\n" + n->tableName + "\"];\n";
}

// visit delete
void DOTVisitor::visit(const Node::Delete* n) {
    size_t thisId = nodeId;
    ++nodeId;

    // create this node
    dotFile << "node" << std::to_string(thisId)
            << " [label=\"delete\\n" + n->tableName + "\"];\n";

    // process child(ren)
    std::vector<int> filterIds = {};
    for (const auto& filter : n->filters) {
        filterIds.push_back(nodeId);
        filter->accept(this);
    }

    // connect child(ren) to this node
    for (auto id : filterIds) {
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(id) << ";\n";
    }
}

// visit filter
void DOTVisitor::visit(const Node::Filter* n) {
    size_t thisId = nodeId;
    ++nodeId;

    // create this node
    dotFile << "node" << std::to_string(thisId)
            << " [label=\"filter\"];\n";

    // process child(ren)
    int exprId = nodeId;
    n->expr->accept(this);

    // connect child(ren) to this node
    dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(exprId) << ";\n";
}

// visit update
void DOTVisitor::visit(const Node::Update* n) {
    size_t thisId = nodeId;
    ++nodeId;

    // create this node
    dotFile << "node" << std::to_string(thisId)
            << " [label=\"update\\n" + n->tableName + "\"];\n";

    // process assign list child
    int assignListId = nodeId;
    n->assignList->accept(this);

    // connect assign list child to this node
    dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(assignListId) << ";\n";

    // process filter children
    std::vector<int> filterIds = {};
    for (const auto& filter : n->filters) {
        filterIds.push_back(nodeId);
        filter->accept(this);
    }

    // connect filter children to this node
    for (auto id : filterIds) {
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(id) << ";\n";
    }
}

// visit assign list
void DOTVisitor::visit(const Node::AssignList* n) {
    size_t thisId = nodeId;
    ++nodeId;

    // create this node
    dotFile << "node" << std::to_string(thisId)
            << " [label=\"assign list\"];\n";

    // process child(ren)
    std::vector<int> assignIds = {};
    for (const auto& assign : n->assigns) {
        assignIds.push_back(nodeId);
        assign->accept(this);
    }

    // connect child(ren) to this node
    for (auto id : assignIds) {
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(id) << ";\n";
    }
}

// visit assign
void DOTVisitor::visit(const Node::Assign* n) {
    size_t thisId = nodeId;
    ++nodeId;

    // create this node
    dotFile << "node" << std::to_string(thisId)
            << " [label=\"assign\\n" + n->name + "\"];\n";

    // process child(ren)
    int exprId = nodeId;
    n->expr->accept(this);

    // connect child(ren) to this node
    dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(exprId) << ";\n";
}

// visit insert
void DOTVisitor::visit(const Node::Insert* n) {
    size_t thisId = nodeId;
    ++nodeId;

    // create this node
    dotFile << "node" << std::to_string(thisId)
            << " [label=\"insert\\n" + n->tableName + "\"];\n";

    // process child(ren)
    std::vector<int> exprListIds = {};
    for (const auto& exprList : n->expressionLists) {
        exprListIds.push_back(nodeId);
        exprList->accept(this);
    }

    // connect child(ren) to this node
    for (auto id : exprListIds) {
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(id) << ";\n";
    }
}

// visit expression list
void DOTVisitor::visit(const Node::ExpressionList* n) {
    size_t thisId = nodeId;
    ++nodeId;

    // create this node
    dotFile << "node" << std::to_string(thisId)
            << " [label=\"expression list\"];\n";

    // process child(ren)
    std::vector<int> exprIds = {};
    for (const auto& expr : n->expressions) {
        exprIds.push_back(nodeId);
        expr->accept(this);
    }

    // connect child(ren) to this node
    for (auto id : exprIds) {
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(id) << ";\n";
    }
}

// visit or expression
void DOTVisitor::visit(const Node::OrExpression* n) {
    size_t thisId = nodeId;
    ++nodeId;

    // create this node
    dotFile << "node" << std::to_string(thisId)
            << " [label=\"||\"];\n";

    // process children
    int leftId = nodeId;
    n->LHS->accept(this);
    int rightId = nodeId;
    n->RHS->accept(this);

    // connect children to this node
    dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(leftId) << ";\n";
    dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(rightId) << ";\n";
}

// visit and expression
void DOTVisitor::visit(const Node::AndExpression* n) {
    size_t thisId = nodeId;
    ++nodeId;

    // create this node
    dotFile << "node" << std::to_string(thisId)
            << " [label=\"&&\"];\n";

    // process children
    int leftId = nodeId;
    n->LHS->accept(this);
    int rightId = nodeId;
    n->RHS->accept(this);

    // connect children to this node
    dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(leftId) << ";\n";
    dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(rightId) << ";\n";
}

// visit equality expression
void DOTVisitor::visit(const Node::EqualityExpression* n) {
    size_t thisId = nodeId;
    ++nodeId;

    // create this node
    dotFile << "node" << std::to_string(thisId)
            << " [label=\"" + n->op + "\"];\n";

    // process children
    int leftId = nodeId;
    n->LHS->accept(this);
    int rightId = nodeId;
    n->RHS->accept(this);

    // connect children to this node
    dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(leftId) << ";\n";
    dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(rightId) << ";\n";
}

// visit relational expression
void DOTVisitor::visit(const Node::RelationalExpression* n) {
    size_t thisId = nodeId;
    ++nodeId;

    // create this node
    dotFile << "node" << std::to_string(thisId)
            << " [label=\"" + n->op + "\"];\n";

    // process children
    int leftId = nodeId;
    n->LHS->accept(this);
    int rightId = nodeId;
    n->RHS->accept(this);

    // connect children to this node
    dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(leftId) << ";\n";
    dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(rightId) << ";\n";
}

// visit additive expression
void DOTVisitor::visit(const Node::AdditiveExpression* n) {
    size_t thisId = nodeId;
    ++nodeId;

    // create this node
    dotFile << "node" << std::to_string(thisId)
            << " [label=\"" + n->op + "\"];\n";

    // process children
    int leftId = nodeId;
    n->LHS->accept(this);
    int rightId = nodeId;
    n->RHS->accept(this);

    // connect children to this node
    dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(leftId) << ";\n";
    dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(rightId) << ";\n";
}

// visit multiplicative expression
void DOTVisitor::visit(const Node::MultiplicativeExpression* n) {
    size_t thisId = nodeId;
    ++nodeId;

    // create this node
    dotFile << "node" << std::to_string(thisId)
            << " [label=\"" + n->op + "\"];\n";

    // process children
    int leftId = nodeId;
    n->LHS->accept(this);
    int rightId = nodeId;
    n->RHS->accept(this);

    // connect children to this node
    dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(leftId) << ";\n";
    dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(rightId) << ";\n";
}

// visit identifier
void DOTVisitor::visit(const Node::Identifier* n) {
    size_t thisId = nodeId;
    ++nodeId;

    // create this node
    dotFile << "node" << std::to_string(thisId)
            << " [label=\"identifier\\n" + n->name + "\"];\n";
}

// visit int literal
void DOTVisitor::visit(const Node::IntLiteral* n) {
    size_t thisId = nodeId;
    ++nodeId;

    // create this node
    dotFile << "node" << std::to_string(thisId)
            << " [label=\"int literal\\n" + std::to_string(n->value) + "\"];\n";
}

// visit float literal
void DOTVisitor::visit(const Node::FloatLiteral* n) {
    size_t thisId = nodeId;
    ++nodeId;

    // create this node
    dotFile << "node" << std::to_string(thisId)
            << " [label=\"float literal\\n" + std::to_string(n->value) + "\"];\n";
}

// visit bool literal
void DOTVisitor::visit(const Node::BoolLiteral* n) {
    size_t thisId = nodeId;
    ++nodeId;

    // create this node
    dotFile << "node" << std::to_string(thisId)
            << " [label=\"bool literal\\n" + std::string(n->value ? "true" : "false") + "\"];\n";
}

// visit chars literal
void DOTVisitor::visit(const Node::CharsLiteral* n) {
    size_t thisId = nodeId;
    ++nodeId;

    // create this node
    dotFile << "node" << std::to_string(thisId)
            << " [label=\"chars literal\\n" + n->value + "\"];\n";
}

// visit select expression
void DOTVisitor::visit(const Node::SelectExpression* n) {
    size_t thisId = nodeId;
    ++nodeId;

    // create this node
    dotFile << "node" << std::to_string(thisId)
            << " [label=\"select expression\"];\n";

    // process children
    int leftId = nodeId;
    n->LHS->accept(this);
    int rightId = nodeId;
    n->RHS->accept(this);

    // connect children to this node
    dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(leftId) << ";\n";
    dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(rightId) << ";\n";
}

// visit project expression
void DOTVisitor::visit(const Node::ProjectExpression* n) {
    size_t thisId = nodeId;
    ++nodeId;

    // create this node
    dotFile << "node" << std::to_string(thisId)
            << " [label=\"project expression\"];\n";

    // process children
    int leftId = nodeId;
    n->LHS->accept(this);
    int rightId = nodeId;
    n->RHS->accept(this);

    // connect children to this node
    dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(leftId) << ";\n";
    dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(rightId) << ";\n";
}

// visit column list
void DOTVisitor::visit(const Node::ColumnList* n) {
    size_t thisId = nodeId;
    ++nodeId;

    // create this node
    dotFile << "node" << std::to_string(thisId)
            << " [label=\"column list\"];\n";

    // process child(ren)
    std::vector<int> columnIds = {};
    for (const auto& column : n->columns) {
        columnIds.push_back(nodeId);
        column->accept(this);
    }

    // connect child(ren) to this node
    for (auto id : columnIds) {
        dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(id) << ";\n";
    }
}

// visit union expression
void DOTVisitor::visit(const Node::UnionExpression* n) {
    size_t thisId = nodeId;
    ++nodeId;

    // create this node
    dotFile << "node" << std::to_string(thisId)
            << " [label=\"union expression\"];\n";

    // process children
    int leftId = nodeId;
    n->LHS->accept(this);
    int rightId = nodeId;
    n->RHS->accept(this);

    // connect children to this node
    dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(leftId) << ";\n";
    dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(rightId) << ";\n";
}

// visit difference expression
void DOTVisitor::visit(const Node::DifferenceExpression* n) {
    size_t thisId = nodeId;
    ++nodeId;

    // create this node
    dotFile << "node" << std::to_string(thisId)
            << " [label=\"difference expression\"];\n";

    // process children
    int leftId = nodeId;
    n->LHS->accept(this);
    int rightId = nodeId;
    n->RHS->accept(this);

    // connect children to this node
    dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(leftId) << ";\n";
    dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(rightId) << ";\n";
}

// visit intersect expression
void DOTVisitor::visit(const Node::IntersectExpression* n) {
    size_t thisId = nodeId;
    ++nodeId;

    // create this node
    dotFile << "node" << std::to_string(thisId)
            << " [label=\"intersect expression\"];\n";

    // process children
    int leftId = nodeId;
    n->LHS->accept(this);
    int rightId = nodeId;
    n->RHS->accept(this);

    // connect children to this node
    dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(leftId) << ";\n";
    dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(rightId) << ";\n";
}

// visit join expression
void DOTVisitor::visit(const Node::JoinExpression* n) {
    size_t thisId = nodeId;
    ++nodeId;

    // create this node
    dotFile << "node" << std::to_string(thisId)
            << " [label=\"join expression\"];\n";

    // process children
    int leftId = nodeId;
    n->LHS->accept(this);
    int rightId = nodeId;
    n->RHS->accept(this);

    // connect children to this node
    dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(leftId) << ";\n";
    dotFile << "node" << std::to_string(thisId) << " -- node" << std::to_string(rightId) << ";\n";
}