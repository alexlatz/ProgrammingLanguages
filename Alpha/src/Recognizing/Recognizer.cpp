//
// Created by Alex Latz on 2/18/22.
//

#include "../../includes/Recognizing/Recognizer.h"

Recognizer::Recognizer(vector<Lexeme *>& lexemes) : lexemes(lexemes) {
    this->it = this->lexemes.begin();
}

Lexeme* Recognizer::consume(TokenType type) {
    Lexeme* lex = *it;
    if (check(type) && it != lexemes.end()) it++;
    else Alpha::runtimeError((*it)->getLineNum(), "recognizing", "Expected " + tokenName[(*it)->getType()]);
    return lex;
}

Lexeme* Recognizer::consume() {
    Lexeme* lex = *it;
    if (it != lexemes.end()) it++;
    else Alpha::runtimeError((*it)->getLineNum(), "recognizing", "Expected lexeme");
    return lex;
}

bool Recognizer::check(TokenType type) {
    return (*it)->getType() == type;
}

bool Recognizer::checkNext(TokenType type) {
    return (it != lexemes.end()) && (*(it + 1))->getType() == type;
}

TokenType Recognizer::peek() {
    return (*it)->getType();
}

TokenType Recognizer::peekNext() {
    if (it != lexemes.end()) return (*(it + 1))->getType();
    else {
        Alpha::runtimeError(Lexeme(TokenType::ENDFILE, 0), "Expected additional lexeme");
        return ENDFILE;
    }
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
    return check(TokenType::LET);
}

bool Recognizer::assignmentPending() {
    return check(TokenType::IDENTIFIER) && checkNext(TokenType::BE);
}

bool Recognizer::fxnDeclarationPending() {
    return check(TokenType::FXN);
}

bool Recognizer::fxnCallPending() {
    return check(TokenType::IDENTIFIER) && checkNext(TokenType::OPEN_PAREN);
}

bool Recognizer::loopPending() {
    return forLoopPending() || whileLoopPending();
}

bool Recognizer::returnStatementPending() {
    return check(TokenType::RETURN);
}

bool Recognizer::commentPending() {
    return check(TokenType::COMMENT_LINEEND);
}

bool Recognizer::forLoopPending() {
    return check(TokenType::FOR);
}

bool Recognizer::whileLoopPending() {
    return check(TokenType::WHILE);
}

bool Recognizer::primaryPending() {
    return check(TokenType::BOOL) || check(TokenType::NUMBER) || check(TokenType::STRING) || check(TokenType::CHAR) || collectionGetPending() || parenthesizedExpressionPending() || check(TokenType::IDENTIFIER);
}

bool Recognizer::collectionGetPending() {
    return check(TokenType::IDENTIFIER) && checkNext(TokenType::OPEN_SQ_BRACKET);
}

bool Recognizer::parenthesizedExpressionPending() {
    //not looking one into the future because it would require
    //like 10 other functions for a minor issue that the recognizer would still catch
    return check(TokenType::OPEN_PAREN);
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
    return comparisonPending() || check(TokenType::BOOL) || parenthesizedBooleanPending();
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
    return check(TokenType::OPEN_PAREN);
}

bool Recognizer::booleanUnaryExpPending() {
    return check(TokenType::NOT);
}

bool Recognizer::isBinaryOperator(TokenType type) {
    return type == TokenType::BE || type == TokenType::ADD || type == TokenType::SUB || type == TokenType::X || type == TokenType::DIV
        || type == TokenType::MOD || type == TokenType::ADDBE || type == TokenType::SUBBE || type == TokenType::XBE || type == TokenType::DIVBE || type == TokenType::MODBE;
}

bool Recognizer::isUnaryOperator(TokenType type) {
    return type == TokenType::INC || type == TokenType::DEC;
}

bool Recognizer::isBooleanBinaryOperator(TokenType type) {
    return type == TokenType::AND || type == TokenType::OR;
}

bool Recognizer::isComparisonOperator(TokenType type) {
    return type == TokenType::LESS || type == TokenType::MORE || type == TokenType::IS || type == TokenType::LESSIS || type == TokenType::MOREIS;
}

Lexeme* Recognizer::program() {
    auto* statementList = new Lexeme(STATEMENTLIST, 0);
    if (statementListPending()) {
        Lexeme* lex = statementList();
        statementList->setChild(lex);
    }
    return statementList;
}

