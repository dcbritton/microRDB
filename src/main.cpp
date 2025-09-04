// main.cpp

#include <string>
#include <iostream>
#include "microRDB/Lexer.hpp"
#include "microRDB/Parser.hpp"

int main(int argc, char** argv) {
    // get input
    std::string inputText; 
    std::getline(std::cin, inputText, '\n');
    // std::cout << inputText << "\n";

    // lex
    Lexer l;
    auto tokens = l.lex(inputText);
    
    // print tokens
    for (const auto& token : tokens) {
        std::cout << token.value << " ";
    } 
    std::cout << "\n";

    // parse
    Parser p(tokens);
    auto astRoot = p.parse();

    // DOT visitor
    // semantic analyis visitor
    // execution
    return 0;
}
