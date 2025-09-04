// Parser.cpp

#include <iostream>
#include "microRDB/Parser.hpp"

std::unique_ptr<Node::Script> Parser::parse() {
    it = tokens.begin();

    std::vector<std::unique_ptr<Node::Node>> statements;
    while (it != tokens.cend()) {
        // bounds check
        if (it+1 == tokens.cend()) {
            std::cout << "Parser error. Unexpected end of input on line " << it->lineNumber << ".\n";
            exit(1);
        }

        if (*(it+1) == Token::opAssign) {
            statements.push_back(parseCreate());
        }
        else if (*(it+1) == Token::arrowLeft) {
            statements.push_back(parseInsert());
        }
        else if (*(it+1) == Token::tilde) {
            statements.push_back(parseDrop());
        }
        else if (*(it+1) == Token::exclamationPoint) {
            statements.push_back(parseDelete());
        }
        else if (*(it+1) == Token::opWalrus) {
            statements.push_back(parseUpdate());
        }
        // @TODO: other statement types
        else {
            statements.push_back(parseSelectExpression());
        }

        discard(Token::semicolon);
    }

    return std::make_unique<Node::Script>(statements);
}

// CREATE - TABLE_NAME = TYPE_ID_LIST | SELECT_EXPR
std::unique_ptr<Node::Create> Parser::parseCreate() {
    std::string name = consume(Token::identifier);
    discard(Token::opAssign);
    std::unique_ptr<Node::Node> RHS;
    // @TODO: bounds check with a peek method
    if (*(it+1) == Token::colon) {
        RHS = parseNameTypeList();
    }
    else {
        RHS = parseSelectExpression();
    }

    return std::make_unique<Node::Create>(name, std::move(RHS));
}

// NAME_TYPE_LIST - NAME_TYPE_PAIR [, NAME_TYPE_PAIR]*
std::unique_ptr<Node::NameTypeList> Parser::parseNameTypeList() {
    std::vector<std::unique_ptr<Node::Node>> pairs;
    pairs.push_back(parseNameTypePair());
    while (*it == Token::comma) {
        discard(Token::comma);
        pairs.push_back(parseNameTypePair());
    }

    return std::make_unique<Node::NameTypeList>(pairs);
}

// NAME_TYPE_PAIR - IDENTIFIER : [kwInt | kwFloat | kwBool | [kwChars INTEGER]]
std::unique_ptr<Node::NameTypePair> Parser::parseNameTypePair() {
    std::string name = consume(Token::identifier);
    discard(Token::colon);
    std::string type;
    if (*it == Token::kwInt || *it == Token::kwFloat || *it == Token::kwBool) {
        type = consume(*it);
    }
    else if (*it == Token::kwChars) {
        type = consume(Token::kwChars);
        std::string numChars = consume(Token::intLiteral);

        return std::make_unique<Node::NameTypePair>(name, type, numChars);
    }
    else {
        std::cout << "Parser error. Expected a type name on line " << it->lineNumber <<  ". Got "
                  << it->toString() << " \"" << it->value
                  << "\" instead. Terminating.\n";
        exit(1);
    }
    
    return std::make_unique<Node::NameTypePair>(name, type);
}

// DROP - IDENTIFIER ~
std::unique_ptr<Node::Drop> Parser::parseDrop() {
    std::string name = consume(Token::identifier);
    discard(Token::tilde);
    return std::make_unique<Node::Drop>(name);
}

// DELETE - IDENTIFIER ! FILTER [FILTER]*
std::unique_ptr<Node::Delete> Parser::parseDelete() {
    std::string name = consume(Token::identifier);
    discard(Token::exclamationPoint);
    std::vector<std::unique_ptr<Node::Filter>> filters;
    filters.push_back(parseFilter());
    while (*it == Token::questionMark) {
        filters.push_back(parseFilter());
    }

    return std::make_unique<Node::Delete>(name, filters);
}

// FILTER - ? OR_EXPR
std::unique_ptr<Node::Filter> Parser::parseFilter() {
    discard(Token::questionMark);
    auto expr = parseOrExpression();
    return std::make_unique<Node::Filter>(std::move(expr));
}

// UPDATE - IDENTIFIER := ASSIGN_LIST FILTER [FILTER]*
std::unique_ptr<Node::Update> Parser::parseUpdate() {
    std::string name = consume(Token::identifier);
    discard(Token::opWalrus);
    auto assignList = parseAssignList();
    std::vector<std::unique_ptr<Node::Filter>> filters;
    filters.push_back(parseFilter());
    while (*it == Token::questionMark) {
        filters.push_back(parseFilter());
    }

    return std::make_unique<Node::Update>(name, std::move(assignList), filters);
}