Lexeme* Recognizer::statementList() {
    Lexeme* statementList = new Lexeme(STATEMENTLIST, 0);
    while (statementPending()) {
        Lexeme* statement = statement();
        statementList->setChild(statement);
    }
    if (statementList->getChildrenLength() > 0) statementList->setLineNum(statementList->getChild(0)->getLineNum());
    return statementList;
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

Lexeme* Recognizer::variableInit() {
    Lexeme* let = consume(TokenType::LET);
    if (assignmentPending()) {
        Lexeme* be = assignment();
        let->setChild(be);
    }
    else if (check(TokenType::IDENTIFIER)) {
        let->setChild(consume(TokenType::IDENTIFIER));
        if (check(TokenType::OPEN_SQ_BRACKET)) {
            consume(TokenType::OPEN_SQ_BRACKET);
            let->setChild(consume(TokenType::NUMBER));
            consume(TokenType::CLOSE_SQ_BRACKET);
        }
    }
    return let;
}

Lexeme* Recognizer::assignment() {
    Lexeme* identifier = consume(TokenType::IDENTIFIER);
    Lexeme* be = consume(TokenType::BE);
    be->setChild(identifier);
    be->setChild(expression());
    return be;
}

Lexeme* Recognizer::fxnDeclaration() {
    Lexeme* fxn = consume(TokenType::FXN);
    fxn->setChild(consume(TokenType::IDENTIFIER));
    consume(TokenType::OPEN_PAREN);
    fxn->setChild(parameter());
    consume(TokenType::CLOSE_PAREN);
    fxn->setChild(block());
    return fxn;
}

Lexeme* Recognizer::fxnCall() {
    Lexeme* id = consume(TokenType::IDENTIFIER);
    Lexeme* call = new Lexeme(FXN_CALL, id->getLineNum());
    call->setChild(id);
    consume(TokenType::OPEN_PAREN);
    call->setChild(parameter());
    consume(TokenType::CLOSE_PAREN);
    return call;
}

Lexeme* Recognizer::conditional() {
    Lexeme* op = consume(IF);
    consume(TokenType::OPEN_PAREN);
    op->setChild(condition());
    consume(TokenType::CLOSE_PAREN);
    op->setChild(block());
    if (check(TokenType::ELIF) || check(TokenType::ELSE)) op->setChild(elifOrElse());
    return op;
}

Lexeme* Recognize::elifOrElse() {
    Lexeme* op;
    if (check(TokenType::ELSE)) {
        op = consume(TokenType::ELSE);
        op->setChild(block());
    }
    else {
        op = consume(TokenType::ELIF);
        consume(TokenType::OPEN_PAREN);
        op->setChild(condition());
        consume(TokenType::CLOSE_PAREN);
        op->setChild(block());
        op->setChild(elifOrElse());
    }
    return op;
}

void Recognizer::loop() {
    if (forLoopPending()) {
        consume(TokenType::FOR);
        consume(TokenType::OPEN_PAREN);
        variableInit();
        if (check(TokenType::IN)) {
            consume(TokenType::IN);
            consume(TokenType::IDENTIFIER);
        } else if (check(TokenType::LINE_END)) {
            consume(TokenType::LINE_END);
            condition();
            consume(TokenType::LINE_END);
            expression();
        }
        consume(TokenType::CLOSE_PAREN);
    } else if (whileLoopPending()) {
        consume(TokenType::WHILE);
        consume(TokenType::OPEN_PAREN);
        condition();
        consume(TokenType::CLOSE_PAREN);
    }
    block();
}

void Recognizer::returnStatement() {
    consume(TokenType::RETURN);
    primary();
}

void Recognizer::lineEnd() {
    consume(TokenType::LINE_END);
}

void Recognizer::comment() {
    consume(TokenType::COMMENT_LINEEND);
}

void Recognizer::expression() {
    if (binaryExpressionPending()) binaryExpression();
    else if (unaryExpressionPending()) unaryExpression();
    else if (primaryPending()) primary();
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
    consume(TokenType::OPEN_BLOCK);
    if (statementListPending()) statementList();
    consume(TokenType::CLOSE_BLOCK);
}

void Recognizer::condition() {
    if (booleanOperandPending()) booleanOperand();
    else if (booleanBinaryExpPending()) booleanBinaryExp();
    else if (booleanUnaryExpPending()) booleanUnaryExp();
}

void Recognizer::booleanOperand() {
    if (comparisonPending()) comparison();
    else if (check(TokenType::BOOL)) consume(TokenType::BOOL);
    else if (parenthesizedBooleanPending()) parenthesizedBoolean();
}

void Recognizer::booleanBinaryExp() {
    booleanOperand();
    booleanBinaryOperator();
    booleanOperand();
}

void Recognizer::booleanUnaryExp() {
    consume(TokenType::NOT);
    booleanOperand();
}

void Recognizer::comparison() {
    primary();
    comparisonOperator();
    primary();
}

void Recognizer::parenthesizedBoolean() {
    consume(TokenType::OPEN_PAREN);
    condition();
    consume(TokenType::CLOSE_PAREN);
}

void Recognizer::booleanBinaryOperator() {
    if (check(TokenType::AND)) consume(TokenType::AND);
    else if (check(TokenType::OR)) consume(TokenType::OR);
}

void Recognizer::comparisonOperator() {
    if (isComparisonOperator(peek())) consume();
}

void Recognizer::primary() {
    if (check(TokenType::NUMBER)) consume(TokenType::NUMBER);
    else if (check(TokenType::BOOL)) consume(TokenType::BOOL);
    else if (check(TokenType::STRING)) consume(TokenType::STRING);
    else if (check(TokenType::CHAR)) consume(TokenType::CHAR);
    else if (collectionGetPending()) collectionGet();
    else if (parenthesizedExpressionPending()) parenthesizedExpression();
    else if (check(TokenType::IDENTIFIER)) consume(TokenType::IDENTIFIER);
}

void Recognizer::collectionGet() {
    primary();
    consume(TokenType::OPEN_SQ_BRACKET);
    consume(TokenType::NUMBER);
    consume(TokenType::CLOSE_SQ_BRACKET);
}

void Recognizer::parenthesizedExpression() {
    consume(TokenType::OPEN_PAREN);
    expression();
    consume(TokenType::CLOSE_PAREN);
}



