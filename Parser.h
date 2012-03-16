#ifndef _PARSER_H_
#define _PARSER_H_

#include "Tree.h"
#include "ParseException.h"
#include "LexicalAnalyzer.h"

class Parser {
public:
    Parser();
    ~Parser();
    Tree* parse(std::string const& input);
private:
    Tree* E();
    Tree* EPrime();
    
    LexicalAnalyzer* lex_;
};

#endif //_PARSER_H_
