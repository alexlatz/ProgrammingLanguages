//
// Created by Alex Latz on 2/18/22.
//

#ifndef PROGRAMMINGLANGUAGES_RECOGNIZER_H
#define PROGRAMMINGLANGUAGES_RECOGNIZER_H

#include "../../includes/Alpha.h"

class Recognizer {
    const vector<Lexeme*> lexemes;
    vector<Lexeme*>::const_iterator it;
    void consume(TokenType type);
    bool check(TokenType type);
    bool checkNext(TokenType type);
    TokenType peek();
    bool statementPending();
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
    void program();
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
    public:
        Recognizer(vector<Lexeme*>& lexemes);
        bool statementListPending();

};


#endif //PROGRAMMINGLANGUAGES_RECOGNIZER_H
