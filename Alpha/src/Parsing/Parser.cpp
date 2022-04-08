//
// Created by Alex Latz on 2/18/22.
//

#include "../../includes/Parsing/Parser.h"

Parser::Parser(vector<Lexeme *>& lexemes) : lexemes(lexemes) {
    this->it = this->lexemes.begin();
}

Lexeme* Parser::consume(TokenType type) {
    Lexeme* lex = *it;
    if (check(type) && it != lexemes.end()) it++;
    else Alpha::runtimeError((*it)->getLineNum(), "recognizing", "Expected " + tokenName[(*it)->getType()]);
    return lex;
}

Lexeme* Parser::consume() {
    Lexeme* lex = *it;
    if (it != lexemes.end()) it++;
    else Alpha::runtimeError((*it)->getLineNum(), "recognizing", "Expected lexeme");
    return lex;
}

int Parser::getLineNum() {
    return (*it)->getLineNum();
}

bool Parser::check(TokenType type) {
    return (*it)->getType() == type;
}

bool Parser::checkNext(TokenType type) {
    return (it != lexemes.end()) && (*(it + 1))->getType() == type;
}

TokenType Parser::peek() {
    return (*it)->getType();
}

TokenType Parser::peekNext() {
    if (it != lexemes.end()) return (*(it + 1))->getType();
    else {
        Alpha::runtimeError(getLineNum(), "Parsing: end of file", "Expected additional lexeme");
        return TokenType::ENDFILE;
    }
}

bool Parser::statementListPending() {
    return statementPending();
}

bool Parser::statementPending() {
    return (variableInitPending() || assignmentPending() || fxnDeclarationPending()
        || fxnCallPending() || conditionalPending() || loopPending() || returnStatementPending())
        || commentPending() || lineEndPending();
}

bool Parser::variableInitPending() {
    return check(TokenType::LET);
}

bool Parser::assignmentPending() {
    return check(TokenType::IDENTIFIER) && checkNext(TokenType::BE);
}

bool Parser::fxnDeclarationPending() {
    return check(TokenType::FXN);
}

bool Parser::fxnCallPending() {
    return check(TokenType::IDENTIFIER) && checkNext(TokenType::OPEN_PAREN);
}

bool Parser::conditionalPending() {
    return check(TokenType::IF);
}

bool Parser::loopPending() {
    return forLoopPending() || whileLoopPending();
}

bool Parser::returnStatementPending() {
    return check(TokenType::RETURN);
}

bool Parser::commentPending() {
    return check(TokenType::COMMENT_LINEEND);
}

bool Parser::lineEndPending() {
    return check(TokenType::LINE_END);
}

bool Parser::forLoopPending() {
    return check(TokenType::FOR);
}

bool Parser::whileLoopPending() {
    return check(TokenType::WHILE);
}

bool Parser::primaryPending() {
    return check(TokenType::BOOL) || check(TokenType::NUMBER) || check(TokenType::STRING) || check(TokenType::CHAR) || collectionGetPending() || parenthesizedExpressionPending() || check(TokenType::IDENTIFIER);
}

bool Parser::collectionGetPending() {
    return check(TokenType::IDENTIFIER) && checkNext(TokenType::OPEN_SQ_BRACKET);
}

bool Parser::parenthesizedExpressionPending() {
    //not looking one into the future because it would require
    //like 10 other functions for a minor issue that the recognizer would still catch
    return check(TokenType::OPEN_PAREN);
}

bool Parser::binaryExpressionPending() {
    return primaryPending() && isBinaryOperator(peekNext());
}

bool Parser::unaryExpressionPending() {
    return primaryPending() && isUnaryOperator(peekNext());
}

bool Parser::parameterPending() {
    return !binaryExpressionPending() && !unaryExpressionPending() && primaryPending();
}

bool Parser::booleanOperandPending() {
    return comparisonPending() || check(TokenType::BOOL) || check(TokenType::IDENTIFIER) || booleanUnaryExpPending() || parenthesizedBooleanPending();
}

