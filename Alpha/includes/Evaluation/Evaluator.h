//
// Created by Alex Latz on 4/5/22.
//

#ifndef PROGRAMMINGLANGUAGES_EVALUATOR_H
#define PROGRAMMINGLANGUAGES_EVALUATOR_H

#include "../Environment/Environment.h"

class Evaluator {
    public:
        bool evalBoolVal(Lexeme* tree);
        Lexeme* eval(Lexeme* tree, Environment& env);
        Lexeme* evalStatementList(Lexeme* tree, Environment& env);
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
};

#endif //PROGRAMMINGLANGUAGES_EVALUATOR_H