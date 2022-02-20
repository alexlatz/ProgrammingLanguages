//
// Created by Alex Latz on 2/18/22.
//

#ifndef PROGRAMMINGLANGUAGES_RECOGNIZER_H
#define PROGRAMMINGLANGUAGES_RECOGNIZER_H

#include "../../includes/Alpha.h"

class Recognizer {
    const vector<Lexeme*> lexemes;
    vector<Lexeme*>::iterator it;
    void consume(TokenType type);
    bool check(TokenType type);
    bool checkNext(TokenType type);
    TokenType peek();
    bool statementListPending();
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
    public:
        Recognizer(vector<Lexeme*>& lexemes);
};


#endif //PROGRAMMINGLANGUAGES_RECOGNIZER_H