bool Parser::booleanBinaryExpPending() {
    return booleanOperandPending() && isBooleanBinaryOperator(peekNext());
}

bool Parser::comparisonPending() {
    return primaryPending() && isComparisonOperator(peekNext());
}

bool Parser::parenthesizedBooleanPending() {
    //not looking one into the future because it would require
    //like 10 other functions for a minor issue that the recognizer would still catch
    return check(TokenType::OPEN_PAREN);
}

bool Parser::booleanUnaryExpPending() {
    return check(TokenType::NOT);
}

bool Parser::isBinaryOperator(TokenType type) {
    return type == TokenType::BE || type == TokenType::ADD || type == TokenType::SUB || type == TokenType::X || type == TokenType::DIV
        || type == TokenType::MOD || type == TokenType::ADDBE || type == TokenType::SUBBE || type == TokenType::XBE || type == TokenType::DIVBE || type == TokenType::MODBE;
}

bool Parser::isComboBinaryOperator(TokenType type) {
    return type == TokenType::ADDBE || type == TokenType::SUBBE || type == TokenType::XBE || type == TokenType::DIVBE || type == TokenType::MODBE;
}

bool Parser::isUnaryOperator(TokenType type) {
    return type == TokenType::INC || type == TokenType::DEC;
}

bool Parser::isBooleanBinaryOperator(TokenType type) {
    return type == TokenType::AND || type == TokenType::OR;
}

bool Parser::isComparisonOperator(TokenType type) {
    return type == TokenType::LESS || type == TokenType::MORE || type == TokenType::IS || type == TokenType::LESSIS || type == TokenType::MOREIS;
}

Lexeme* Parser::program() {
    Lexeme* stateList = nullptr;
    if (statementListPending()) {
        stateList = statementList();
    }
    if (stateList != nullptr) return stateList;
    else return new Lexeme(TokenType::STATEMENTLIST, 0);
}

Lexeme* Parser::statementList() {
    Lexeme* statementList = new Lexeme(TokenType::STATEMENTLIST, 0);
    while (statementPending()) {
        Lexeme* state = statement();
        if (state != nullptr) statementList->setChild(state);
    }
    if (statementList->getChildrenLength() > 0) statementList->setLineNum(statementList->getChild(0)->getLineNum());
    return statementList;
}

Lexeme* Parser::statement() {
    if (variableInitPending() || assignmentPending() || fxnDeclarationPending() || fxnCallPending() ||
        conditionalPending() || loopPending() || returnStatementPending()) {
        Lexeme *statement = nullptr;
        if (variableInitPending()) statement = variableInit();
        else if (assignmentPending()) statement = assignment();
        else if (fxnDeclarationPending()) statement = fxnDeclaration();
        else if (fxnCallPending()) statement = fxnCall();
        else if (conditionalPending()) statement = conditional();
        else if (loopPending()) statement = loop();
        else if (returnStatementPending()) statement = returnStatement();
        if (!conditionalPending() && !loopPending()) lineEnd();
        if (commentPending()) comment();
        return statement;
    } else if (commentPending()) comment();
    else if (lineEndPending()) lineEnd();
    else Alpha::runtimeError(getLineNum(), "Parsing statement", "statement expected");
    return nullptr;
}

