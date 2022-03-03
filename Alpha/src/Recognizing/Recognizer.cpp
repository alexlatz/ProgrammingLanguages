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

void Recognizer::consume() {
    if (it != lexemes.end()) it++;
    else Alpha::runtimeError((*it)->getLineNum(), "recognizing", "Expected lexeme");
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

TokenType Recognizer::peekNext() {
    return (it != lexemes.end()) ? ENDFILE : (*(it + 1))->getType();
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

bool Recognizer::primaryPending() {
    return check(BOOL) || check(NUMBER) || check(STRING) || check(CHAR) || collectionGetPending() || parenthesizedExpressionPending();
}

bool Recognizer::collectionGetPending() {
    return check(IDENTIFIER) && checkNext(OPEN_SQ_BRACKET);
}

bool Recognizer::parenthesizedExpressionPending() {
    //not looking one into the future because it would require
    //like 10 other functions for a minor issue that the recognizer would still catch
    return check(OPEN_PAREN);
}

bool Recognizer::binaryExpressionPending() {
    return primaryPending() && isBinaryOperator(peekNext());
}

bool Recognizer::unaryExpressionPending() {
    return primaryPending() && isUnaryOperator(peekNext());
}

bool Recognizer::parameterPending() {
    return !binaryExpressionPending() && !unaryExpressionPending() && primaryPending();
}

bool Recognizer::booleanOperandPending() {
    return comparisonPending() || check(BOOL) || parenthesizedBooleanPending();
}

bool Recognizer::booleanBinaryExpPending() {
    return booleanOperandPending() && isBooleanBinaryOperator(peekNext());
}

bool Recognizer::comparisonPending() {
    return primaryPending() && isComparisonOperator(peekNext());
}

bool Recognizer::parenthesizedBooleanPending() {
    //not looking one into the future because it would require
    //like 10 other functions for a minor issue that the recognizer would still catch
    return check(OPEN_PAREN);
}

bool Recognizer::booleanUnaryExpPending() {
    return check(NOT);
}

bool Recognizer::isBinaryOperator(TokenType type) {
    return type == BE || type == ADD || type == SUB || type == X || type == DIV
        || type == MOD || type == ADDBE || type == SUBBE || type == XBE || type == DIVBE || type == MODBE;
}

bool Recognizer::isUnaryOperator(TokenType type) {
    return type == INC || type == DEC;
}

bool Recognizer::isBooleanBinaryOperator(TokenType type) {
    return type == AND || type == OR;
}

bool Recognizer::isComparisonOperator(TokenType type) {
    return type == LESS || type == MORE || type == IS || type == LESSIS || type == MOREIS;
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
    if (variableInitPending() || assignmentPending() || fxnDeclarationPending() || fxnCallPending() || conditionalPending() || loopPending() || returnStatementPending()) {
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
    parameter();
    consume(CLOSE_PAREN);
    block();
}

void Recognizer::fxnCall() {
    consume(IDENTIFIER);
    consume(OPEN_PAREN);
    parameter();
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
        consume(CLOSE_PAREN);
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

void Recognizer::expression() {
    if (primaryPending()) primary();
    else if (binaryExpressionPending()) binaryExpression();
    else if (unaryExpressionPending()) unaryExpression();
}

void Recognizer::binaryExpression() {
    primary();
    binaryOperator();
    primary();
}

void Recognizer::unaryExpression() {
    primary();
    unaryOperator();
}

void Recognizer::binaryOperator() {
    if (isBinaryOperator(peek())) consume();
}

void Recognizer::unaryOperator() {
    if (isUnaryOperator(peek())) consume();
}

void Recognizer::parameter() {
    if (parameterPending()) {
        primary();
        parameter();
    }
}

void Recognizer::block() {
    consume(OPEN_BLOCK);
    if (statementListPending()) statementList();
    consume(CLOSE_BLOCK);
}

void Recognizer::conditionalOperator() {
    if (check(IF)) consume(IF);
    else if (check(ELIF)) consume(ELIF);
    else if (check(ELSE)) consume(ELSE);
}

void Recognizer::condition() {
    if (booleanOperandPending()) booleanOperand();
    else if (booleanBinaryExpPending()) booleanBinaryExp();
    else if (booleanUnaryExpPending()) booleanUnaryExp();
}

void Recognizer::booleanOperand() {
    if (comparisonPending()) comparison();
    else if (check(BOOL)) consume(BOOL);
    else if (parenthesizedBooleanPending()) parenthesizedBoolean();
}

void Recognizer::booleanBinaryExp() {
    booleanOperand();
    booleanBinaryOperator();
    booleanOperand();
}

void Recognizer::booleanUnaryExp() {
    consume(NOT);
    booleanOperand();
}

void Recognizer::comparison() {
    primary();
    comparisonOperator();
    primary();
}

void Recognizer::parenthesizedBoolean() {
    consume(OPEN_PAREN);
    condition();
    consume(CLOSE_PAREN);
}

void Recognizer::booleanBinaryOperator() {
    if (check(AND)) consume(AND);
    else if (check(OR)) consume(OR);
}

void Recognizer::comparisonOperator() {
    if (isComparisonOperator(peek())) consume();
}

void Recognizer::primary() {
    if (check(NUMBER)) consume(NUMBER);
    else if (check(BOOL)) consume(BOOL);
    else if (check(STRING)) consume(STRING);
    else if (check(CHAR)) consume(CHAR);
    else if (collectionGetPending()) collectionGet();
    else if (parenthesizedExpressionPending()) parenthesizedExpression();
    else if (check(IDENTIFIER)) consume(IDENTIFIER);
}

void Recognizer::collectionGet() {
    primary();
    consume(OPEN_SQ_BRACKET);
    consume(NUMBER);
    consume(CLOSE_SQ_BRACKET);
}

void Recognizer::parenthesizedExpression() {
    consume(OPEN_PAREN);
    expression();
    consume(CLOSE_PAREN);
}



