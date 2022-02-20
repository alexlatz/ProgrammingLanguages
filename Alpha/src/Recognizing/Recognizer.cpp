//
// Created by Alex Latz on 2/18/22.
//

#include "../../includes/Recognizing/Recognizer.h"

Recognizer::Recognizer(vector<Lexeme *>& lexemes) : lexemes(lexemes) {
    this->it = lexemes.begin();
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


