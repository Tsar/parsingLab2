#include "Parser.h"

Parser::Parser()
    : lex_(0) {
}

Parser::~Parser() {
    if (lex_)
        delete lex_;
}

Tree* Parser::parse(std::string const& input) {
    if (lex_)
        delete lex_;
    lex_ = new LexicalAnalyzer(input);
    lex_->nextToken();
    return E();
}

Tree* Parser::E() {
    Tree* res = new Tree("E");
    switch (lex_->curToken()) {
        case NUMBER:
            //n
            res->addChild(new Tree("n"));
            lex_->nextToken();
            //E'
            res->addChild(EPrime());
            break;
        default:
            throw new ParseException("Unexpected token at position", lex_->curPos());
    }
    return res;
}

Tree* Parser::EPrime() {
    Tree* res = new Tree("E'");
    switch (lex_->curToken()) {
        case NUMBER:
            //E
            res->addChild(E());
            lex_->nextToken();
            //o
            if (lex_->curToken() != OPERATOR)
                throw new ParseException("Operator expected at position", lex_->curPos());
            res->addChild(new Tree("o"));
            lex_->nextToken();
            //E'
            res->addChild(EPrime());
            break;
        case END:
            //eps
            break;
        case OPERATOR:
        default:
            throw new ParseException("Unexpected token at position", lex_->curPos());
    }
    return res;
}
