#ifndef _LEXICAL_ANALYZER_H_
#define _LEXICAL_ANALYZER_H_

#include <string>

enum Token {
    NUMBER,
    OPERATOR,
    END
};

class LexicalAnalyzer {
public:
    LexicalAnalyzer(std::string const& input);
    void nextToken();
    Token curToken() const;
    int curPos() const;
private:
    bool isBlank(char c) const;
    bool isDigit(char c) const;
    void skipDigits();
    void nextChar();

    std::string input_;
    char curChar_;
    int curPos_;
    Token curToken_;
};

#endif //_LEXICAL_ANALYZER_H_
