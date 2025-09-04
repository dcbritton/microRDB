// Token.cpp

#include "microRDB/Token.hpp"

std::string Token::toString() const {
    switch (type) {
        case identifier: return "identifier";
        case intLiteral: return "integer literal";
        case floatLiteral: return "float literal";
        case charsLiteral: return "characters literal";
        case kwFalse: return "false keyword";
        case kwTrue: return "true keyword";

        case kwInt: return "int keyword";
        case kwFloat: return "float keyword";
        case kwBool: return "bool keyword";
        case kwChars: return "chars keyword";

        case opLogicalAnd: return "logical and";
        case opLogicalOr: return "logical or";
        case opEquals: return "equals";
        case opNotEquals: return "not equals";
        case opLessThan: return "less than";
        case opLessThanOrEquals: return "less than or equals";
        case opGreaterThan: return "greater than";
        case opGreaterThanOrEquals: return "greater than or equals";
        case opMinus: return "minus";
        case opPlus: return "plus";
        case opMultiply: return "multiply";
        case opDivide: return "divide";
        case opModulus: return "modulus";
        case opNot: return "not";

        case at: return "at symbol";
        case sigil: return "sigil";
        case opAssign: return "assignment";
        case exclamationPoint: return "exclamation point";
        case questionMark: return "question mark";
        case opWalrus: return "walrus operator";
        case arrowLeft: return "left arrow";
        case arrowRight: return "right arrow";
        case opUnion: return "union";
        case opIntersect: return "intersection";
        case opJoin: return "join";

        case openParen: return "open parenthesis";
        case closeParen: return "close parenthesis";
        case comma: return "comma";
        case dot: return "dot";
        case colon: return "colon";
        case semicolon: return "semicolon";

        default: return "unknown token";
    }
}
