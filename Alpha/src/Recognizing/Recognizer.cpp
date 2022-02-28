//
// Created by Alex Latz on 2/18/22.
//

#include "../../includes/Recognizing/Recognizer.h"

Recognizer::Recognizer(vector<Lexeme *>& lexemes) : lexemes(lexemes) {
    this->it = this->lexemes.begin();
}

void Recognizer::consume(TokenType type) {
    if (check(type) && it != lexemes.end()) it++;
    else Alpha::runtimeError((*it)->getLineNum(), "recognizing", "Expected " + tokenName[(*it)->getType()]);
}

bool Recognizer::check(TokenType type) {
    return (*it)->getType() == type;
}

bool Recognizer::checkNext(TokenType type) {
    return (it != lexemes.end()) && (*(it + 1))->getType();
}

TokenType Recognizer::peek() {
    return (*it)->getType();
}

bool Recognizer::statementListPending() {
    return statementPending();
}

bool Recognizer::statementPending() {
    return (variableInitPending() || assignmentPending() || fxnDeclarationPending()
        || fxnCallPending() || conditionalPending() || loopPending() || returnStatementPending())
        || commentPending();
}

bool Recognizer::variableInitPending() {
    return check(LET);
}

bool Recognizer::assignmentPending() {
    return check(IDENTIFIER) && checkNext(BE);
}

bool Recognizer::fxnDeclarationPending() {
    return check(FXN);
}

bool Recognizer::fxnCallPending() {
    return check(IDENTIFIER) && checkNext(OPEN_PAREN);
}

bool Recognizer::conditionalPending() {
    return conditionalOperatorPending();
}

bool Recognizer::loopPending() {
    return forLoopPending() || whileLoopPending();
}

bool Recognizer::returnStatementPending() {
    return check(RETURN);
}

bool Recognizer::commentPending() {
    return check(COMMENT_LINEEND);
}

bool Recognizer::conditionalOperatorPending() {
    return check(IF) || check(ELIF) || check(ELSE);
}

bool Recognizer::forLoopPending() {
    return check(FOR);
}

bool Recognizer::whileLoopPending() {
    return check(WHILE);
}

void Recognizer::program() {
    if (statementListPending()) {
        statementList();
    }
}

void Recognizer::statementList() {
    while (statementPending()) {
        statement();
    }
}

void Recognizer::statement() {
    if (!commentPending()) {
        if (variableInitPending()) variableInit();
        else if (assignmentPending()) assignment();
        else if (fxnDeclarationPending()) fxnDeclaration();
        else if (fxnCallPending()) fxnCall();
        else if (conditionalPending()) conditional();
        else if (loopPending()) loop();
        else if (returnStatementPending()) returnStatement();
        lineEnd();
        if (commentPending()) comment();
    } else comment();
}

void Recognizer::variableInit() {
    consume(LET);
    if (assignmentPending()) assignment();
    else if (check(IDENTIFIER)) {
        consume(IDENTIFIER);
        if (check(OPEN_SQ_BRACKET)) {
            consume(OPEN_SQ_BRACKET);
            consume(NUMBER);
            consume(CLOSE_SQ_BRACKET);
        }
    }
}

void Recognizer::assignment() {
    consume(IDENTIFIER);
    consume(BE);
    expression();
}

void Recognizer::fxnDeclaration() {
    consume(FXN);
    consume(IDENTIFIER);
    consume(OPEN_PAREN);
    parameters();
    consume(CLOSE_PAREN);
    block();
}

void Recognizer::fxnCall() {
    consume(IDENTIFIER);
    consume(OPEN_PAREN);
    parameters();
    consume(CLOSE_PAREN);
}

void Recognizer::conditional() {
    conditionalOperator();
    consume(OPEN_PAREN);
    condition();
    consume(CLOSE_PAREN);
    block();
}

void Recognizer::loop() {
    if (forLoopPending()) {
        consume(FOR);
        consume(OPEN_PAREN);
        variableInit();
        if (check(IN)) {
            consume(IN);
            consume(IDENTIFIER);
        } else if (check(LINE_END)) {
            consume(LINE_END);
            condition();
            consume(LINE_END);
            expression();
        }
        consume(CLOSE_PAREN)
    } else if (whileLoopPending()) {
        consume(WHILE);
        consume(OPEN_PAREN);
        condition();
        consume(CLOSE_PAREN);
    }
    block();
}

void Recognizer::returnStatement() {
    consume(RETURN);
    primary();
}

void Recognizer::lineEnd() {
    consume(LINE_END);
}

void Recognizer::comment() {
    consume(COMMENT_LINEEND);
}



