// Token.hpp

#ifndef TOKEN
#define TOKEN

#include <string>

struct Token {
    enum Type {
        identifier,
        intLiteral,
        floatLiteral,
        charsLiteral,
        kwFalse,
        kwTrue,

        kwInt,
        kwFloat,
        kwBool,
        kwChars,

        opLogicalAnd,
        opLogicalOr,
        opEquals,
        opNotEquals,
        opLessThan,
        opLessThanOrEquals,
        opGreaterThan,
        opGreaterThanOrEquals,
        opMinus,
        opPlus,
        opMultiply,
        opDivide,
        opModulus,
        opNot,

        at, // @
        sigil, // $
        opAssign,
        exclamationPoint,
        questionMark,
        opWalrus,
        arrowLeft,
        arrowRight,
        opUnion, // |
        opIntersect, // &
        opJoin, // ^
        openParen,
        closeParen,
        comma,
        dot,
        colon,
        semicolon,
        tilde, // ~
    };

    Type type;
    std::string value;
    size_t lineNumber;

    Token(Type type)
        : type(type) {}
        
    Token(Type type, std::string value, size_t lineNumber)
        : type(type), value(value), lineNumber(lineNumber) {}

    operator Type() const {
        return type;
    }

    std::string toString() const;
};

#endif
