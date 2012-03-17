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
    Tree* res = E();
    if (lex_->curToken() != END)
        throw ParseException("Unexpected token at position", lex_->curPos() - 1);
    return res;
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
            throw ParseException("Unexpected token at position", lex_->curPos() - 1);
    }
    return res;
}

Tree* Parser::EPrime() {
    Tree* res = new Tree("E'");
    switch (lex_->curToken()) {
        case NUMBER:
            //E
            res->addChild(E());
            //o
            if (lex_->curToken() != OPERATOR)
                throw ParseException("Operator expected at position", lex_->curPos() - 1);
            res->addChild(new Tree("o"));
            lex_->nextToken();
            //E'
            res->addChild(EPrime());
            break;
        case OPERATOR:
        case END:
            res->addChild(new Tree("eps"));
            break;
        default:
            throw ParseException("Unexpected token at position", lex_->curPos() - 1);
    }
    return res;
}
