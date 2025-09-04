// Lexer.hpp

#ifndef LEXER
#define LEXER

#include <vector>
#include "microRDB/Token.hpp"

class Lexer {
private:
    
public:
    std::vector<Token> lex(const std::string& text);
};

#endif
