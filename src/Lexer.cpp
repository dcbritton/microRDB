// Lexer.cpp

#include <iostream>
#include <bitset>
#include <unordered_map>
#include "microRDB/Lexer.hpp"

namespace {
    std::unordered_map<std::string, Token::Type> keywordMap {
        {"int", Token::kwInt},
        {"float", Token::kwFloat},
        {"bool", Token::kwBool},
        {"chars", Token::kwChars},
        {"true", Token::kwTrue},
        {"false", Token::kwFalse}
    };

    bool isOpMinus(const Token type) {
        // conditions that guarantee opMinus, anything else assumed negative number
        // last token is a straightforward determiner
        return (type == Token::closeParen
            || type == Token::identifier
            || type == Token::intLiteral
            || type == Token::floatLiteral
            || type == Token::kwTrue
            || type == Token::kwFalse
            || type == Token::charsLiteral
            // @TODO:complete this condition
            // || tokens.back() == Token::
        );
    }
}

std::vector<Token> Lexer::lex(const std::string& text) {
    // @TODO: bounds check with a peek method
    // @TODO: determine proper bitset once grammar established
    // const std::bitset<128> validCharacterMap("");
    size_t currentLineNumber = 1;
    std::vector<Token> tokens = {};

     // traverse input characters
    auto it = text.cbegin();
    while (it != text.cend()) {

        // if (!validCharacterMap[(unsigned )(*it)]) {
        //     std::cout << "Lexer error: Invalid character \"" << *it << "\" in line " << currentLineNumber << ". Terminating with exit code: 1.\n";
        //     exit(1);
        // }

        // whitespace
        if (isspace(*it)) {

            // @NOTE: condenses any number of sequential newlines into one token
            // windows newline crlf
            if (*it == '\r' && *(it+1) == '\n') {
                while (isspace(*it)) {
                    if (*it == '\r' && *(it+1) == '\n') {
                        ++currentLineNumber;
                        it += 2;
                    }
                    else {
                        ++it;
                    }
                }
                // tokens.push_back(Token(Token::terminator, "\\n", currentLineNumber));
                continue;
            }
            // *nix 
            else if (*it == '\n') {
                while (isspace(*it)) {
                    if (*it == '\n') { 
                        ++currentLineNumber;
                    }
                    ++it;
                }
                // tokens.push_back(Token(Token::terminator, "\\n", currentLineNumber));
                continue;
            }

            // ignore other whitespace
            ++it;
            continue;
        }

        // comment
        else if (*it == '#') {
            // ignore until next newline or end of input
            while (*it != '\r' && *it != '\n' && it != text.cend()) {
                ++it;
            }

            // text begins with a comment
            if (tokens.empty()) {
                while (isspace(*it)) {
                    if (*it == '\n') {
                        ++currentLineNumber;
                    }
                    ++it;
                }
                // tokens.push_back(Token(Token::terminator, "\\n", currentLineNumber));
            }

            // if the most recent token was a terminator, prevent another from being made after the comment by ignoring all whitespace
            // if (tokens.back() == Token::terminator) {
            //     while (isspace(*it)) {
            //         if (*it == '\n') {
            //             ++currentLineNumber;
            //         }
            //         ++it;
            //     }
            // }
        }

        // identifier or keyword
        else if (isalpha(*it) || *it == '_') {

            std::string::const_iterator wordEnd = it;
            while (isalnum(*wordEnd) || *wordEnd == '_') {        
                ++wordEnd;
            }
            std::string word = std::string(it, wordEnd);

            // identifier found if not in keyword map
            std::unordered_map<std::string, Token::Type>::const_iterator possibleKeyword = keywordMap.find(word);
            if (possibleKeyword == keywordMap.end()) {
                tokens.push_back(Token(Token::identifier, word, currentLineNumber));
            }
            // otherwise keyword
            else {
                tokens.push_back(Token(possibleKeyword->second, word, currentLineNumber));
            }
            it = wordEnd;

            continue;
        }

        // ->
        else if (*it == '-' && *(it+1) == '>') {
            tokens.push_back(Token(Token::arrowRight, "->", currentLineNumber));
            it += 2;
            continue;
        }

        // special case due to ambiguity between opMinus and a negative number
        // -
        else if (*it == '-' && isOpMinus(tokens.back())) {
            tokens.push_back(Token(Token::opMinus, "-", currentLineNumber));
            ++it;
            continue;
        }

        // numeric
        else if (isdigit(*it) || *it == '-') {
            if (*it == '-' && !isdigit(*(it+1))) {
                std::cout << "Lexer error on line " << currentLineNumber << ". '-' must be followed by a digit.\n";
                exit(1);
            }
            std::string::const_iterator numberEnd = it + 1;
            while (isdigit(*numberEnd))
                numberEnd++;
            
            // float
            if (*numberEnd == '.') {

                // float, b/c digit after .
                ++numberEnd;
                if (isdigit(*numberEnd)) {
                    
                    // expect only more digits
                    while (isdigit(*numberEnd))
                        numberEnd++;

                    std::string number(it, numberEnd);
                    tokens.push_back(Token(Token::floatLiteral, number, currentLineNumber));
                    it = numberEnd;
                }

                // otherwise, the '.' is a dot operator
                else {
                    --numberEnd; // exclude the '.'
                    std::string number(it, numberEnd);
                    tokens.push_back(Token(Token::intLiteral, number, currentLineNumber));
                    it = numberEnd;
                }
                continue;
            }

            // int
            std::string number(it, numberEnd);
            try {
                std::stoi(number);
            }
            catch (const std::out_of_range& e) {
                std::cout << "Lexer error on line " << currentLineNumber << ". The integer " << number << " out of 32 bit int range.\n";
                exit(1);
            }

            tokens.push_back(Token(Token::intLiteral, number, currentLineNumber));
            it = numberEnd;
            continue;
        }

        // " string
        else if (*it == '\"') {
            std::string::const_iterator string_end = it;
            string_end++; // consume "
            while (*string_end != '\"' && string_end != text.end()) {
                string_end++;
            }

            if (string_end == text.end()) {
                std::cout << "Tokenizer error. Unpaired \" on line " << currentLineNumber << ".\n";
                exit(1);
            }
            string_end++; // consume ending "
            
            // discard the quotes
            std::string string(it+1, string_end-1);
            tokens.push_back(Token(Token::charsLiteral, string, currentLineNumber));
            it = string_end;
        }

        // .
        else if (*it == '.') {
            tokens.push_back(Token(Token::dot, ".", currentLineNumber));
            ++it;
            continue;
        }

        // =, ==
        else if (*it == '=') {
            if (*(it+1) == '=') {
                tokens.push_back(Token(Token::opEquals, "==", currentLineNumber));
                it += 2;
                continue;
            }

            tokens.push_back(Token(Token::opAssign, "=", currentLineNumber));
            ++it;
            continue;
        }

        // +
        else if (*it == '+') {
            tokens.push_back(Token(Token::opPlus, "+", currentLineNumber));
            ++it;
            continue;
        }

        // *
        else if (*it == '*') {
            tokens.push_back(Token(Token::opMultiply, "*", currentLineNumber));
            ++it;
            continue;
        }

        // /
        else if (*it == '/') {
            tokens.push_back(Token(Token::opDivide, "/", currentLineNumber));
            ++it;
            continue;
        }

        // %
        else if (*it == '%') {
            tokens.push_back(Token(Token::opModulus, "%", currentLineNumber));
            ++it;
            continue;
        }

        // &, &&
        else if (*it == '&') {
            if (*(it+1) == '&') {
                tokens.push_back(Token(Token::opLogicalAnd, "&&", currentLineNumber));
                it += 2;
                continue;
            }
             
            tokens.push_back(Token(Token::opIntersect, "&", currentLineNumber));
            ++it;
            continue;
        }

        // |, ||
        else if (*it == '|') {
            if (*(it+1) == '|') {
                tokens.push_back(Token(Token::opLogicalOr, "||", currentLineNumber));
                it += 2;
                continue;
            }

            tokens.push_back(Token(Token::opUnion, "|", currentLineNumber));
            ++it;
            continue;
        }

        // ^
        else if (*it == '^') {
            tokens.push_back(Token(Token::opJoin, "^", currentLineNumber));
            ++it;
            continue;
        }

        // <, <=, <-
        else if (*it == '<') {
            if (*(it+1) == '=') {
                tokens.push_back(Token(Token::opLessThanOrEquals, "<=", currentLineNumber));
                it += 2;
                continue;
            }
            else if (*(it+1) == '-') {
                tokens.push_back(Token(Token::arrowLeft, "<-", currentLineNumber));
                it += 2;
                continue;
            }

            tokens.push_back(Token(Token::opLessThan, "<", currentLineNumber));
            ++it;
            continue;
        }

        // >, >=
        else if (*it == '>') {
            if (*(it+1) == '=') {
                tokens.push_back(Token(Token::opGreaterThanOrEquals, ">=", currentLineNumber));
                it += 2;
                continue;
            }

            tokens.push_back(Token(Token::opGreaterThan, ">", currentLineNumber));
            ++it;
            continue;
        }

        // ?
        else if (*it == '?') {
            tokens.push_back(Token(Token::questionMark, "?", currentLineNumber));
            ++it;
            continue;
        }

        // !, !=
        else if (*it == '!') {
            if (*(it+1) == '=') {
                tokens.push_back(Token(Token::opNotEquals, "!=", currentLineNumber));
                it += 2;
                continue;
            }

            tokens.push_back(Token(Token::exclamationPoint, "!", currentLineNumber));
            ++it;
            continue;
        }

        // ,
        else if (*it == ',') {
            tokens.push_back(Token(Token::comma, ",", currentLineNumber));
            ++it;
            continue;         
        }

        // (
        else if (*it == '(') {
            tokens.push_back(Token(Token::openParen, "(", currentLineNumber));
            ++it;
            continue;
        }

        // )
        else if (*it == ')') {
            tokens.push_back(Token(Token::closeParen, ")", currentLineNumber));
            ++it;
            continue;
        }

        // :, :=
        else if (*it == ':') {
            if (*(it+1) == '=') {
                tokens.push_back(Token(Token::opWalrus, ":=", currentLineNumber));
                it += 2;
                continue;
            }

            tokens.push_back(Token(Token::colon, ":", currentLineNumber));
            ++it;
            continue;
        }

        // ;
        else if (*it == ';') {
            tokens.push_back(Token(Token::semicolon, ";", currentLineNumber));
            ++it;
            continue;
        }

        // @
        else if (*it == '@') {
            tokens.push_back(Token(Token::at, "@", currentLineNumber));
            ++it;
            continue;
        }

        // ~
        else if (*it == '~') {
            tokens.push_back(Token(Token::tilde, "~", currentLineNumber));
            ++it;
            continue;
        }

        else {
            std::cout << "Internal lexer error: Invalid character \"" << *it << "\" on line " << currentLineNumber << ". Terminating.\n";
            exit(1);
        }
    }

    return tokens;
}
