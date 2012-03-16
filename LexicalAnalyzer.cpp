#include "LexicalAnalyzer.h"

#include "ParseException.h"

LexicalAnalyzer::LexicalAnalyzer(std::string const& input)
    : input_(input + "$")
    , curPos_(0) {
    nextChar();
}

void LexicalAnalyzer::nextToken() {
    while (isBlank(curChar_))
        nextChar();
    if (isDigit(curChar_)) {
        curToken_ = NUMBER;
        skipDigits();
    }
    switch (curChar_) {
        case '+':
            nextChar();
            curToken_ = OPERATOR;
            break;
        case '*':
            nextChar();
            curToken_ = OPERATOR;
            break;
        case '-':
            nextChar();
            if (isDigit(curChar_)) {
                curToken_ = NUMBER;
                skipDigits();
            } else {
                curToken_ = OPERATOR;
            }
            break;
        case '$':
            curToken_ = END;
            break;
        default:
            throw new ParseException("Illegal character " + curChar_, curPos_);
    }
}

Token LexicalAnalyzer::curToken() const {
    return curToken_;
}

int LexicalAnalyzer::curPos() const {
    return curPos_;
}

bool LexicalAnalyzer::isBlank(char c) const {
    return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

bool LexicalAnalyzer::isDigit(char c) const {
    return '0' <= c && c <= '9';
}

void LexicalAnalyzer::skipDigits() {
    while (isDigit(curChar_))
        nextChar();
}

void LexicalAnalyzer::nextChar() {
    curChar_ = input[curPos_++];
}