Lexeme* Parser::variableInit() {
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

Lexeme* Parser::assignment() {
    Lexeme* identifier = consume(TokenType::IDENTIFIER);
    Lexeme* be = consume(TokenType::BE);
    be->setChild(identifier);
    be->setChild(expression());
    return be;
}

Lexeme* Parser::fxnDeclaration() {
    Lexeme* fxn = consume(TokenType::FXN);
    fxn->setChild(consume(TokenType::IDENTIFIER));
    consume(TokenType::OPEN_PAREN);
    fxn->setChild(parameter());
    consume(TokenType::CLOSE_PAREN);
    fxn->setChild(block());
    return fxn;
}

Lexeme* Parser::fxnCall() {
    Lexeme* id = consume(TokenType::IDENTIFIER);
    consume(TokenType::OPEN_PAREN);
    id->setChild(parameter());
    consume(TokenType::CLOSE_PAREN);
    return id;
}

Lexeme* Parser::conditional() {
    Lexeme* op = consume(IF);
    consume(TokenType::OPEN_PAREN);
    op->setChild(condition());
    consume(TokenType::CLOSE_PAREN);
    op->setChild(block());
    if (check(TokenType::ELIF) || check(TokenType::ELSE)) {
        Lexeme* ending = elifOrElse();
        if (ending != nullptr) op->setChild(elifOrElse());
    }
    return op;
}

Lexeme* Parser::elifOrElse() {
    Lexeme* op = nullptr;
    if (check(TokenType::ELSE)) {
        op = consume(TokenType::ELSE);
        op->setChild(block());
    }
    else if (check(TokenType::ELIF)) {
        op = consume(TokenType::ELIF);
        consume(TokenType::OPEN_PAREN);
        op->setChild(condition());
        consume(TokenType::CLOSE_PAREN);
        op->setChild(block());
        Lexeme* ending = elifOrElse();
        if (ending != nullptr) op->setChild(ending);
    }
    return op;
}

Lexeme* Parser::loop() {
    Lexeme* root = nullptr;
    if (forLoopPending()) {
        root = consume(TokenType::FOR);
        consume(TokenType::OPEN_PAREN);
        root->setChild(variableInit());
        if (check(TokenType::IN)) {
            consume(TokenType::IN);
            root->setChild(consume(TokenType::IDENTIFIER));
        } else if (lineEndPending()) {
            lineEnd();
            root->setChild(condition());
            lineEnd();
            root->setChild(expression());
        }
        consume(TokenType::CLOSE_PAREN);
    } else if (whileLoopPending()) {
        root = consume(TokenType::WHILE);
        consume(TokenType::OPEN_PAREN);
        root->setChild(condition());
        consume(TokenType::CLOSE_PAREN);
    } else Alpha::runtimeError(getLineNum(), "Parsing loop", "While or For expected");
    if (root != nullptr) root->setChild(block());
    return root;
}

Lexeme* Parser::returnStatement() {
    Lexeme* rtrn = consume(TokenType::RETURN);
    rtrn->setChild(primary());
    return rtrn;
}

void Parser::lineEnd() {
    consume(TokenType::LINE_END);
}

void Parser::comment() {
    consume(TokenType::COMMENT_LINEEND);
}

Lexeme* Parser::expression() {
    if (binaryExpressionPending()) return binaryExpression();
    else if (unaryExpressionPending()) return unaryExpression();
    else if (primaryPending()) return primary();
    else Alpha::runtimeError(getLineNum(), "Parsing expression", "Binary expression, unary expression, or primary expected");
    return nullptr;
}

Lexeme* Parser::binaryExpression() {
    Lexeme* first = primary();
    Lexeme* op = binaryOperator();
    op->setChild(first);
    op->setChild(primary());
    if (isComboBinaryOperator(op->getType()) && first->getType() != TokenType::IDENTIFIER) Alpha::runtimeError(getLineNum(), "Parsing binary expression", "Combo binary operator must have an identifier as the first operand");
    else if (isComboBinaryOperator(op->getType())) {
        Lexeme* be = new Lexeme(TokenType::BE, getLineNum());
        be->setChild(first);
        be->setChild(op);
        return be;
    }
    return op;
}

Lexeme* Parser::unaryExpression() {
    Lexeme* first = primary();
    Lexeme* op = unaryOperator();
    op->setChild(first);
    return op;
}

Lexeme* Parser::binaryOperator() {
    if (isBinaryOperator(peek())) return consume();
    Alpha::runtimeError(getLineNum(), "Parsing Binary Operator", "Expected binary operator");
    return nullptr;
}

Lexeme* Parser::unaryOperator() {
    if (isUnaryOperator(peek())) return consume();
    Alpha::runtimeError(getLineNum(), "Parsing Unary Operator", "Expected unary operator");
    return nullptr;
}

Lexeme* Parser::parameter() {
    auto* list = new Lexeme(TokenType::PARAMETERLIST, 0);
    while (parameterPending()) {
        list->setChild(primary());
    }
    return list;
}

Lexeme* Parser::block() {
    Lexeme* lex = nullptr;
    consume(TokenType::OPEN_BLOCK);
    if (statementListPending()) lex = statementList();
    consume(TokenType::CLOSE_BLOCK);
    return lex;
}

Lexeme* Parser::condition() {
    if (booleanUnaryExpPending()) return booleanUnaryExp();
    else if (booleanBinaryExpPending()) return booleanBinaryExp();
    else if (booleanOperandPending()) return booleanOperand();
    Alpha::runtimeError(getLineNum(), "Parsing Condition", "Expected condition");
    return nullptr;
}

Lexeme* Parser::booleanOperand() {
    if (comparisonPending()) return comparison();
    else if (check(TokenType::BOOL)) return consume(TokenType::BOOL);
    else if (check(TokenType::IDENTIFIER)) return consume(TokenType::IDENTIFIER);
    else if (parenthesizedBooleanPending()) return parenthesizedBoolean();
    else if (booleanUnaryExpPending()) return booleanUnaryExp();
    Alpha::runtimeError(getLineNum(), "Parsing Boolean Operand", "Expected Comparison, boolean, or parenthesized boolean.");
    return nullptr;
}

Lexeme* Parser::booleanBinaryExp() {
    Lexeme* first = booleanOperand();
    Lexeme* op = booleanBinaryOperator();
    op->setChild(first);
    op->setChild(booleanOperand());
    return op;
}

Lexeme* Parser::booleanUnaryExp() {
    Lexeme* op = consume(TokenType::NOT);
    op->setChild(booleanOperand());
    return op;
}

Lexeme* Parser::comparison() {
    Lexeme* first = primary();
    Lexeme* op = comparisonOperator();
    op->setChild(first);
    op->setChild(primary());
    return op;
}

Lexeme* Parser::parenthesizedBoolean() {
    consume(TokenType::OPEN_PAREN);
    Lexeme* lex = condition();
    consume(TokenType::CLOSE_PAREN);
    return lex;
}

Lexeme* Parser::booleanBinaryOperator() {
    if (check(TokenType::AND)) return consume(TokenType::AND);
    else if (check(TokenType::OR)) return consume(TokenType::OR);
    Alpha::runtimeError(getLineNum(), "Parsing Boolean Binary Operator", "Expected AND or OR");
    return nullptr;
}

Lexeme* Parser::comparisonOperator() {
    if (isComparisonOperator(peek())) return consume();
    Alpha::runtimeError(getLineNum(), "Parsing Comparison Operator", "Expected comparison operator");
    return nullptr;
}

Lexeme* Parser::primary() {
    if (check(TokenType::NUMBER)) return consume(TokenType::NUMBER);
    else if (check(TokenType::BOOL)) return consume(TokenType::BOOL);
    else if (check(TokenType::STRING)) return consume(TokenType::STRING);
    else if (check(TokenType::CHAR)) return consume(TokenType::CHAR);
    else if (collectionGetPending()) return collectionGet();
    else if (parenthesizedExpressionPending()) return parenthesizedExpression();
    else if (check(TokenType::IDENTIFIER)) return consume(TokenType::IDENTIFIER);
    Alpha::runtimeError(getLineNum(), "Parsing Primary", "Expected primary value");
    return nullptr;
}

Lexeme* Parser::collectionGet() {
    Lexeme* collection = consume(TokenType::IDENTIFIER);
    consume(TokenType::OPEN_SQ_BRACKET);
    collection->setChild(consume(TokenType::NUMBER));
    consume(TokenType::CLOSE_SQ_BRACKET);
    return collection;
}

Lexeme* Parser::parenthesizedExpression() {
    consume(TokenType::OPEN_PAREN);
    Lexeme* lex =  expression();
    consume(TokenType::CLOSE_PAREN);
    return lex;
}



