//
// Created by Alex Latz on 2/18/22.
//

#ifndef PROGRAMMINGLANGUAGES_PARSER_H
#define PROGRAMMINGLANGUAGES_PARSER_H

#include "../../includes/LexicalAnalysis/Lexeme.h"
#include "../../includes/Alpha.h"

class Parser {
    const vector<Lexeme*> lexemes;
    vector<Lexeme*>::const_iterator it;
    Lexeme* consume(TokenType type);
    Lexeme* consume();
    int getLineNum();
    bool check(TokenType type);
    bool checkNext(TokenType type);
    TokenType peek();
    TokenType peekNext();
    bool statementPending();
    bool statementListPending();
    bool variableInitPending();
    bool assignmentPending();
    bool fxnDeclarationPending();
    bool fxnCallPending();
    bool conditionalPending();
    bool loopPending();
    bool returnStatementPending();
    bool commentPending();
    bool lineEndPending();
    bool forLoopPending();
    bool whileLoopPending();
    bool primaryPending();
    bool collectionGetPending();
    bool parenthesizedExpressionPending();
    bool binaryExpressionPending();
    bool unaryExpressionPending();
    bool parameterPending();
    bool parameterDeclarePending();
    bool booleanOperandPending();
    bool booleanBinaryExpPending();
    bool booleanUnaryExpPending();
    bool comparisonPending();
    bool parenthesizedBooleanPending();
    Lexeme* statementList();
    Lexeme* statement();
    Lexeme* variableInit();
    Lexeme* assignment();
    Lexeme* fxnDeclaration();
    Lexeme* fxnCall();
    Lexeme* conditional();
    Lexeme* elifOrElse();
    Lexeme* loop();
    Lexeme* returnStatement();
    void lineEnd();
    void comment();
    Lexeme* expression();
    Lexeme* binaryExpression();
    Lexeme* unaryExpression();
    Lexeme* binaryOperator();
    Lexeme* unaryOperator();
    Lexeme* parameter();
    Lexeme* parameterDeclare();
    Lexeme* block();
    Lexeme* condition();
    Lexeme* booleanOperand();
    Lexeme* booleanBinaryExp();
    Lexeme* booleanUnaryExp();
    Lexeme* comparison();
    Lexeme* parenthesizedBoolean();
    Lexeme* booleanBinaryOperator();
    Lexeme* comparisonOperator();
    Lexeme* primary();
    Lexeme* collectionGet();
    Lexeme* parenthesizedExpression();
    bool isBinaryOperator(TokenType type);
    bool isComboBinaryOperator(TokenType type);
    bool isUnaryOperator(TokenType type);
    bool isBooleanBinaryOperator(TokenType type);
    bool isComparisonOperator(TokenType type);
    public:
        Parser(vector<Lexeme*>& lexemes);
        Lexeme* program();
};


#endif //PROGRAMMINGLANGUAGES_PARSER_H
