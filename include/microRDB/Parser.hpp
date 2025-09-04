// Parser.hpp

#ifndef PARSER
#define PARSER

#include <memory>
#include <vector>
#include "microRDB/Node.hpp"
#include "microRDB/Token.hpp"

class Parser {
private:
    const std::vector<Token>& tokens;
    std::vector<Token>::const_iterator it;

    void discard(Token::Type expectedType);
    std::string consume(Token::Type expectedType);

public:
    Parser(const std::vector<Token>& tokens) : tokens(tokens) {}
    std::unique_ptr<Node::Script> parse();
    
    std::unique_ptr<Node::Create> parseCreate();
    std::unique_ptr<Node::NameTypeList> parseNameTypeList();
    std::unique_ptr<Node::NameTypePair> parseNameTypePair();

    std::unique_ptr<Node::Drop> parseDrop();

    std::unique_ptr<Node::Delete> parseDelete();
    std::unique_ptr<Node::Filter> parseFilter();

    std::unique_ptr<Node::Update> parseUpdate();
    std::unique_ptr<Node::AssignList> parseAssignList();
    std::unique_ptr<Node::Assign> parseAssign();

    std::unique_ptr<Node::Insert> parseInsert();
    std::unique_ptr<Node::ExpressionList> parseExpressionList();

    std::unique_ptr<Node::Node> parseOrExpression();
    std::unique_ptr<Node::Node> parseAndExpression();
    std::unique_ptr<Node::Node> parseEqualityExpression();
    std::unique_ptr<Node::Node> parseRelationalExpression();
    std::unique_ptr<Node::Node> parseAdditiveExpression();
    std::unique_ptr<Node::Node> parseMultiplicativeExpression();
    std::unique_ptr<Node::Node> parsePrimary();
    
    std::unique_ptr<Node::Node> parseSelectExpression();
    std::unique_ptr<Node::Node> parseProjectExpression();
    std::unique_ptr<Node::Node> parseColumnList();
    std::unique_ptr<Node::Node> parseUnionExpression();
    std::unique_ptr<Node::Node> parseDifferenceExpression();
    std::unique_ptr<Node::Node> parseIntersectExpression();
    std::unique_ptr<Node::Node> parseJoinExpression();
    std::unique_ptr<Node::Node> parseTablePrimary();

    //std::unique_ptr<Node::> parse();
};

#endif
