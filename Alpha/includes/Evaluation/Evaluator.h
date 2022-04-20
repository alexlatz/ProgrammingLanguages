//
// Created by Alex Latz on 4/5/22.
//

#ifndef PROGRAMMINGLANGUAGES_EVALUATOR_H
#define PROGRAMMINGLANGUAGES_EVALUATOR_H

#include "../Environment/Environment.h"

class Evaluator {
    vector<pair<string, Lexeme*> > convertParameterList(Lexeme* tree);
    public:
        bool evalBoolVal(Lexeme* tree);
        Lexeme* eval(Lexeme* tree, Environment& env);
        Lexeme* eval(Lexeme* tree, Environment& env, TokenType caller);
        Lexeme* evalStatementList(Lexeme* tree, Environment& env, TokenType caller);
        Lexeme* evalAdd(Lexeme* tree, Environment& env);
        Lexeme* evalSub(Lexeme* tree, Environment& env);
        Lexeme* evalX(Lexeme* tree, Environment& env);
        Lexeme* evalDiv(Lexeme* tree, Environment& env);
        Lexeme* evalMod(Lexeme* tree, Environment& env);
        Lexeme* evalIs(Lexeme* tree, Environment& env);
        Lexeme* evalMore(Lexeme* tree, Environment& env);
        Lexeme* evalLess(Lexeme* tree, Environment& env);
        Lexeme* evalMoreis(Lexeme* tree, Environment& env);
        Lexeme* evalLessis(Lexeme* tree, Environment& env);
        Lexeme* evalAnd(Lexeme* tree, Environment& env);
        Lexeme* evalOr(Lexeme* tree, Environment& env);
        Lexeme* evalNot(Lexeme* tree, Environment& env);
        Lexeme* evalBe(Lexeme* tree, Environment& env);
        Lexeme* evalLet(Lexeme* tree, Environment& env);
        Lexeme* evalCondition(Lexeme* tree, Environment& env);
        Lexeme* evalElse(Lexeme* tree, Environment& env);
        Lexeme* evalInc(Lexeme* tree, Environment& env);
        Lexeme* evalDec(Lexeme* tree, Environment& env);
        Lexeme* evalFor(Lexeme* tree, Environment& env);
        Lexeme* evalWhile(Lexeme* tree, Environment& env);
        Lexeme* evalIdentifier(Lexeme* tree, Environment& env);
        Lexeme* evalFxn(Lexeme* tree, Environment& env);
        Lexeme* evalFxnCall(Lexeme* tree, Environment& env);
        Lexeme* evalReturn(Lexeme* tree, Environment& env);
};

#endif //PROGRAMMINGLANGUAGES_EVALUATOR_H