// ASSIGN_LIST - ASSIGN [, ASSIGN]*
std::unique_ptr<Node::AssignList> Parser::parseAssignList() {
    std::vector<std::unique_ptr<Node::Node>> assigns;
    assigns.push_back(parseAssign());
    while (*it == Token::comma) {
        discard(Token::comma);
        assigns.push_back(parseAssign());
    }
    
    return std::make_unique<Node::AssignList>(assigns);
}

// ASSIGN - IDENTIFIER ( OR_EXPR )
std::unique_ptr<Node::Assign> Parser::parseAssign() {
    std::string name = consume(Token::identifier);
    discard(Token::openParen);
    auto expr = parseOrExpression();
    discard(Token::closeParen);

    return std::make_unique<Node::Assign>(name, std::move(expr));
}

// INSERT - TABLE_NAME <- EXPRESSION_LIST [<- EXPRESSION_LIST]*
std::unique_ptr<Node::Insert> Parser::parseInsert() {
    std::vector<std::unique_ptr<Node::Node>> expressionLists;
    std::string name = consume(Token::identifier);
    discard(Token::arrowLeft);
    expressionLists.push_back(parseExpressionList());
    while (*it == Token::arrowLeft) {
        discard (Token::arrowLeft);
        expressionLists.push_back(parseExpressionList());
    }

    return std::make_unique<Node::Insert>(name, expressionLists);
}

// EXPRESSION_LIST - OR_EXPR [, OR_EXPR]*
std::unique_ptr<Node::ExpressionList> Parser::parseExpressionList() {
    std::vector<std::unique_ptr<Node::Node>> expressions;
    expressions.push_back(parseOrExpression());
    while (*it == Token::comma) {
        discard(Token::comma);
        expressions.push_back(parseOrExpression());
    }

    return std::make_unique<Node::ExpressionList>(expressions);
}

// OR_EXPR         - AND_EXPR
//                 | OR_EXPR || AND_EXPR
std::unique_ptr<Node::Node> Parser::parseOrExpression() {
    auto LHS = parseAndExpression();
    while (*it == Token::opLogicalOr) {
        discard(Token::opLogicalOr);
        auto RHS = parseAndExpression();
        LHS = std::make_unique<Node::OrExpression>(std::move(LHS), std::move(RHS));
    }

    return LHS;
}

// AND_EXPR        - EQUALITY_EXPR
//                 | AND_EXPR && EQUALITY_EXPR
std::unique_ptr<Node::Node> Parser::parseAndExpression() {
    auto LHS = parseEqualityExpression();
    while (*it == Token::opLogicalAnd) {
        discard(Token::opLogicalAnd);
        auto RHS = parseEqualityExpression();
        LHS = std::make_unique<Node::AndExpression>(std::move(LHS), std::move(RHS));
    }

    return LHS;
}

// EQUALITY_EXPR   - RELATIONAL_EXPR
//                 | EQUALITY_EXPR == RELATIONAL_EXPR
//                 | EQUALITY_EXPR != RELATIONAL_EXPR
std::unique_ptr<Node::Node> Parser::parseEqualityExpression() {
    auto LHS = parseRelationalExpression();
    while (true) {
        if (*it == Token::opEquals) {
            std::string op = "==";
            discard(Token::opEquals);
            auto RHS = parseRelationalExpression();
            LHS = std::make_unique<Node::EqualityExpression>(std::move(LHS), std::move(RHS), op);
        }
        else if (*it == Token::opNotEquals) {
            std::string op = "!=";
            discard(Token::opNotEquals);
            auto RHS = parseRelationalExpression();
            LHS = std::make_unique<Node::EqualityExpression>(std::move(LHS), std::move(RHS), op);
        }
        else {
            break;
        }
    }

    return LHS;
}

// RELATIONAL_EXPR - ADDITIVE_EXPR
//                 | RELATIONAL_EXPR < ADDITIVE_EXPR
//                 | RELATIONAL_EXPR > ADDITIVE_EXPR
//                 | RELATIONAL_EXPR <= ADDITIVE_EXPR
//                 | RELATIONAL_EXPR >= ADDITIVE_EXPR
std::unique_ptr<Node::Node> Parser::parseRelationalExpression() {
    auto LHS = parseAdditiveExpression();
    while (true) {
        if (*it == Token::opLessThan) {
            std::string op = "<";
            discard(Token::opLessThan);
            auto RHS = parseAdditiveExpression();
            LHS = std::make_unique<Node::RelationalExpression>(std::move(LHS), std::move(RHS), op);
        }
        else if (*it == Token::opGreaterThan) {
            std::string op = ">";
            discard(Token::opGreaterThan);
            auto RHS = parseAdditiveExpression();
            LHS = std::make_unique<Node::RelationalExpression>(std::move(LHS), std::move(RHS), op);
        }
        else if (*it == Token::opLessThanOrEquals) {
            std::string op = "<=";
            discard(Token::opLessThanOrEquals);
            auto RHS = parseAdditiveExpression();
            LHS = std::make_unique<Node::RelationalExpression>(std::move(LHS), std::move(RHS), op);
        }
        else if (*it == Token::opGreaterThanOrEquals) {
            std::string op = ">=";
            discard(Token::opGreaterThanOrEquals);
            auto RHS = parseAdditiveExpression();
            LHS = std::make_unique<Node::RelationalExpression>(std::move(LHS), std::move(RHS), op);
        }
        else {
            break;
        }
    }

    return LHS;
}

