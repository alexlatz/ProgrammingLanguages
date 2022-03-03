//
// Created by Alex Latz on 2/18/22.
//

#ifndef PROGRAMMINGLANGUAGES_RECOGNIZER_H
#define PROGRAMMINGLANGUAGES_RECOGNIZER_H

#include "../../includes/LexicalAnalysis/Lexeme.h"
#include "../../includes/Alpha.h"

class Recognizer {
    const vector<Lexeme*> lexemes;
    vector<Lexeme*>::const_iterator it;
    void consume(TokenType type);
    void consume();
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
    bool conditionalOperatorPending();
    bool forLoopPending();
    bool whileLoopPending();
    bool primaryPending();
    bool collectionGetPending();
    bool parenthesizedExpressionPending();
    bool binaryExpressionPending();
    bool unaryExpressionPending();
    bool parameterPending();
    bool booleanOperandPending();
    bool booleanBinaryExpPending();
    bool booleanUnaryExpPending();
    bool comparisonPending();
    bool parenthesizedBooleanPending();
    void statementList();
    void statement();
    void variableInit();
    void assignment();
    void fxnDeclaration();
    void fxnCall();
    void conditional();
    void loop();
    void returnStatement();
    void lineEnd();
    void comment();
    void expression();
    void binaryExpression();
    void unaryExpression();
    void binaryOperator();
    void unaryOperator();
    void parameter();
    void block();
    void conditionalOperator();
    void condition();
    void booleanOperand();
    void booleanBinaryExp();
    void booleanUnaryExp();
    void comparison();
    void parenthesizedBoolean();
    void booleanBinaryOperator();
    void comparisonOperator();
    void primary();
    void collectionGet();
    void parenthesizedExpression();
    bool isBinaryOperator(TokenType type);
    bool isUnaryOperator(TokenType type);
    bool isBooleanBinaryOperator(TokenType type);
    bool isComparisonOperator(TokenType type);
    public:
        Recognizer(vector<Lexeme*>& lexemes);
        void program();
};


#endif //PROGRAMMINGLANGUAGES_RECOGNIZER_H