// ADDITIVE_EXPR   - MULT_EXPR
//                 | ADDITIVE_EXPR + MULT_EXPR
//                 | ADDITIVE_EXPR - MULT_EXPR  
std::unique_ptr<Node::Node> Parser::parseAdditiveExpression() {
    auto LHS = parseMultiplicativeExpression();
    while (true) {
        std::string op;
        if (*it == Token::opPlus) {
            op = "+";
            discard(Token::opPlus);
            auto RHS = parseMultiplicativeExpression();
            LHS = std::make_unique<Node::AdditiveExpression>(std::move(LHS), std::move(RHS), op);
        }
        else if (*it == Token::opMinus) {
            op = "-";
            discard(Token::opMinus);
            auto RHS = parseMultiplicativeExpression();
            LHS = std::make_unique<Node::AdditiveExpression>(std::move(LHS), std::move(RHS), op);
        }
        else {
            break;
        }
    }

    return LHS;
}

// MULT_EXPR       - PRIMARY
//                 | MULT_EXPR * PRIMARY
//                 | MULT_EXPR / PRIMARY
//                 | MULT_EXPR % PRIMARY
std::unique_ptr<Node::Node> Parser::parseMultiplicativeExpression() {
    auto LHS = parsePrimary();
    while (true) {
        std::string op;
        if (*it == Token::opMultiply) {
            op = "*";
            discard(Token::opMultiply);
            auto RHS = parsePrimary();
            LHS = std::make_unique<Node::MultiplicativeExpression>(std::move(LHS), std::move(RHS), op);
        }
        else if (*it == Token::opDivide) {
            op = "/";
            discard(Token::opDivide);
            auto RHS = parsePrimary();
            LHS = std::make_unique<Node::MultiplicativeExpression>(std::move(LHS), std::move(RHS), op);
        }
        else if (*it == Token::opModulus) {
            op = "%";
            discard(Token::opModulus);
            auto RHS = parsePrimary();
            LHS = std::make_unique<Node::MultiplicativeExpression>(std::move(LHS), std::move(RHS), op);
        }
        else {
            break;
        }
    }

    return LHS;
}

// PRIMARY         - ( OR_EXPR )
//                 | IDENTIFIER
//                 | INT_LITERAL
//                 | FLOAT_LITERAL
//                 | kwTrue
//                 | kwFalse
//                 | CHARS_LITERAL
std::unique_ptr<Node::Node> Parser::parsePrimary() {
    if (*it == Token::identifier) {
        std::string name = consume(Token::identifier);
        
        return std::make_unique<Node::Identifier>(name);
    }
    else if (*it == Token::intLiteral) {
        std::string value = consume(Token::intLiteral);
        int intValue = std::stoi(value);
        
        return std::make_unique<Node::IntLiteral>(intValue);
    }
    else if (*it == Token::floatLiteral) {
        std::string value = consume(Token::floatLiteral);
        float floatValue = std::stod(value);

        return std::make_unique<Node::FloatLiteral>(floatValue);
    }
    else if (*it == Token::kwTrue) {
        consume(Token::kwTrue);

        return std::make_unique<Node::BoolLiteral>(true);
    }
    else if (*it == Token::kwFalse) {
        consume(Token::kwFalse);

        return std::make_unique<Node::BoolLiteral>(false);
    }
    else if (*it == Token::charsLiteral) {
        std::string value = consume(Token::charsLiteral);

        return std::make_unique<Node::CharsLiteral>(value);
    }
    else if (*it == Token::openParen) {
        discard(Token::openParen);
        auto expr = parseOrExpression();
        discard(Token::closeParen);

        return expr;
    }

    std::cout << "Parser error. Expected a primary expression on line " << it->lineNumber << ". Got "
              << it->toString() << " \"" << it->value << "\" instead. Terminating.\n";
    exit(1);
}

// SELECT_EXPR     - PROJECT_EXPR
//                 | SELECT_EXPR ? OR_EXPR
std::unique_ptr<Node::Node> Parser::parseSelectExpression() {
    auto LHS = parseProjectExpression();
    while (*it == Token::questionMark) {
        discard(Token::questionMark);
        auto RHS = parseOrExpression();
        LHS = std::make_unique<Node::SelectExpression>(std::move(LHS), std::move(RHS));
    }

    return LHS;
}

// PROJECT_EXPR    - UNION_EXPR
//                 | PROJECT_EXPR -> COLUMN_LIST
std::unique_ptr<Node::Node> Parser::parseProjectExpression() {
    auto LHS = parseUnionExpression();
    while (*it == Token::arrowRight) {
        discard(Token::arrowRight);
        auto RHS = parseColumnList();
        LHS = std::make_unique<Node::ProjectExpression>(std::move(LHS), std::move(RHS));
    }

    return LHS;
}

// COLUMN_LIST     - IDENTIFIER [, IDENTIFIER]*
std::unique_ptr<Node::Node> Parser::parseColumnList() {
    std::vector<std::unique_ptr<Node::Identifier>> columns;
    columns.push_back(std::make_unique<Node::Identifier>(consume(Token::identifier)));
    while (*it == Token::comma) {
        discard(Token::comma);
        columns.push_back(std::make_unique<Node::Identifier>(consume(Token::identifier)));
    }

    return std::make_unique<Node::ColumnList>(columns);
}

// UNION_EXPR      - DIFF_EXPR
//                 | UNION_EXPR | DIFF_EXPR
std::unique_ptr<Node::Node> Parser::parseUnionExpression() {
    auto LHS = parseDifferenceExpression();
    while (*it == Token::opUnion) {
        discard(Token::opUnion);
        auto RHS = parseDifferenceExpression();
        LHS = std::make_unique<Node::UnionExpression>(std::move(LHS), std::move(RHS));
    }

    return LHS;
}

// DIFF_EXPR       - INTERSECT_EXPR
//                 | DIFF_EXPR - INTERSECT_EXPR
std::unique_ptr<Node::Node> Parser::parseDifferenceExpression() {
    auto LHS = parseIntersectExpression();
    while (*it == Token::opMinus) {
        discard(Token::opMinus);
        auto RHS = parseIntersectExpression();
        LHS = std::make_unique<Node::DifferenceExpression>(std::move(LHS), std::move(RHS));
    }

    return LHS;
}

// INTERSECT_EXPR  - JOIN_EXPR
//                 | INTERSECT_EXPR & JOIN_EXPR
std::unique_ptr<Node::Node> Parser::parseIntersectExpression() {
    auto LHS = parseJoinExpression();
    while (*it == Token::opIntersect) {
        discard(Token::opIntersect);
        auto RHS = parseJoinExpression();
        LHS = std::make_unique<Node::IntersectExpression>(std::move(LHS), std::move(RHS));
    }

    return LHS;
}

// JOIN_EXPR       - TABLE_PRIMARY
//                 | JOIN_EXPR ^ TABLE_PRIMARY
std::unique_ptr<Node::Node> Parser::parseJoinExpression() {
    auto LHS = parseTablePrimary();
    while (*it == Token::opJoin) {
        discard(Token::opJoin);
        auto RHS = parseTablePrimary();
        LHS = std::make_unique<Node::JoinExpression>(std::move(LHS), std::move(RHS));
    }

    return LHS;
}

// TABLE_PRIMARY   - ( PROJECT_EXPR ) 
//                 | TABLE_NAME
std::unique_ptr<Node::Node> Parser::parseTablePrimary() {
    if (*it == Token::openParen) {
        discard(Token::openParen);
        auto expr = parseProjectExpression();
        discard(Token::closeParen);
        return expr;
    }

    std::string name = consume(Token::identifier);
    return std::make_unique<Node::Identifier>(name);
}

// discard
void Parser::discard(Token::Type expectedType) {
    if (it == tokens.cend()) {
        // throw unexpected end of input
        std::cout << "Unexpected end of input.\n";
        exit(1);
    }

    if (*it != expectedType) {
        std::cout << "Parser error on line " << std::to_string(it->lineNumber)
                  << ". Expected token: " << Token(expectedType).toString() << ". Got " 
                  << it->toString() << " \"" << it->value << "\" instead. Terminating.\n";
        exit(1);
    }
    ++it;
}

// consume
std::string Parser::consume(Token::Type expectedType) {
    if (it == tokens.cend()) {
        std::cout << "Unexpected end of input.\n";
        exit(1);
    }

    if (*it != expectedType) {
        std::cout << "Parser error on line " << std::to_string(it->lineNumber)
                  << ". Expected token: " << Token(expectedType).toString() << ". Got " 
                  << it->toString() << " \"" << it->value << "\" instead. Terminating.\n";
        exit(1);
    }

    ++it;
    return (it-1)->value;

    std::cout << "Placeholder for consume(Token::Type).  Got a " << (it-1)->toString() << ":  " << (it-1)->value << ". Exiting.\n";
    exit(1);
}
